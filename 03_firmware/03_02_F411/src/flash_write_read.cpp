#include "flash_write_read.h"

// File configuration
#define CIRCULAR_FILE_SIZE (2 * 1024 * 1024)   // 2MB circular buffer
#define REGULAR_FILE_SIZE (13.5 * 1024 * 1024) // regular logging buffer

// Tracking file state for reading
// File configuration
#define CIRCULAR_FILE_SIZE (2 * 1024 * 1024)   // 2MB circular buffer
#define REGULAR_FILE_SIZE (13.5 * 1024 * 1024) // regular logging buffer

// Tracking file state for reading
File currentFile;
const char *currentFileName = nullptr;
bool fileIsOpen = false;
String partialLine = ""; // buffer for incomplete lines

// Tracking circular buffer
uint32_t circularWritePos = 0;

// ==== Functions ====

// Initialize filesystem for flash writing
bool initFlashWrite(bool clear)
    // Tracking circular buffer
    uint32_t circularWritePos = 0;

// ==== Functions ====

// Initialize filesystem for flash writing
bool initFlashWrite(bool clear)
{
    // Initialize the flash file system (assuming fatfs is already initialized in globals)
    if (!fatfs.begin(&flash_memory))
    {
        Serial.println("Filesystem not found. Please run formatter first.");
        return false;
    }
#if F411_DEBUG_MODE
    Serial.println("Filesystem mounted.");
#endif
    // In DEBUG MODE: CLEAR FLASH for both circular and regular files
    //     if (clear)
    //     {
    //         clearFlash(detectedSensor, 1);
    //         clearFlash(detectedSensor, 0);
    // #if F411_DEBUG_MODE
    //         Serial.println("Flash cleared due to launched state");
    // #endif
    //     }

    return true;
}

// Create CSV headers for each sensor file
void createCSVHeaders(SensorType sensorType)
{
    const char *headers = nullptr;
    const char *regularName = nullptr;
    const char *circularName = nullptr;

    switch (sensorType)
    {
    case SENSOR_LSM6DS_ACCEL_GYRO:
        headers = "timestamp_ms,accel_x_mss,accel_y_mss,accel_z_mss,gyro_x_rads,gyro_y_rads,gyro_z_rads,temperature_c";
        circularName = "lsm6ds_circular.csv";
        regularName = "lsm6ds_data.csv";
        break;

    case SENSOR_DPS310_BARO_TEMP:
        headers = "timestamp_ms,temperature_c,pressure_hpa";
        circularName = "dps310_circular.csv";
        regularName = "dps310_data.csv";
        break;

    case SENSOR_BMI088_ACCEL:
        headers = "timestamp_ms,accel_x_mss,accel_y_mss,accel_z_mss,gyro_x_rads,gyro_y_rads,gyro_z_rads,temperature_c";
        circularName = "bmi088_circular.csv";
        regularName = "bmi088_data.csv";
        break;

    case SENSOR_BMP390_BARO:
        headers = "timestamp_ms,temperature_c,pressure_hpa,altitude_m";
        circularName = "bmp390_circular.csv";
        regularName = "bmp390_data.csv";
        break;

    case SENSOR_LIS2MDL_MAG:
        headers = "timestamp_ms,mag_x_ut,mag_y_ut,mag_z_ut";
        circularName = "lis2mdl_circular.csv";
        regularName = "lis2mdl_data.csv";
        break;

    case SENSOR_HDC302_TEMP_HUM:
        headers = "timestamp_ms,humidity_percent,temperature_c";
        circularName = "hdc302_circular.csv";
        regularName = "hdc302_data.csv";
        break;

    default:
        return;
    }

    // Create circular buffer file
    if (!fatfs.exists(circularName))
    {
        circularFile = fatfs.open(circularName, FILE_WRITE);
        if (circularFile)
        {
            // Pre-allocate space
            circularFile.truncate(CIRCULAR_FILE_SIZE);
            circularFile.seek(0); // go to beginning
            circularFile.println(headers);
            circularWritePos = circularFile.position(); // save starting writing position as after header
            circularFile.close();
#if F411_DEBUG_MODE
            Serial.print("Created circular buffer: ");
            Serial.println(circularName);
#endif
        }
    }
    else
    {
        // Find current position in circular buffer
        circularFile = fatfs.open(circularName, FILE_READ);
        if (circularFile)
        {
            circularFile.readStringUntil('\n'); // Skip header
            circularWritePos = circularFile.position();
            circularFile.close();
        }
    }
    // Create regular file
    if (!fatfs.exists(regularName))
    {
        regularFile = fatfs.open(regularName, FILE_WRITE);
        if (regularFile)
        {
            regularFile.println(headers);
            regularFile.close();
#if F411_DEBUG_MODE
            Serial.print("Created ");
            Serial.println(regularName);
#endif
        }
    }
}

