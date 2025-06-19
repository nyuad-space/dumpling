#include "flash_write_read.h"

// File configuration
#define CIRCULAR_FILE_SIZE (2 * 1024 * 1024)   // 2MB circular buffer
#define REGULAR_FILE_SIZE (13.5 * 1024 * 1024) // regular logging buffer

// Track file status for writing
bool headersCreated = false;
uint32_t circularWritePos = 0;

// Track file state for reading
File currentFile;
const char *currentFileName = nullptr;
bool fileIsOpen = false;
String partialLine = ""; // buffer for incomplete lines

// ==== Functions ====

// Initialize filesystem for flash writing
bool initFlashWrite()
{
    // Initialize the flash file system (assuming fatfs is already initialized in globals)
    if (!fatfs.begin(&flash_memory))
    {
#if F411_DEBUG_MODE
        Serial.println("Filesystem not found. Please run formatter first.");
#endif
        return false;
    }
#if F411_DEBUG_MODE
    Serial.println("Filesystem mounted.");
#endif

    return true;
}

SensorFileInfo getSensorFileInfo(SensorType sensorType)
{
    SensorFileInfo info = {nullptr, nullptr, nullptr};
    switch (sensorType)
    {
    case SENSOR_LSM6DS_ACCEL_GYRO:
        info.headers = "timestamp_ms,accel_x_mss,accel_y_mss,accel_z_mss,gyro_x_rads,gyro_y_rads,gyro_z_rads,temperature_c";
        info.circularName = "lsm6ds_circular.csv";
        info.regularName = "lsm6ds_data.csv";
        break;

    case SENSOR_DPS310_BARO_TEMP:
        info.headers = "timestamp_ms,temperature_c,pressure_hpa";
        info.circularName = "dps310_circular.csv";
        info.regularName = "dps310_data.csv";
        break;

    case SENSOR_BMI088_ACCEL:
        info.headers = "timestamp_ms,accel_x_mss,accel_y_mss,accel_z_mss,gyro_x_rads,gyro_y_rads,gyro_z_rads,temperature_c";
        info.circularName = "bmi088_circular.csv";
        info.regularName = "bmi088_data.csv";
        break;

    case SENSOR_BMP390_BARO:
        info.headers = "timestamp_ms,temperature_c,pressure_hpa,altitude_m";
        info.circularName = "bmp390_circular.csv";
        info.regularName = "bmp390_data.csv";
        break;

    case SENSOR_LIS2MDL_MAG:
        info.headers = "timestamp_ms,mag_x_ut,mag_y_ut,mag_z_ut";
        info.circularName = "lis2mdl_circular.csv";
        info.regularName = "lis2mdl_data.csv";
        break;

    case SENSOR_HDC302_TEMP_HUM:
        info.headers = "timestamp_ms,humidity_percent,temperature_c";
        info.circularName = "hdc302_circular.csv";
        info.regularName = "hdc302_data.csv";
        break;
    }
    return info;
}

// Create header and open file
bool initFilesForSensor(SensorType sensorType)
{
    SensorFileInfo info = getSensorFileInfo(sensorType);

    if (!info.headers || !info.regularName || !info.circularName)
    {
        Serial.println("Invalid sensor type");
        return false;
    }
    // // Close any previously open files
    // circularFile.close();
    // regularFile.close();

    // == CIRCULAR FILE SETUP ==
    circularFile = fatfs.open(info.circularName, FILE_WRITE);
    if (!circularFile)
    {
#if F411_DEBUG_MODE
        Serial.print("Failed to open circular file: ");
        Serial.println(info.circularName);
#endif
        return false;
    }
    // New/Empty file: create header and pre-allocate space
    if (circularFile.size() == 0)
    {
        circularFile.truncate(CIRCULAR_FILE_SIZE);
        circularFile.seek(0); // go to beginning
        circularFile.println(info.headers);
        circularFile.flush();                       // Ensure header is written
        circularWritePos = circularFile.position(); // save starting writing position as after header
#if F411_DEBUG_MODE
        Serial.print("Created circular file with header: ");
        Serial.println(info.circularName);
#endif
    }
    else
    {
        // Existing file: find current write position
        circularFile.seek(0);
        circularFile.readStringUntil('\n'); // write from after header
        circularWritePos = circularFile.position();
#if F411_DEBUG_MODE
        Serial.print("Opened existing circular file: ");
        Serial.println(info.circularName);
#endif
    }
    // Only one file can be open at a time so close this one before opening another.
    circularFile.close();
    delay(10);

    // === REGULAR FILE SETUP ===
    regularFile = fatfs.open(info.regularName, FILE_WRITE);
    if (!regularFile)
    {
        Serial.print("Failed to open regular file: ");
        Serial.println(info.regularName);
        return false;
    }

    if (regularFile.size() == 0)
    {
        // New/Empty file: create header
        regularFile.println(info.headers);
        regularFile.flush(); // Ensure header is written
#if F411_DEBUG_MODE
        Serial.print("Created regular file with header: ");
        Serial.println(info.regularName);
#endif
    }
    else
    { // Check if storage is already full
        if (regularFile.size() >= REGULAR_FILE_SIZE)
        {
            regularStorageFull = true;
            regularFile.close();
#if F411_DEBUG_MODE
            Serial.print("Regular file is full");
#endif
        }
        else
        {
            // Existing file: append new data at the end of file
            regularFile.seek(regularFile.size());
        }
    }
#if F411_DEBUG_MODE
    Serial.print("Opened existing regular file: ");
    Serial.println(info.regularName);
#endif
    // Only one file can be open at a time so close this one before opening another.
    regularFile.close();

#if F411_DEBUG_MODE
    Serial.println("Both files successfully initialized");
#endif

    return true;
}

