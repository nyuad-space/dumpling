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

#define LSM6DS_WHOAMI_ADDR 0x0F
#define DPS310_WHOAMI_ADDR 0x0D
#define BMI_WHOAMI_ADDR 0x00

#define LSM6DS_WHOAMI 0x6C
#define DPS310_WHOAMI 0x10
#define BMI_WHOAMI 0x1E

extern Adafruit_SPIDevice spi_lsm6ds;
extern Adafruit_BusIO_Register regID_lsm6ds;

extern Adafruit_SPIDevice spi_dps310;
extern Adafruit_BusIO_Register regID_dps310;

extern Adafruit_SPIDevice spi_bmi;
extern Adafruit_BusIO_Register regID_bmi;

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
    SENSOR_LSM6DSO32,
    SENSOR_DPS310,
    SENSOR_Bmi088Accel,
    SENSOR_Bmi088Gyro,
    SENSOR_BMP3xx,
    SENSOR_LIS2MDL,
    SENSOR_HDC302x
};

#endif // SENSOR_DETECT_H
