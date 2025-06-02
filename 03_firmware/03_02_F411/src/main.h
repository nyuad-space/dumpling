#pragma once

#include "Arduino.h"
#include "SPI.h"
#include "Wire.h"
#include "float.h"

// Sensor libraries
#include <Adafruit_LSM6DSO32.h>
#include <Adafruit_DPS310.h>
#include "BMI088.h"
#include <Adafruit_BMP3XX.h>
#include <Adafruit_LIS2MDL.h>
#include <Adafruit_HDC302x.h>

// Flash libraries
#include <Adafruit_SPIFlash.h>
#include <SPIFlash.h>
#include <SdFat_Adafruit_Fork.h>
#include "flash_config.h"

// Others
#include "flash_format.h"
#include "sensor_detect.h"
#include "sensor_types.h"
#include "pinout.h"