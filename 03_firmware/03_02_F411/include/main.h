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
#include "flash_write_read.h"
#include "pinout.h"
#include "sensor_read.h"
#include "SPIPacket.h"

#define DEBUG 1

// Indication
Adafruit_NeoPixel neopixel(1, RGB_LED, NEO_GRB + NEO_KHZ800);
uint32_t color_red = neopixel.Color(200, 0, 0);
uint32_t color_amber = neopixel.Color(220, 160, 0);
uint32_t color_green = neopixel.Color(0, 160, 0);
uint32_t color_light_green = neopixel.Color(0, 50, 0);
uint32_t color_blue = neopixel.Color(0, 0, 100);
bool success_flag = false;

// Logging
bool logging_allowed = false;

// SPI Related
SPI_HandleTypeDef *hspi = INTERBOARD_SPI.getHandle();
uint8_t INTERBOARD_TX_BUFFER[MAX_PACKET_SIZE];
uint8_t INTERBOARD_RX_BUFFER[MAX_PACKET_SIZE];
volatile bool INTERBOARD_RCVD = false;

uint8_t counter = 20; // TODO: remove dummy logic
uint8_t master_data_size;
uint8_t master_message_size;
uint8_t coproc_response_size;
bool first_packet_from_master = true;

// Functions
void LOG_TRIGGER_ISR();
void INTERBOARD_SPI_ISR();
void INTERBOARD_SPI_PROCESS_MSG();
void prepareResponse();
void _print_buffer(const char *label, uint8_t *buffer, uint8_t size);
void _blink_red();