// Write to circular buffer
void writeCircular(File &file, uint32_t &writePos, const String &data)
{
    // Get header size
    file.seek(0);
    String header = file.readStringUntil('\n');
    uint32_t headerSize = header.length() + 1;

    // Check if we need to wrap
    if (writePos + data.length() >= CIRCULAR_FILE_SIZE)
    {
        writePos = headerSize;
    }

    // Write data
    file.seek(writePos);
    file.print(data);
#if F411_DEBUG_MODE
    Serial.println(data);
#endif
    writePos = file.position();
}

// TODO: add logic for after running out of storage for regular logging

// Write sensor data to appropriate CSV file
void writeToFlash(SensorType sensorType, bool circular)
{
    unsigned long timestamp = millis();

    // If regular storage is full
    if (!circular && regularStorageFull)
    {
        Serial.println("Regular storage full");
    }
    switch (sensorType)
    {
    case SENSOR_LSM6DS_ACCEL_GYRO:
    {
        if (circular)
        {
            circularFile = fatfs.open("lsm6ds_circular.csv", FILE_WRITE);
            if (circularFile)
            {
                String dataLine = String(timestamp) + "," +
                                  String(accel_x_read) + "," +
                                  String(accel_y_read) + "," +
                                  String(accel_z_read) + "," +
                                  String(gyro_x_read) + "," +
                                  String(gyro_y_read) + "," +
                                  String(gyro_z_read) + "," +
                                  String(temp_read) + "\n";

                writeCircular(circularFile, circularWritePos, dataLine);
                circularFile.close();
#if F411_DEBUG_MODE
                Serial.println("LSM6DS data written to circular");
#endif
            }
            else
            {
#if F411_DEBUG_MODE
                Serial.println("Failed to open lsm6ds_circular.csv");
#endif
            }
        }
        else
        {
            regularFile = fatfs.open("lsm6ds_data.csv", FILE_WRITE);
            if (regularFile)
            {
                // Check if we're approaching file size limit
                if (regularFile.size() >= REGULAR_FILE_SIZE)
                {
                    regularFile.close();
                    regularStorageFull = true;
#if F411_DEBUG_MODE
                    Serial.println("Regular file size limit reached");
#endif
                    return;
                }
                regularFile.print(timestamp);
                regularFile.print(",");
                regularFile.print(accel_x_read);
                regularFile.print(",");
                regularFile.print(accel_y_read);
                regularFile.print(",");
                regularFile.print(accel_z_read);
                regularFile.print(",");
                regularFile.print(gyro_x_read);
                regularFile.print(",");
                regularFile.print(gyro_y_read);
                regularFile.print(",");
                regularFile.print(gyro_z_read);
                regularFile.print(",");
                regularFile.println(temp_read);
                regularFile.close();
#if F411_DEBUG_MODE
                Serial.print(timestamp);
                Serial.print(",");
                Serial.print(accel_x_read);
                Serial.print(",");
                Serial.print(accel_y_read);
                Serial.print(",");
                Serial.print(accel_z_read);
                Serial.print(",");
                Serial.print(gyro_x_read);
                Serial.print(",");
                Serial.print(gyro_y_read);
                Serial.print(",");
                Serial.print(gyro_z_read);
                Serial.print(",");
                Serial.println(temp_read);
                Serial.println("LSM6DS data written");
#endif
            }
            else
            {
#if F411_DEBUG_MODE
                Serial.println("Failed to open lsm6ds_data.csv");
#endif
            }
        }
        break;
    }

    case SENSOR_DPS310_BARO_TEMP:
    {
        if (circular)
        {
            circularFile = fatfs.open("dps310_circular.csv", FILE_WRITE);
            if (circularFile)
            {
                String dataLine = String(timestamp) + "," +
                                  String(temp_read) + "," +
                                  String(press_read) + "\n";

                writeCircular(circularFile, circularWritePos, dataLine);
                circularFile.close();
#if F411_DEBUG_MODE
                Serial.println("DPS310 data written to circular");
#endif
            }
            else
            {
#if F411_DEBUG_MODE
                Serial.println("Failed to open dps310_circular.csv");
#endif
            }
        }
        else
        {
            regularFile = fatfs.open("dps310_data.csv", FILE_WRITE);
            if (regularFile)
            {
                // Check if we're approaching file size limit
                if (regularFile.size() >= REGULAR_FILE_SIZE)
                {
                    regularFile.close();
                    regularStorageFull = true;
#if F411_DEBUG_MODE
                    Serial.println("Regular file size limit reached");
#endif
                    return;
                }
                regularFile.print(timestamp);
                regularFile.print(",");
                regularFile.print(temp_read);
                regularFile.print(",");
                regularFile.println(press_read);
                regularFile.close();
#if F411_DEBUG_MODE
                Serial.println("DPS310 data written");
#endif
            }
            else
            {
#if F411_DEBUG_MODE
                Serial.println("Failed to open dps310_data.csv");
#endif
            }
        }
        break;
    }

    case SENSOR_BMI088_ACCEL:
    {
        if (circular)
        {
            circularFile = fatfs.open("bmi088_circular.csv", FILE_WRITE);
            if (circularFile)
            {
                String dataLine = String(timestamp) + "," +
                                  String(accel_x_read) + "," +
                                  String(accel_y_read) + "," +
                                  String(accel_z_read) + "," +
                                  String(gyro_x_read) + "," +
                                  String(gyro_y_read) + "," +
                                  String(gyro_z_read) + "," +
                                  String(temp_read) + "\n";

                writeCircular(circularFile, circularWritePos, dataLine);
                circularFile.close();
#if F411_DEBUG_MODE
                Serial.println("BMI088 data written to circular");
#endif
            }
            else
            {
#if F411_DEBUG_MODE
                Serial.println("Failed to open bmi088_circular.csv");
#endif
            }
        }
        else
        {
            regularFile = fatfs.open("bmi088_data.csv", FILE_WRITE);
            if (regularFile)
            {
                // Check if we're approaching file size limit
                if (regularFile.size() >= REGULAR_FILE_SIZE)
                {
                    regularFile.close();
                    regularStorageFull = true;
#if F411_DEBUG_MODE
                    Serial.println("Regular file size limit reached");
#endif
                    return;
                }
                regularFile.print(timestamp);
                regularFile.print(",");
                regularFile.print(accel_x_read);
                regularFile.print(",");
                regularFile.print(accel_y_read);
                regularFile.print(",");
                regularFile.print(accel_z_read);
                regularFile.print(",");
                regularFile.print(gyro_x_read);
                regularFile.print(",");
                regularFile.print(gyro_y_read);
                regularFile.print(",");
                regularFile.print(gyro_z_read);
                regularFile.print(",");
                regularFile.println(temp_read);
                regularFile.close();
#if F411_DEBUG_MODE
                Serial.println("BMI088 data written");
#endif
            }
            else
            {
#if F411_DEBUG_MODE
                Serial.println("Failed to open bmi088_data.csv");
#endif
            }
        }
        break;
    }

    case SENSOR_BMP390_BARO:
    {
        if (circular)
        {
            circularFile = fatfs.open("bmp390_circular.csv", FILE_WRITE);
            if (circularFile)
            {
                String dataLine = String(timestamp) + "," +
                                  String(temp_read) + "," +
                                  String(press_read) + "," +
                                  String(bmp390_alt_read) + "\n";

                writeCircular(circularFile, circularWritePos, dataLine);
                circularFile.close();
#if F411_DEBUG_MODE
                Serial.println("BMP390 data written to circular");
#endif
            }
            else
            {
#if F411_DEBUG_MODE
                Serial.println("Failed to open bmp390_circular.csv");
#endif
            }
        }
        else
        {
            regularFile = fatfs.open("bmp390_data.csv", FILE_WRITE);
            if (regularFile)
            {
                // Check if we're approaching file size limit
                if (regularFile.size() >= REGULAR_FILE_SIZE)
                {
                    regularFile.close();
                    regularStorageFull = true;
#if F411_DEBUG_MODE
                    Serial.println("Regular file size limit reached");
#endif
                    return;
                }
                regularFile.print(timestamp);
                regularFile.print(",");
                regularFile.print(temp_read);
                regularFile.print(",");
                regularFile.print(press_read);
                regularFile.print(",");
                regularFile.println(bmp390_alt_read);
                regularFile.close();
#if F411_DEBUG_MODE
                Serial.println("BMP390 data written");
#endif
            }
            else
            {
#if F411_DEBUG_MODE
                Serial.println("Failed to open bmp390_data.csv");
#endif
            }
        }
        break;
    }

    case SENSOR_LIS2MDL_MAG:
    {
        if (circular)
        {
            circularFile = fatfs.open("lis2mdl_circular.csv", FILE_WRITE);
            if (circularFile)
            {
                String dataLine = String(timestamp) + "," +
                                  String(mag_x_read) + "," +
                                  String(mag_y_read) + "," +
                                  String(mag_z_read) + "\n";

                writeCircular(circularFile, circularWritePos, dataLine);
                circularFile.close();
#if F411_DEBUG_MODE
                Serial.println("LIS2MDL data written to circular");
#endif
            }
            else
            {
#if F411_DEBUG_MODE
                Serial.println("Failed to open lis2mdl_circular.csv");
#endif
            }
        }
        else
        {
            regularFile = fatfs.open("lis2mdl_data.csv", FILE_WRITE);
            if (regularFile)
            {
                // Check if we're approaching file size limit
                if (regularFile.size() >= REGULAR_FILE_SIZE)
                {
                    regularFile.close();
                    regularStorageFull = true;
#if F411_DEBUG_MODE
                    Serial.println("Regular file size limit reached");
#endif
                    return;
                }
                regularFile.print(timestamp);
                regularFile.print(",");
                regularFile.print(mag_x_read);
                regularFile.print(",");
                regularFile.print(mag_y_read);
                regularFile.print(",");
                regularFile.println(mag_z_read);
                regularFile.close();
#if F411_DEBUG_MODE
                Serial.println("LIS2MDL data written");
#endif
            }
            else
            {
#if F411_DEBUG_MODE
                Serial.println("Failed to open lis2mdl_data.csv");
#endif
            }
        }
        break;
    }

    case SENSOR_HDC302_TEMP_HUM:
    {
        if (circular)
        {
            circularFile = fatfs.open("hdc302_circular.csv", FILE_WRITE);
            if (circularFile)
            {
                String dataLine = String(timestamp) + "," +
                                  String(hum_read) + "," +
                                  String(temp_read) + "\n";

                writeCircular(circularFile, circularWritePos, dataLine);
                circularFile.close();
#if F411_DEBUG_MODE
                Serial.println("HDC302 data written to circular");
#endif
            }
            else
            {
#if F411_DEBUG_MODE
                Serial.println("Failed to open hdc302_circular.csv");
#endif
            }
        }
        else
        {
            regularFile = fatfs.open("hdc302_data.csv", FILE_WRITE);
            if (regularFile)
            {
                // Check if we're approaching file size limit
                if (regularFile.size() >= REGULAR_FILE_SIZE)
                {
                    regularFile.close();
                    regularStorageFull = true;
#if F411_DEBUG_MODE
                    Serial.println("Regular file size limit reached");
#endif
                    return;
                }
                regularFile.print(timestamp);
                regularFile.print(",");
                regularFile.print(hum_read);
                regularFile.print(",");
                regularFile.println(temp_read);
                regularFile.close();
#if F411_DEBUG_MODE
                Serial.println("HDC302 data written");
#endif
            }
            else
            {
#if F411_DEBUG_MODE
                Serial.println("Failed to open hdc302_data.csv");
#endif
            }
        }
        break;
    }
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

// Get the appropriate filename for a sensor type
const char *getSensorFilename(SensorType sensorType, bool circular)
{
    const char *fileName;
    switch (sensorType)
    {
    case SENSOR_LSM6DS_ACCEL_GYRO:
        fileName = circular ? "lsm6ds_circular.csv" : "lsm6ds_data.csv";
        break;
    case SENSOR_DPS310_BARO_TEMP:
        fileName = circular ? "dps310_circular.csv" : "dps310_data.csv";
        break;
    case SENSOR_BMI088_ACCEL:
        fileName = circular ? "bmi088_circular.csv" : "bmi088_data.csv";
        break;
    case SENSOR_BMP390_BARO:
        fileName = circular ? "bmp390_circular.csv" : "bmp390_data.csv";
        break;
    case SENSOR_LIS2MDL_MAG:
        fileName = circular ? "lis2mdl_circular.csv" : "lis2mdl_data.csv";
        break;
    case SENSOR_HDC302_TEMP_HUM:
        fileName = circular ? "hdc302_circular.csv" : "hdc302_data.csv";
        break;
    default:
        fileName = nullptr;
    }
    return fileName;
}

bool readFromFlash(SensorType sensorType, uint8_t *buffer, size_t buffer_size, bool circular)
{
    const char *fileName = getSensorFilename(sensorType, circular);

    // Check if we need to switch files
    if (!fileIsOpen || currentFileName != fileName)
    {
        // Close current file if open
        if (fileIsOpen && currentFile)
        {
            currentFile.close();
            fileIsOpen = false;
            partialLine = "";   // Reset when switching files
            reachedEOF = false; // Reset EOF flag when switching files
        }

        // Open the new file into currentFile
        if (fileName == nullptr)
        {
            Serial.println("Unknown sensor type");
            return false;
        }

        currentFile = fatfs.open(fileName);

        // Check if file opened successfully
        if (!currentFile)
        {
            Serial.print("Failed to open csv file: ");
            Serial.println(fileName);
            fileIsOpen = false;
            return false;
        }

        // Update state for successful open
        currentFileName = fileName;
        fileIsOpen = true;
        reachedEOF = false;
    }

    // Read chunk from the current file
    size_t bytesRead = read_by_chunk(currentFile, buffer, buffer_size, circular);

    // Check if buffer is full (read exactly the requested amount)
    bool bufferFull = (bytesRead == buffer_size);
#if F411_DEBUG_MODE
    if (bufferFull)
    {
        Serial.println("Buffer completely filled");
    }
#endif
    return (bytesRead > 0); // return true if successfully read data, false if EOF or error
}

// Read sensor data recordings from flash with size limit
size_t read_by_chunk(File &fileHandle, uint8_t *buffer, size_t buffer_size, bool circular)
{
    // partialLine = done reading in terms of chunk, but didn't hit new line
    // currentChunk = working variable within chunk, reading char by char

    if (!fileHandle)
    {
        Serial.println("Failed to open csv file");
        return 0;
    }

    String currentChunk = partialLine; // Start with any partial line from previous read
    size_t fetched_size = 0;
    size_t buffer_pos = 0;
    bool bufferFull = false;
    partialLine = ""; // Clear buffer

    // Read requested bytes or until EOF
    while (fetched_size < buffer_size && fileHandle.available())
    {
        char c = fileHandle.read();
        fetched_size++;

        if (c == '\n')
        {
            if (currentChunk.length() > 0)
            {
                // Write to buffer with newline
                size_t line_len = currentChunk.length();
                if (buffer_pos + line_len + 1 < buffer_size) // +1 for newline
                {
                    memcpy(buffer + buffer_pos, currentChunk.c_str(), line_len);
                    buffer_pos += line_len;
                    buffer[buffer_pos++] = '\n';
#if F411_DEBUG_MODE
                    Serial.println(currentChunk);
#endif
                }
                else
                {
                    partialLine = currentChunk; // can't fit this line in buffer, save it for next time
#if F411_DEBUG_MODE
                    Serial.println("Buffer full, saving line for next read");
#endif
                    break;
                }

                currentChunk = "";
            }
        }
        else if (c != '\r') // if not at new line
        {
            currentChunk += c;
        }
    }

    // Handle any remaining data in currentChunk
    if (currentChunk.length() > 0 && buffer_pos < buffer_size)
    {
        if (fileHandle.available() > 0)
        {
            // More data exists, save partial line for next chunk
            partialLine = currentChunk;
        }
        else
        {
            // At EOF, write the last line if it fits
            size_t line_len = currentChunk.length();
            if (buffer_pos + line_len + 1 < buffer_size)
            {
                memcpy(buffer + buffer_pos, currentChunk.c_str(), line_len);
                buffer_pos += line_len;
                buffer[buffer_pos++] = '\n';
#if F411_DEBUG_MODE
                Serial.println(currentChunk);
#endif
            }
            else
            {
                partialLine = currentChunk; // can't fit this line in buffer, save it for next time
#if F411_DEBUG_MODE
                Serial.println("Buffer full, saving line for next read");
#endif
            }
        }
    }

    // Null terminate the buffer
    if (buffer_pos < buffer_size)
    {
        buffer[buffer_pos] = '\0';
    }

#if F411_DEBUG_MODE
    Serial.print("Read ");
    Serial.print(fetched_size);
    Serial.println(" bytes from file, wrote ");
    Serial.println(buffer_pos);
    Serial.println(" bytes to buffer");
#endif

    // Only close file if we've reached EOF
    if (!fileHandle.available() && partialLine.length() == 0)
    {
        Serial.println("End of file reached");
        fileHandle.close();
        fileIsOpen = false;
        currentFileName = nullptr;
        partialLine = "";
        reachedEOF = true;
    }
    return buffer_pos; // return number of bytes actually written to buffer
}

// Clear/delete the CSV file for the specified sensor type
bool clearFlash(SensorType sensorType, bool circular)
{
    // Clear both circular and regular files
    const char *fileName = getSensorFilename(sensorType, circular);

    if (fileName == nullptr)
    {
#if F411_DEBUG_MODE
        Serial.println("Unknown sensor type - cannot clear file");
#endif
        return false;
    }

    // Close the file if it's currently open for reading
    if (fileIsOpen && currentFileName == fileName)
    {
        currentFile.close();
        fileIsOpen = false;
        currentFileName = nullptr;
        partialLine = "";
    }

    // Check if file exists before attempting to delete
    if (fatfs.exists(fileName))
    {
        if (fatfs.remove(fileName))
        {
#if F411_DEBUG_MODE
            Serial.print("Successfully deleted: ");
            Serial.println(fileName);
#endif
            // Reset storage full flag
            regularStorageFull = false;
            return true;
        }
        else
        {
#if F411_DEBUG_MODE
            Serial.print("Failed to delete: ");
            Serial.println(fileName);
#endif
            return false;
        }
    }
    else
    {
#if F411_DEBUG_MODE
        Serial.print("File does not exist: ");
        Serial.println(fileName);
#endif
        return true; // Consider this successful since file is already gone
    }
}