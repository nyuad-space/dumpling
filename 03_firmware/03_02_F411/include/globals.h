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
#include "SdFat_Adafruit_Fork.h"
#include <Adafruit_SPIFlash.h>
#include "flash_write.h"
// Bus
extern SPIClass INTERBOARD_SPI;
extern TwoWire SENSOR_I2C;
extern SPIClass SENSOR_SPI;
extern SPIClass FLASH_SPI;

// Sensor objects
extern Adafruit_LSM6DSO32 lsm6ds_accel_gyro;
extern Adafruit_DPS310 dps310_baro_temp;
extern Bmi088Accel bmi088_accel;
extern Bmi088Gyro bmi088_gyro;
extern Adafruit_BMP3XX bmp390_baro;
extern Adafruit_LIS2MDL lis2mdl_mag;
extern Adafruit_HDC302x hdc302_temp_hum;

// Sensor detect
extern SensorType detectedSensor;

// Flash objects
extern Adafruit_FlashTransport_SPI adafruit_flash_spi;
extern Adafruit_SPIFlash flash_memory;
extern FatVolume fatfs;
extern File32 lsm6dsFile;
extern File32 dps310File;
extern File32 bmi088File;
extern File32 bmp390File;
extern File32 lis2mdlFile;
extern File32 hdc302File;

// Sensor readings
extern float accel_x_read;
extern float accel_y_read;
extern float accel_z_read;
extern float gyro_x_read;
extern float gyro_y_read;
extern float gyro_z_read;
extern float temp_read;
extern float press_read;
extern float bmp390_alt_read;
extern float mag_x_read;
extern float mag_y_read;
extern float mag_z_read;
extern float hum_read;

#endif
