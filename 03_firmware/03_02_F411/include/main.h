#pragma once

#include "Arduino.h"
#include "SPI.h"
#include "Wire.h"
#include "float.h"

// Flash libraries
#include <Adafruit_SPIFlash.h>
#include <SPIFlash.h>
#include <SdFat_Adafruit_Fork.h>
#include "flash_config.h"
#include "flash_format.h"

// Sensor communication libraries
#include "globals.h"
#include "pinout.h"
#include "sensor_detect.h"
#include "sensor_config.h"
#include "sensor2flash.h"

// Others
#include "Adafruit_NeoPixel.h"
#include "SPIPacket.h"