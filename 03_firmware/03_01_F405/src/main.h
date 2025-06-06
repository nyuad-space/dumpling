#pragma once
#include "Arduino.h"
#include "SPI.h"
#include "pinout.h"
#include "Adafruit_NeoPixel.h"
#include "SPIPacket.h"

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

// Timing constants
extern const unsigned long HEARTBEAT_INTERVAL;
extern const unsigned long SPI_TEST_INTERVAL;

// Function declarations
void initialize_spi_system();
void broadcast_heartbeat_request(SPIPacket request_packet, uint8_t* statuses);