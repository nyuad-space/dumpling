#include "flash_write_read.h"

// Enable debug prints if not already defined
#ifndef F411_DEBUG_MODE
#define F411_DEBUG_MODE 1
#endif

// File configuration
#define CIRCULAR_FILE_SIZE   (2 * 1024 * 1024)    // 2 MB circular buffer
#define REGULAR_FILE_SIZE    (13.5 * 1024 * 1024) // 13.5 MB regular logging buffer

// Track file status for writing
bool headersCreated      = false;
uint32_t circularWritePos = 0;

// Track file state for reading
File    currentFile;
const char *currentFileName = nullptr;
bool    fileIsOpen        = false;
String  partialLine       = ""; // buffer for incomplete lines

// ==== Functions ====

// Initialize filesystem for flash writing
bool initFlashWrite() {
    if (!fatfs.begin(&flash_memory)) {
#if F411_DEBUG_MODE
        Serial.println("⚠️ Filesystem not found. Please run formatter first.");
#endif
        return false;
    }
#if F411_DEBUG_MODE
    Serial.println("✅ Filesystem mounted.");
#endif
    return true;
}

SensorFileInfo getSensorFileInfo(SensorType sensorType) {
    SensorFileInfo info = { nullptr, nullptr, nullptr };
    switch (sensorType) {
        case SENSOR_LSM6DS_ACCEL_GYRO:
            info.headers      = "timestamp_ms,accel_x_mss,accel_y_mss,accel_z_mss,gyro_x_rads,gyro_y_rads,gyro_z_rads,temperature_c";
            info.circularName = "lsm6ds_circular.csv";
            info.regularName  = "lsm6ds_data.csv";
            break;
        case SENSOR_DPS310_BARO_TEMP:
            info.headers      = "timestamp_ms,temperature_c,pressure_hpa";
            info.circularName = "dps310_circular.csv";
            info.regularName  = "dps310_data.csv";
            break;
        case SENSOR_BMI088_ACCEL:
            info.headers      = "timestamp_ms,accel_x_mss,accel_y_mss,accel_z_mss,gyro_x_rads,gyro_y_rads,gyro_z_rads,temperature_c";
            info.circularName = "bmi088_circular.csv";
            info.regularName  = "bmi088_data.csv";
            break;
        case SENSOR_BMP390_BARO:
            info.headers      = "timestamp_ms,temperature_c,pressure_hpa,altitude_m";
            info.circularName = "bmp390_circular.csv";
            info.regularName  = "bmp390_data.csv";
            break;
        case SENSOR_LIS2MDL_MAG:
            info.headers      = "timestamp_ms,mag_x_ut,mag_y_ut,mag_z_ut";
            info.circularName = "lis2mdl_circular.csv";
            info.regularName  = "lis2mdl_data.csv";
            break;
        case SENSOR_HDC302_TEMP_HUM:
            info.headers      = "timestamp_ms,humidity_percent,temperature_c";
            info.circularName = "hdc302_circular.csv";
            info.regularName  = "hdc302_data.csv";
            break;
    }
    return info;
}

