#pragma once

#include "Arduino.h"
#include "SPI.h"
#include "Wire.h"

// Sensor libraries
#include <Adafruit_LSM6DSOX.h>
#include <Adafruit_DPS310.h>
#include "bmi088.h"
#include <Adafruit_BMP3XX.h>
#include <Adafruit_LIS2MDL.h>
#include <Adafruit_HDC302x.h>

// Flash libraries
#include <Adafruit_SPIFlash.h>
#include <SPIFlash.h>
#include <SdFat_Adafruit_Fork.h>
extern "C"
{
#include "ff.h"
#include "diskio.h"
#include "ffconf.h"
#include "flash_config.h"
}

// Global sensor objects
extern Adafruit_LSM6DSOX lsm6ds;
extern Adafruit_DPS310 dps310;
extern bfs::Bmi088Accel accel;
extern bfs::Bmi088Gyro gyro;
extern Adafruit_BMP3XX bmp3xx;
extern Adafruit_LIS2MDL lis2mdl;
extern Adafruit_HDC302x hdc;
extern Adafruit_MPU6050 mpu;

// Global Flash
extern Adafruit_SPIFlash flash;
extern FatVolume fatfs;

// Others
extern String detectedSensor;