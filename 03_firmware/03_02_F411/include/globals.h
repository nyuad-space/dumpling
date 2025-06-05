#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include "SPI.h"
#include "Wire.h"
#include <Adafruit_LSM6DSO32.h>
#include <Adafruit_DPS310.h>
#include "BMI088.h"
#include <Adafruit_BMP3XX.h>
#include <Adafruit_LIS2MDL.h>
#include <Adafruit_HDC302x.h>
#include "sensor_detect.h"

// Bus
extern SPIClass INTERBOARD_SPI;
extern TwoWire SENSOR_I2C;
extern SPIClass SENSOR_SPI;
extern SPIClass FLASH_SPI;

// Sensor objects
extern Adafruit_LSM6DS lsm6ds;
extern Adafruit_DPS310 dps310;
extern Bmi088Accel bmiAccel;
extern Bmi088Gyro bmiGyro;
extern Adafruit_BMP3XX bmp3xx;
extern Adafruit_LIS2MDL lis2mdl;
extern Adafruit_HDC302x hdc;

// Sensor detect
extern SensorType detectedSensor;

#endif