// Create header and open file for a given sensor
bool initFilesForSensor(SensorType sensorType) {
    SensorFileInfo info = getSensorFileInfo(sensorType);
    if (!info.headers || !info.regularName || !info.circularName) {
        Serial.println("❌ Invalid sensor type");
        return false;
    }

    // --- CIRCULAR FILE SETUP ---
    circularFile = fatfs.open(info.circularName, FILE_WRITE);
    if (!circularFile) {
#if F411_DEBUG_MODE
        Serial.print("❌ Failed to open circular file: ");
        Serial.println(info.circularName);
#endif
        return false;
    }

    if (circularFile.size() == 0) {
        circularFile.truncate(CIRCULAR_FILE_SIZE);
        circularFile.seek(0);
        circularFile.println(info.headers);
        circularFile.flush();
        circularWritePos = circularFile.position();
#if F411_DEBUG_MODE
        Serial.print("🆕 Created circular file with header: ");
        Serial.println(info.circularName);
#endif
    } else {
        circularFile.seek(0);
        circularFile.readStringUntil('\n');
        circularWritePos = circularFile.position();
#if F411_DEBUG_MODE
        Serial.print("📂 Opened existing circular file: ");
        Serial.println(info.circularName);
#endif
    }
    circularFile.close();

    // --- REGULAR FILE SETUP ---
    regularFile = fatfs.open(info.regularName, FILE_WRITE);
    if (!regularFile) {
        Serial.print("❌ Failed to open regular file: ");
        Serial.println(info.regularName);
        return false;
    }

    if (regularFile.size() == 0) {
        regularFile.println(info.headers);
        regularFile.flush();
#if F411_DEBUG_MODE
        Serial.print("🆕 Created regular file with header: ");
        Serial.println(info.regularName);
#endif
    } else {
        if (regularFile.size() >= REGULAR_FILE_SIZE) {
            regularStorageFull = true;
            regularFile.close();
#if F411_DEBUG_MODE
            Serial.print("⚠️ Regular file is full (size >= ");
            Serial.print(REGULAR_FILE_SIZE);
            Serial.println(")");
#endif
        } else {
            regularFile.seek(regularFile.size());
        }
    }
#if F411_DEBUG_MODE
    Serial.print("📂 Opened existing regular file: ");
    Serial.println(info.regularName);
    Serial.println("✅ Both files successfully initialized");
#endif
    regularFile.close();
    return true;
}

// Write to circular buffer
void writeCircular(const String &data, SensorType sensorType) {
    SensorFileInfo info = getSensorFileInfo(sensorType);
    circularFile = fatfs.open(info.circularName, FILE_WRITE);
    if (!circularFile) {
#if F411_DEBUG_MODE
        Serial.print("❌ Failed to open circular file for writing: ");
        Serial.println(info.circularName);
#endif
        return;
    }

    String header = String(info.headers);
    uint32_t headerSize = header.length() + 1; // +1 for newline

#if F411_DEBUG_MODE
    Serial.print("🔄 Circular write pos before: ");
    Serial.println(circularWritePos);
    Serial.print("📏 Data length: ");
    Serial.println(data.length());
#endif

    // wrap if needed
    if (circularWritePos + data.length() >= CIRCULAR_FILE_SIZE) {
        circularWritePos = headerSize;
#if F411_DEBUG_MODE
        Serial.println("🔄 Wrapped circular buffer to after header");
#endif
    }

    bool seekOk = circularFile.seek(circularWritePos);
#if F411_DEBUG_MODE
    Serial.print("⏩ circularFile.seek(");
    Serial.print(circularWritePos);
    Serial.print(") success=");
    Serial.println(seekOk);
#endif

    circularFile.print(data);
    circularFile.flush();
    circularWritePos = circularFile.position();
#if F411_DEBUG_MODE
    Serial.print("🔄 Circular write pos after: ");
    Serial.println(circularWritePos);
    Serial.print("✏️ Data written: ");
    Serial.println(data);
#endif
    circularFile.close();
}

