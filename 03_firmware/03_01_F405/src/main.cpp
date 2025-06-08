#include "main.h"

SPIClass INTERBOARD_SPI(INTERBOARD_SPI_MOSI, INTERBOARD_SPI_MISO, INTERBOARD_SPI_SCK);
Adafruit_NeoPixel rgb_led(1, RGB_LED, NEO_GRB + NEO_KHZ800);
SPIPacket heartbeat_request(MASTER_CONTROLLER); // Global declaration
uint8_t heartbeat_statuses[InterboardSPI_CS::COUNT] = {0}; // Initialize to zero

// Timing variables
const unsigned long HEARTBEAT_INTERVAL = 5000;

void setup()
{
  Serial.begin(115200);
  delay(1000);

  // Initialize RGB LED
  rgb_led.begin();

  // Chip selects high
  initialize_spi_system();

  // Build heartbeat request packet
  heartbeat_request.addByte(CommType::HEARTBEAT);
}

void loop()
{
  // debug delay
  delay(HEARTBEAT_INTERVAL);
  
  // CS INTERBOARD_SPI_CO2_CS
  digitalWrite(INTERBOARD_SPI_CO2_CS, LOW);
  INTERBOARD_SPI.transfer(0xAA); // Send some random data
  digitalWrite(INTERBOARD_SPI_CO2_CS, HIGH);
  
  // Flash neopixel rgb for a second
  rgb_led.setPixelColor(0, rgb_led.Color(255, 0, 0));
  rgb_led.show();
  delay(1000);
  rgb_led.clear();
  rgb_led.show();
}

// Functions
void initialize_spi_system()
{
  // Initialize all CS pins as outputs (HIGH = inactive, safe default)
  for (size_t i = 0; i < InterboardSPI_CS::COUNT; i++)
  {
    pinMode(InterboardSPI_CS::PINS_LIST[i], OUTPUT);
    digitalWrite(InterboardSPI_CS::PINS_LIST[i], HIGH);
    Serial.print(InterboardSPI_CS::PINS_LIST[i]);
  }

  // Initialize SPI bus
  INTERBOARD_SPI.begin();
  Serial.println("SPI bus initialized");
}