#include "globals.h"
#include "pinout.h"
#include <Adafruit_LSM6DSO32.h>
#include <Adafruit_LSM6DS.h>
#include <Adafruit_DPS310.h>
#include "BMI088.h"
#include <Adafruit_BMP3XX.h>
#include <Adafruit_LIS2MDL.h>
#include <Adafruit_HDC302x.h>

// sensor_detect.h
#ifndef SENSOR_DETECT_H
#define SENSOR_DETECT_H

#include <Arduino.h> // or relevant includes if needed

// WhoAmI register address
#define LSM6DS_WHOAMI_ADDR 0x0F
#define DPS310_WHOAMI_ADDR 0x0D
#define BMI_WHOAMI_ADDR 0x00
#define BMP390_WHOAMI_ADDR 0x00
#define LIS2MDL_WHOAMI_ADDR 0x4F
#define HDC302_WHOAMI_ADDR 0x8137

// WhoAmI register values
#define LSM6DS_WHOAMI_VAL 0x6C
#define DPS310_WHOAMI_VAL 0x10
#define BMI_WHOAMI_VAL 0x1E
#define BMP390_WHOAMI_VAL 0x60
#define LIS2MDL_WHOAMI_VAL 0x40
// This is a hacky workaround, the actual data is 0x3000, we just read the upper byte!
#define HDC302_WHOAMI_VAL 0x30

// I2C sensors address
#define BMP390_I2C_ADDR 0x76
#define LISMDL_I2C_ADDR 0x1E
#define HDC302_I2C_ADDR 0x44

// Declare your detectSensor function
String detectSensor();

struct SensorData
{
    float temperature = NAN;
    float pressure = NAN;
    float ax = NAN, ay = NAN, az = NAN;
    float gx = NAN, gy = NAN, gz = NAN;
};

enum SensorType
{
    SENSOR_UNKNOWN,
    SENSOR_LSM6DS_ACCEL_GYRO,
    SENSOR_DPS310_BARO_TEMP,
    SENSOR_BMI088_ACCEL,
    SENSOR_BMP390_BARO,
    SENSOR_LIS2MDL_MAG,
    SENSOR_HDC302_TEMP_HUM
};

#endif // SENSOR_DETECT_H