// Write sensor data to flash, auto-switching when regular fills up
void writeToFlash(SensorType sensorType, bool circular) {
    unsigned long timestamp = millis();
    SensorFileInfo info = getSensorFileInfo(sensorType);

    Serial.print("▶️ writeToFlash called (circular=");
    Serial.print(circular);
    Serial.print(", regularFull=");
    Serial.print(regularStorageFull);
    Serial.println(")");

    String dataLine = createDataString(sensorType, timestamp);
    if (dataLine.length() == 0) {
#if F411_DEBUG_MODE
        Serial.println("⚠️ Failed to create data string");
#endif
        return;
    }

    if (circular) {
        writeCircular(dataLine + "\n", sensorType);
#if F411_DEBUG_MODE
        Serial.println("✅ Written to circular buffer");
#endif
    } else {
        if (regularStorageFull) {
#if F411_DEBUG_MODE
            Serial.println("⚠️ Regular storage already full; skipping write");
#endif
            return;
        }

        regularFile = fatfs.open(info.regularName, FILE_WRITE);
        if (!regularFile) {
#if F411_DEBUG_MODE
            Serial.print("❌ Failed to open regular file for writing: ");
            Serial.println(info.regularName);
#endif
            return;
        }

        size_t beforeSize = regularFile.size();
#if F411_DEBUG_MODE
        Serial.print("📏 regularFile.size() before write: ");
        Serial.println(beforeSize);
#endif

        bool seekOk = regularFile.seek(beforeSize);
#if F411_DEBUG_MODE
        Serial.print("⏩ regularFile.seek(");
        Serial.print(beforeSize);
        Serial.print(") success=");
        Serial.println(seekOk);
        Serial.print("✏️ Writing line: ");
        Serial.println(dataLine);
#endif

        regularFile.println(dataLine);
        regularFile.flush();
        
        size_t afterSize = regularFile.size();
#if F411_DEBUG_MODE
        Serial.print("📏 regularFile.size() after write: ");
        Serial.println(afterSize);
#endif

        // check for overflow
        if (afterSize >= REGULAR_FILE_SIZE) {
            regularStorageFull = true;
#if F411_DEBUG_MODE
            Serial.println("⚠️ Regular storage limit reached; future writes will skip");
#endif
        }

        regularFile.close();

#if F411_DEBUG_MODE
        // report free space
        uint32_t freeClusters = fatfs.freeClusterCount();
        uint32_t freeBytes    = freeClusters * fatfs.bytesPerCluster();
        Serial.print("💾 Flash free space (bytes): ");
        Serial.println(freeBytes);
        Serial.println("✅ Written to regular file");
#endif
    }
}

// Helper to build CSV line
String createDataString(SensorType sensorType, unsigned long timestamp) {
    switch (sensorType) {
        case SENSOR_LSM6DS_ACCEL_GYRO:
            return String(timestamp) + "," +
                   String(accel_x_read) + "," +
                   String(accel_y_read) + "," +
                   String(accel_z_read) + "," +
                   String(gyro_x_read)  + "," +
                   String(gyro_y_read)  + "," +
                   String(gyro_z_read)  + "," +
                   String(temp_read);
        case SENSOR_DPS310_BARO_TEMP:
            return String(timestamp) + "," +
                   String(temp_read)   + "," +
                   String(press_read);
        case SENSOR_BMI088_ACCEL:
            return String(timestamp) + "," +
                   String(accel_x_read) + "," +
                   String(accel_y_read) + "," +
                   String(accel_z_read) + "," +
                   String(gyro_x_read)  + "," +
                   String(gyro_y_read)  + "," +
                   String(gyro_z_read)  + "," +
                   String(temp_read);
        case SENSOR_BMP390_BARO:
            return String(timestamp) + "," +
                   String(temp_read)   + "," +
                   String(press_read)  + "," +
                   String(bmp390_alt_read);
        case SENSOR_LIS2MDL_MAG:
            return String(timestamp) + "," +
                   String(mag_x_read)  + "," +
                   String(mag_y_read)  + "," +
                   String(mag_z_read);
        case SENSOR_HDC302_TEMP_HUM:
            return String(timestamp) + "," +
                   String(hum_read)    + "," +
                   String(temp_read);
        default:
            return "";
    }
}

// Utility: get file size
unsigned long getFileSize(const char *filename) {
    File file = fatfs.open(filename, FILE_READ);
    if (file) {
        unsigned long size = file.size();
        file.close();
        Serial.print("📏 getFileSize(");
        Serial.print(filename);
        Serial.print(") = ");
        Serial.println(size);
        return size;
    }
    Serial.print("⚠️ getFileSize: could not open ");
    Serial.println(filename);
    return 0;
}
