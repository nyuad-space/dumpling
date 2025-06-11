#include "flash_write_read.h"

// Variables for tracking file state for reading
File currentFile;
const char *currentFileName = nullptr;
bool fileIsOpen = false;
String partialLine = ""; // buffer for incomplete lines

// Initialize flash storage for writing
bool initFlashWrite()
{
    // Initialize the flash file system (assuming fatfs is already initialized in globals)
    if (!fatfs.begin(&flash_memory))
    {
        Serial.println("Filesystem not found. Please run formatter first.");
        while (1)
            yield();
    }
    Serial.println("Filesystem mounted.");

    // Create headers for CSV files if they don't exist
    createCSVHeaders(detectedSensor);

    return true;
}

// Create CSV headers for each sensor file
void createCSVHeaders(SensorType sensorType)
{
    switch (sensorType)
    {
    case SENSOR_LSM6DS_ACCEL_GYRO:
        if (!fatfs.exists("lsm6ds_data.csv"))
        {
            lsm6dsFile = fatfs.open("lsm6ds_data.csv", FILE_WRITE);
            if (lsm6dsFile)
            {
                lsm6dsFile.println("timestamp_ms,accel_x_mss,accel_y_mss,accel_z_mss,gyro_x_rads,gyro_y_rads,gyro_z_rads,temperature_c");
                lsm6dsFile.close();
                Serial.println("Created lsm6ds_data.csv with headers");
            }
        }
        break;

    case SENSOR_DPS310_BARO_TEMP:
        if (!fatfs.exists("dps310_data.csv"))
        {
            dps310File = fatfs.open("dps310_data.csv", FILE_WRITE);
            if (dps310File)
            {
                dps310File.println("timestamp_ms,temperature_c,pressure_hpa");
                dps310File.close();
                Serial.println("Created dps310_data.csv with headers");
            }
        }
        break;

    case SENSOR_BMI088_ACCEL:
        if (!fatfs.exists("bmi088_data.csv"))
        {
            bmi088File = fatfs.open("bmi088_data.csv", FILE_WRITE);
            if (bmi088File)
            {
                bmi088File.println("timestamp_ms,accel_x_mss,accel_y_mss,accel_z_mss,gyro_x_rads,gyro_y_rads,gyro_z_rads,temperature_c");
                bmi088File.close();
                Serial.println("Created bmi088_data.csv with headers");
            }
        }
        break;

    case SENSOR_BMP390_BARO:
        if (!fatfs.exists("bmp390_data.csv"))
        {
            bmp390File = fatfs.open("bmp390_data.csv", FILE_WRITE);
            if (bmp390File)
            {
                bmp390File.println("timestamp_ms,temperature_c,pressure_hpa,altitude_m");
                bmp390File.close();
                Serial.println("Created bmp390_data.csv with headers");
            }
        }
        break;

    case SENSOR_LIS2MDL_MAG:
        if (!fatfs.exists("lis2mdl_data.csv"))
        {
            lis2mdlFile = fatfs.open("lis2mdl_data.csv", FILE_WRITE);
            if (lis2mdlFile)
            {
                lis2mdlFile.println("timestamp_ms,mag_x_ut,mag_y_ut,mag_z_ut");
                lis2mdlFile.close();
                Serial.println("Created lis2mdl_data.csv with headers");
            }
        }
        break;

    case SENSOR_HDC302_TEMP_HUM:
        if (!fatfs.exists("hdc302_data.csv"))
        {
            hdc302File = fatfs.open("hdc302_data.csv", FILE_WRITE);
            if (hdc302File)
            {
                hdc302File.println("timestamp_ms,humidity_percent,temperature_c");
                hdc302File.close();
                Serial.println("Created hdc302_data.csv with headers");
            }
        }
        break;
    }
}

