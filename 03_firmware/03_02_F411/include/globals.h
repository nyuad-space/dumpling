#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include "SPI.h"
#include "Wire.h"
#include <Adafruit_LSM6DSO32.h>
#include <Adafruit_DPS310.h>
#include <BMI088.h>
#include <Adafruit_BMP3XX.h>
#include <Adafruit_LIS2MDL.h>
#include <Adafruit_HDC302x.h>

// Bus
extern SPIClass INTERBOARD_SPI;
extern TwoWire SENSOR_I2C;
extern SPIClass SENSOR_SPI;
extern SPIClass FLASH_SPI;

// Sensor objects
extern Adafruit_LSM6DSO32 lsm6ds;
extern Adafruit_DPS310 dps310;
extern Bmi088Accel bmiAccel;
extern Bmi088Gyro bmiGyro;
extern Adafruit_BMP3XX bmp3xx;
extern Adafruit_LIS2MDL lis2mdl;
extern Adafruit_HDC302x hdc;

// Sensor type
enum SensorType
{
    SENSOR_UNKNOWN = 0,
    SENSOR_LSM6DSO32,
    SENSOR_DPS310,
    SENSOR_Bmi088Accel,
    SENSOR_Bmi088Gyro,
    SENSOR_BMP3xx,
    SENSOR_LIS2MDL,
    SENSOR_HDC302
};
// Sensor detect
SensorType detectedSensor;

#endif
