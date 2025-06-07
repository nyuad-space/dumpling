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
#include "Adafruit_BusIO_Register.h"

// Bus
extern SPIClass INTERBOARD_SPI;
extern SPIClass FLASH_SPI;
extern SPIClass SENSOR_SPI;
extern TwoWire SENSOR_I2C;

// Sensor objects
extern Adafruit_LSM6DSO32 lsm6ds_accel_gyro;
extern Adafruit_DPS310 dps310_baro_temp;
// extern Bmi088Accel bmi088_accel;
// extern Bmi088Gyro bmi088_gyro;
extern Adafruit_BMP3XX bmp390_baro;
extern Adafruit_LIS2MDL lis2mdl_mag;
extern Adafruit_HDC302x hdc302_temp_hum;

// Sensor type
enum SensorType
{
    SENSOR_UNKNOWN = 0,
    SENSOR_LSM6DS_ACCEL_GYRO,
    SENSOR_DPS310_BARO_TEMP,
    SENSOR_BMI088_ACCEL,
    SENSOR_BMI088_GYRO,
    SENSOR_BMP390_BARO,
    SENSOR_LIS2MDL_MAG,
    SENSOR_HDC302_TEMP_HUM,
};
// Sensor detect
extern SensorType detectedSensor;

#endif
