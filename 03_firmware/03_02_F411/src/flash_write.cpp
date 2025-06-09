#include "globals.h"
#include "flash_write.h"

static unsigned long currentFlashAddress = 0;

bool writeSensorToFlash(SensorType sensorType, const flashStruct &data, unsigned long flashAddress)
{
    SensorRecord record;
    record.timestamp = millis();
    record.type = sensorType;
    record.data = data;

    // Use provided address or auto-increment
    unsigned long writeAddress = (flashAddress == 0) ? currentFlashAddress : flashAddress;

    // Open file and append data
    sensorFile = fatfs.open("sensor.txt", FILE_WRITE);
    if (sensorFile)
    {

        // TODO: Do other cases, Format sensorFile.print contents and header
        switch (sensorType)
        {
            // XYZ data
        case SENSOR_LSM6DS_ACCEL_GYRO || SENSOR_BMI088_ACCEL || SENSOR_LIS2MDL_MAG:
            Serial.println("Writing accelerometer data to flash");
            // Validate data before writing
            if (isnan(data.accel.accel_x_read) || isnan(data.accel.accel_y_read) || isnan(data.accel.accel_z_read))
            {
                Serial.println("Error: Invalid accelerometer data");
                return false;
            }
            // Write header
            sensorFile.println("Timestamp,Accel_X(g),Accel_Y(g),Accel_Z(g),Gyro_X(°/s),Gyro_Y(°/s),Gyro_Z(°/s),Temp(°C)");

            // Write ata rows with timestamp
            sensorFile.print(millis());
            sensorFile.print(",");
            sensorFile.print(accel_x_read);
            sensorFile.print(",");
            sensorFile.print(accel_y_read);
            sensorFile.print(",");
            sensorFile.print(accel_z_read);
            sensorFile.print(",");
            sensorFile.print(gyro_x_read);
            sensorFile.print(",");
            sensorFile.print(gyro_y_read);
            sensorFile.print(",");
            sensorFile.print(gyro_z_read);
            sensorFile.print(",");
            sensorFile.println(temp_read);
            break;

            // Temperature, Pressure data
        case SENSOR_DPS310_BARO_TEMP || SENSOR_BMP390_BARO:
            Serial.println("Writing gyroscope data to flash");
            if (isnan(data.gyro.gyro_x_read) || isnan(data.gyro.gyro_y_read) || isnan(data.gyro.gyro_z_read))
            {
                Serial.println("Error: Invalid gyroscope data");
                return false;

                // TODO: sensorFile.print();
            }

            break;

            // Temperature, Humidity data
        case SENSOR_HDC302_TEMP_HUM:
            Serial.println("Writing magnetometer data to flash");
            if (isnan(data.magnetometer.mag_x_read) || isnan(data.magnetometer.mag_y_read) || isnan(data.magnetometer.mag_z_read))
            {
                Serial.println("Error: Invalid magnetometer data");
                return false;
            }
            // TODO: sensorFile.print();
            break;

        case SENSOR_UNKNOWN:
            break;

        default:
            Serial.println("Error: Unknown sensor type");
            return false;
        }
    }

    else
    {
        Serial.println("Failed to open sensor.txt for writing");
    }

    // // Update global address counter if using auto-increment
    // if (flashAddress == 0)
    // {
    //     currentFlashAddress += recordSize;
    // }

    // Serial.print("Successfully wrote ");
    // Serial.print(recordSize);
    // Serial.print(" bytes to flash at address ");
    // Serial.println(writeAddress);

    // return true;
}

bool readSensorFromFlash(SensorRecord &record, unsigned long flashAddress)
{

    return true;
}