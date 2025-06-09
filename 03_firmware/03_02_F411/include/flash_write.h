#include "globals.h"
#include "pinout.h"
#include <Adafruit_LSM6DSO32.h>
#include <Adafruit_LSM6DS.h>
#include <Adafruit_DPS310.h>
#include "BMI088.h"
#include <Adafruit_BMP3XX.h>
#include <Adafruit_LIS2MDL.h>
#include <Adafruit_HDC302x.h>
#include "sensor_detect.h"
#include "sensor_read.h"

#include <Arduino.h>

// sensor_detect.h
#ifndef FLASH_WRITE_H
#define FLASH_WRITE_H

// Union to store different sensor data types efficiently
union flashStruct
{
    struct
    {
        float accel_x_read;
        float accel_y_read;
        float accel_z_read;
    } accel;

    struct
    {
        float gyro_x_read;
        float gyro_y_read;
        float gyro_z_read;
    } gyro;

    struct
    {
        float temp_read;
        float press_read;
        float hum_read;
        float bmp390_alt_read;
    } environmental;

    struct
    {
        float mag_x_read;
        float mag_y_read;
        float mag_z_read;
    } magnetometer;
};

// Main sensor record structure
struct SensorRecord
{
    unsigned long timestamp; // Timestamp in milliseconds
    SensorType type;         // Type of sensor
    SensorData data;         // Actual sensor data
    uint8_t dataSize;        // Size of valid data in bytes
};

// Function declarations
bool writeSensorToFlash(SensorType sensorType, const flashStruct &data, unsigned long flashAddress = 0);
bool readSensorFromFlash(SensorRecord &record, unsigned long flashAddress = 0);
// uint8_t getSensorDataSize(SensorType sensorType);

#endif