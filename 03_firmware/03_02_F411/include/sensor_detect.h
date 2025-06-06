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

// SPI devices
extern Adafruit_SPIDevice adafruit_spi_device;

extern Adafruit_BusIO_Register regID_lsm6ds;
extern Adafruit_BusIO_Register regID_dps310;
extern Adafruit_BusIO_Register regID_bmi;

// I2C devices
extern Adafruit_I2CDevice i2c_bmp390;
extern Adafruit_BusIO_Register regID_bmp390;

extern Adafruit_I2CDevice i2c_lis2mdl;
extern Adafruit_BusIO_Register regID_lis2mdl;

extern Adafruit_I2CDevice i2c_hdc302;
extern Adafruit_BusIO_Register regID_hdc302;

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
    SENSOR_HDC302
};

#endif // SENSOR_DETECT_H