// Write sensor data to appropriate CSV file
void writeToFlash(SensorType sensorType)
{
    unsigned long timestamp = millis();

    switch (sensorType)
    {
    case SENSOR_LSM6DS_ACCEL_GYRO:
    {
        lsm6dsFile = fatfs.open("lsm6ds_data.csv", FILE_WRITE);
        if (lsm6dsFile)
        {
            Serial.println("LSM6DS data written");
            lsm6dsFile.print(timestamp);
            lsm6dsFile.print(",");
            lsm6dsFile.print(accel_x_read);
            lsm6dsFile.print(",");
            lsm6dsFile.print(accel_y_read);
            lsm6dsFile.print(",");
            lsm6dsFile.print(accel_z_read);
            lsm6dsFile.print(",");
            lsm6dsFile.print(gyro_x_read);
            lsm6dsFile.print(",");
            lsm6dsFile.print(gyro_y_read);
            lsm6dsFile.print(",");
            lsm6dsFile.print(gyro_z_read);
            lsm6dsFile.print(",");
            lsm6dsFile.println(temp_read);
            lsm6dsFile.close();
            Serial.println("LSM6DS data written");
        }
        else
        {
            Serial.println("Failed to open lsm6ds_data.csv");
        }
        break;
    }

    case SENSOR_DPS310_BARO_TEMP:
    {
        dps310File = fatfs.open("dps310_data.csv", FILE_WRITE);
        if (dps310File)
        {
            dps310File.print(timestamp);
            dps310File.print(",");
            dps310File.print(temp_read);
            dps310File.print(",");
            dps310File.println(press_read);
            dps310File.close();
            Serial.println("DPS310 data written");
        }
        else
        {
            Serial.println("Failed to open dps310_data.csv");
        }
        break;
    }

    case SENSOR_BMI088_ACCEL:
    {
        bmi088File = fatfs.open("bmi088_data.csv", FILE_WRITE);
        if (bmi088File)
        {
            bmi088File.print(timestamp);
            bmi088File.print(",");
            bmi088File.print(accel_x_read);
            bmi088File.print(",");
            bmi088File.print(accel_y_read);
            bmi088File.print(",");
            bmi088File.print(accel_z_read);
            bmi088File.print(",");
            bmi088File.print(gyro_x_read);
            bmi088File.print(",");
            bmi088File.print(gyro_y_read);
            bmi088File.print(",");
            bmi088File.print(gyro_z_read);
            bmi088File.print(",");
            bmi088File.println(temp_read);
            bmi088File.close();
            Serial.println("BMI088 data written");
        }
        else
        {
            Serial.println("Failed to open bmi088_data.csv");
        }
        break;
    }

    case SENSOR_BMP390_BARO:
    {
        bmp390File = fatfs.open("bmp390_data.csv", FILE_WRITE);
        if (bmp390File)
        {
            bmp390File.print(timestamp);
            bmp390File.print(",");
            bmp390File.print(temp_read);
            bmp390File.print(",");
            bmp390File.print(press_read);
            bmp390File.print(",");
            bmp390File.println(bmp390_alt_read);
            bmp390File.close();
            Serial.println("BMP390 data written");
        }
        else
        {
            Serial.println("Failed to open bmp390_data.csv");
        }
        break;
    }

    case SENSOR_LIS2MDL_MAG:
    {
        lis2mdlFile = fatfs.open("lis2mdl_data.csv", FILE_WRITE);
        if (lis2mdlFile)
        {
            lis2mdlFile.print(timestamp);
            lis2mdlFile.print(",");
            lis2mdlFile.print(mag_x_read);
            lis2mdlFile.print(",");
            lis2mdlFile.print(mag_y_read);
            lis2mdlFile.print(",");
            lis2mdlFile.println(mag_z_read);
            lis2mdlFile.close();
            Serial.println("LIS2MDL data written");
        }
        else
        {
            Serial.println("Failed to open lis2mdl_data.csv");
        }
        break;
    }

    case SENSOR_HDC302_TEMP_HUM:
    {
        hdc302File = fatfs.open("hdc302_data.csv", FILE_WRITE);
        if (hdc302File)
        {
            hdc302File.print(timestamp);
            hdc302File.print(",");
            hdc302File.print(hum_read);
            hdc302File.print(",");
            hdc302File.println(temp_read);
            hdc302File.close();
            Serial.println("HDC302 data written");
        }
        else
        {
            Serial.println("Failed to open hdc302_data.csv");
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
        return size;
    }
    return 0;
}

// Get the appropriate filename for a sensor type
const char *getSensorFilename(SensorType sensorType)
{
    switch (sensorType)
    {
    case SENSOR_LSM6DS_ACCEL_GYRO:
        return "lsm6ds_data.csv";
    case SENSOR_DPS310_BARO_TEMP:
        return "dps310_data.csv";
    case SENSOR_BMI088_ACCEL:
        return "bmi088_data.csv";
    case SENSOR_BMP390_BARO:
        return "bmp390_data.csv";
    case SENSOR_LIS2MDL_MAG:
        return "lis2mdl_data.csv";
    case SENSOR_HDC302_TEMP_HUM:
        return "hdc302_data.csv";
    default:
        return nullptr;
    }
}

void read_by_chunk(File &fileHandle, size_t request_size)
{
    // partialLine = done reading in terms of chunk, but didn't hit new line
    // currentChunk = working variable within chunk, reading char by char

    if (!fileHandle)
    {
        Serial.print("Failed to open csv file");
        return;
    }
    String currentChunk = partialLine; // Start with any partial line from previous read
    size_t fetched_size = 0;
    partialLine = ""; // Clear buffer

    // Read while chunk needs more bytes & file has more data
    while (fetched_size < request_size && fileHandle.available())
    {
        char c = fileHandle.read();
        fetched_size++;

        if (c == '\n') // at new line
        {
            if (currentChunk.length() > 0)
            {
                Serial.println(currentChunk);
                currentChunk = "";
            }
        }
        else if (c != '\r') // if not at new line
        {
            currentChunk += c; // continue reading char by char
        }
    }

    // Handle end of chunk
    if (!(fetched_size < request_size))
    {
        if (currentChunk.length() > 0)
        {
            if (fileHandle.available()) // file has more data
            {
                // more data available, save partial line for next read
                partialLine = currentChunk;
            }
            else
            {
                // EOF, print the last line
                Serial.println(currentChunk);
            }
        }
        Serial.print("Read ");
        Serial.print(fetched_size);
        Serial.println(" bytes");
    }
    else // while fetching chunk
    {
        if (currentChunk.length() > 0) // if there is final line without \n
        {
            Serial.println(currentChunk);
        }
    }
    Serial.println("End of file reached");
    fileHandle.close();
    fileIsOpen = false;
    currentFileName = nullptr;
    partialLine = "";
}

// Read sensor data recordings from flash with size limit
void readFromFlash(SensorType sensorType, size_t request_size)
{
    const char *fileName = getSensorFilename(sensorType);

    // If need to open a new file or switch files
    if (!fileIsOpen || currentFileName != fileName)
    {
        // Close current file if open
        if (fileIsOpen)
        {
            currentFile.close();
        }

        switch (sensorType)
        {
        case SENSOR_LSM6DS_ACCEL_GYRO:
        {
            lsm6dsFile = fatfs.open("lsm6ds_data.csv");
            read_by_chunk(lsm6dsFile, request_size);
            break;
        }
        case SENSOR_DPS310_BARO_TEMP:
        {
            dps310File = fatfs.open("dps310_data.csv");
            read_by_chunk(dps310File, request_size);
            break;
        }
        case SENSOR_BMI088_ACCEL:
        {
            bmi088File = fatfs.open("bmi088_data.csv");
            read_by_chunk(bmi088File, request_size);
            break;
        }
        case SENSOR_BMP390_BARO:
        {
            bmp390File = fatfs.open("bmp390_data.csv");
            read_by_chunk(bmp390File, request_size);
            break;
        }
        case SENSOR_LIS2MDL_MAG:
        {
            lis2mdlFile = fatfs.open("lis2mdl_data.csv");
            read_by_chunk(lis2mdlFile, request_size);
            break;
        }
        case SENSOR_HDC302_TEMP_HUM:
        {
            hdc302File = fatfs.open("hdc302_data.csv");
            read_by_chunk(hdc302File, request_size);
            break;
        }
        }
        // Update file state
        currentFileName = fileName;
        fileIsOpen = true;
        partialLine = ""; // Reset partial chunk buffer when opening new file

        if (!currentFile)
        {
            Serial.print("Failed to open csv file: ");
            Serial.println(fileName);
            fileIsOpen = false;
            return;
        }
    }
    // Read the chunk from the currently open file
    read_by_chunk(currentFile, request_size);
}