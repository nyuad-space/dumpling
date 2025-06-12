#pragma once
#include "Arduino.h"
#include "SPI.h"
#include "Wire.h"
#include "pinout.h"
#include "Adafruit_NeoPixel.h"
#include "SPIPacket.h"
#include "Adafruit_MPU6050.h"

#include "comm.h"

/*******************************/
/*******************************/
/*******************************/

// Debug
#define DEBUG 1

// Flight ready detection
#define UPRIGHT_ACCEL_THRESH 7.5
#define UPRIGHT_TIME_THRESH_SEC 5000
#define MPU_MOT_DET_DUR 5

// Flight end detection
#define QUIET_TIME_THRESH_SEC 5000
#define LAUNCH_DETECT_ACCEL_THRESH 40

// Data collection
#define PACKET_PREP_TIME 5000
#define POLLING_TIMEOUT_MS 60000

/*******************************/
/*******************************/
/*******************************/

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

/*******************************/
/*******************************/
/*******************************/

// LED
Adafruit_NeoPixel neopixel(1, RGB_LED, NEO_GRB + NEO_KHZ800);

// Comms
SPIClass INTERBOARD_SPI = SPIClass(INTERBOARD_SPI_MOSI, INTERBOARD_SPI_MISO, INTERBOARD_SPI_SCK);
TwoWire MPU_I2C(MPU_SDA, MPU_SCL);

// IMU
Adafruit_MPU6050 mpu;
sensors_event_t a, g, temp;

/*******************************/
/*******************************/
/*******************************/

// Flight states
enum FlightState
{
  FLIGHT_READY_DETECTION,
  FLIGHT_MONITORING,
  DATA_COLLECTION,
  STANDBY
};
FlightState current_state = FLIGHT_READY_DETECTION;
FlightState previous_state = STANDBY;

// LED indication
uint32_t color_red = neopixel.Color(200, 0, 0);
uint32_t color_amber = neopixel.Color(220, 160, 0);
uint32_t color_green = neopixel.Color(0, 160, 0);
uint32_t color_blue = neopixel.Color(0, 0, 100);

unsigned long led_blink_time = 0;
bool led_blink_state = false;

// Flight ready variables
bool rocket_curr_upright = false; 
bool rocket_prev_upright = false;
unsigned long upright_start_time = 0;

// Flight end variables
bool launch_detected = false;
bool first_entry = true;
unsigned long quiet_start_time = 0;

// SPI communication 
uint8_t rx_buffer[MAX_PACKET_SIZE];

uint8_t command_buffer[MAX_PACKET_SIZE];
uint8_t command_packet_size=0;

uint8_t dummy_buffer[MAX_PACKET_SIZE];
uint8_t dummy_packet_size=0;

// Function declarations
void initialize_interboard_spi();
void initialize_mpu();
bool detect_flight_ready();

bool make_data_request(uint8_t);
bool check_end_sequence(uint8_t *);
void _print_buffer(const char*, uint8_t*, uint8_t);