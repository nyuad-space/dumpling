#pragma once
#include "Arduino.h"
#include "SPI.h"
#include "pinout.h"
#include "Adafruit_NeoPixel.h"
#include "SPIPacket.h"
#include "comm.h"

extern SPIClass INTERBOARD_SPI;
extern Adafruit_NeoPixel rgb_led;

// Interboard SPI chip selects
namespace InterboardSPI_CS
{
    constexpr uint32_t PINS_LIST[] = {INTERBOARD_SPI_CO1_CS,
                                      INTERBOARD_SPI_CO2_CS,
                                      INTERBOARD_SPI_CO3_CS,
                                      INTERBOARD_SPI_CO4_CS,
                                      INTERBOARD_SPI_CO5_CS,
                                      INTERBOARD_SPI_CO6_CS};

    constexpr size_t COUNT = sizeof(PINS_LIST) / sizeof(PINS_LIST[0]);
}

//LED
Adafruit_NeoPixel rgb_led(1, RGB_LED, NEO_GRB + NEO_KHZ800);
enum FlightState {
  FLIGHT_MONITORING,
  DATA_COLLECTION, 
  STANDBY
};
uint32_t color_red = rgb_led.Color(255, 0, 0);
uint32_t color_amber = rgb_led.Color(255, 191, 0);
uint32_t color_green = rgb_led.Color(0, 191, 0);
uint32_t color_blue = rgb_led.Color(0, 0, 100);
FlightState currentState = FLIGHT_MONITORING;

SPIClass INTERBOARD_SPI(INTERBOARD_SPI_MOSI, INTERBOARD_SPI_MISO, INTERBOARD_SPI_SCK);
uint8_t command_buffer[MAX_PACKET_SIZE];
uint8_t command_packet_size;

uint8_t dummy_buffer[MAX_PACKET_SIZE];
uint8_t dummy_packet_size;

static bool command_sent = false;
static unsigned long start_time = 0;
uint8_t rx_buffer[MAX_PACKET_SIZE];

// Timing variables
const unsigned long POLLING_TIMEOUT_MS = 5000;  // 5 second timeout
// Function declarations
void initialize_spi_system();
bool make_data_request(uint8_t);
bool check_end_sequence(uint8_t*);