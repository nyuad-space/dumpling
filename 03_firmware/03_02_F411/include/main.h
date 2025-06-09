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
#include <Adafruit_NeoPixel.h>

// Flash libraries
#include <Adafruit_SPIFlash.h>
#include <SdFat_Adafruit_Fork.h>

// Others
#include "SPIPacket.h"
#include "globals.h"
#include "pinout.h"
#include "sensor_detect.h"
#include "sensor_config.h"
#include "flash_write.h"
#include "pinout.h"
#include "sensor_read.h"
#include "SPIPacket.h"

// LED
Adafruit_NeoPixel neopixel(1, RGB_LED, NEO_GRB + NEO_KHZ800);

// SPI Related
SPI_HandleTypeDef *hspi = INTERBOARD_SPI.getHandle();
void INTERBOARD_SPI_ISR();
void INTERBOARD_SPI_PROCESS_MSG();
uint8_t INTERBOARD_RX_BUFFER[MAX_PACKET_SIZE];
volatile bool INTERBOARD_RCVD = false;
uint8_t data_size;
uint8_t SPI_message_size;
