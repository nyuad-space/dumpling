#pragma once

#include "Arduino.h"
#include "SPI.h"
#include "Wire.h"
#include "float.h"

// Sensor libraries
#include <Adafruit_LSM6DSO32.h>
#include <Adafruit_LSM6DS.h>
#include <Adafruit_DPS310.h>
#include <BMI088.h>
#include <Adafruit_BMP3XX.h>
#include <Adafruit_LIS2MDL.h>
#include <Adafruit_HDC302x.h>

// Flash libraries
#include <Adafruit_SPIFlash.h>
#include "SdFat_Adafruit_Fork.h"

// Others
#include "globals.h"
#include "sensor_detect.h"
#include "sensor_config.h"
#include "pinout.h"

// ISR
volatile bool spiRxFlag;
void INTERBOARD_SPI_ISR();
#include "sensor_read.h"