// Write to circular buffer
void writeCircular(const String &data, SensorType sensorType)
{
    SensorFileInfo info = getSensorFileInfo(sensorType);

    // Open circular file for writing
    circularFile = fatfs.open(info.circularName, FILE_WRITE);
    if (!circularFile)
    {
#if F411_DEBUG_MODE
        Serial.print("Failed to open circular file for writing: ");
        Serial.println(info.circularName);
#endif
        return;
    }

    // Get header size
    String header = String(info.headers);
    uint32_t headerSize = header.length() + 1; // +1 for newline

    // Check if we need to wrap
    if (circularWritePos + data.length() >= CIRCULAR_FILE_SIZE)
    {
        circularWritePos = headerSize;
    }

    // Write data
    circularFile.seek(circularWritePos);
    circularFile.print(data);
    circularFile.flush();
#if F411_DEBUG_MODE
    Serial.println(data);
#endif
    circularWritePos = circularFile.position();
    circularFile.close();
}

// Write sensor data to appropriate CSV file
void writeToFlash(SensorType sensorType, bool circular)
{
    unsigned long timestamp = millis();
    SensorFileInfo info = getSensorFileInfo(sensorType);

    // If regular storage is full
    if (!circular && regularStorageFull)
    {
#if F411_DEBUG_MODE
        Serial.println("Regular storage full");
#endif
        return;
    }

    // Create data string based on sensor type
    String dataLine = createDataString(sensorType, timestamp);
    if (dataLine.length() == 0)
    {
#if F411_DEBUG_MODE
        Serial.println("Failed to create data string");
#endif
        return;
    }

    // Write to appropriate file
    if (circular)
    {
        writeCircular(dataLine + "\n", sensorType);
#if F411_DEBUG_MODE
        Serial.println("Written to circular buffer");
#endif
    }
    else
    {
        // Open circular file for writing
        regularFile = fatfs.open(info.regularName, FILE_WRITE);
        if (!regularFile)
        {
#if F411_DEBUG_MODE
            Serial.print("Failed to open regular file for writing: ");
            Serial.println(info.regularName);
#endif
            return;
        }
        // Check file size limit
        if (regularFile.size() >= REGULAR_FILE_SIZE)
        {
            regularStorageFull = true;
            regularFile.close();
#if F411_DEBUG_MODE
            Serial.println("Regular file size limit reached");
#endif
            return;
        }
        // Seek to end of file for appending
        regularFile.seek(regularFile.size());
        regularFile.println(dataLine);
        regularFile.flush(); // Force immediate write to flash
        regularFile.close();
#if F411_DEBUG_MODE
        Serial.println("Written to regular file");
#endif
    }
}

// Helper function to create data string (reduces code duplication)
String createDataString(SensorType sensorType, unsigned long timestamp)
{
    switch (sensorType)
    {
    case SENSOR_LSM6DS_ACCEL_GYRO:
        return String(timestamp) + "," +
               String(accel_x_read) + "," +
               String(accel_y_read) + "," +
               String(accel_z_read) + "," +
               String(gyro_x_read) + "," +
               String(gyro_y_read) + "," +
               String(gyro_z_read) + "," +
               String(temp_read);

    case SENSOR_DPS310_BARO_TEMP:
        return String(timestamp) + "," +
               String(temp_read) + "," +
               String(press_read);

    case SENSOR_BMI088_ACCEL:
        return String(timestamp) + "," +
               String(accel_x_read) + "," +
               String(accel_y_read) + "," +
               String(accel_z_read) + "," +
               String(gyro_x_read) + "," +
               String(gyro_y_read) + "," +
               String(gyro_z_read) + "," +
               String(temp_read);

    case SENSOR_BMP390_BARO:
        return String(timestamp) + "," +
               String(temp_read) + "," +
               String(press_read) + "," +
               String(bmp390_alt_read);

    case SENSOR_LIS2MDL_MAG:
        return String(timestamp) + "," +
               String(mag_x_read) + "," +
               String(mag_y_read) + "," +
               String(mag_z_read);

    case SENSOR_HDC302_TEMP_HUM:
        return String(timestamp) + "," +
               String(hum_read) + "," +
               String(temp_read);

    default:
        return "";
    }
}

// Get file size for monitoring storage usage
unsigned long getFileSize(const char *filename)
{
    File file = fatfs.open(filename, FILE_READ);
    if (file)
    {
        unsigned long size = file.size();
        file.close();
        Serial.println(size);
        return size;
    }
    return 0;
}