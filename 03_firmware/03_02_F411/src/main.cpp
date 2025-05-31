#include "main.h"
#include "flash_format.h"
#include "sensor_detect.h"

// Flash object
Adafruit_SPIFlash flash(&flashTransport);
FatVolume fatfs;

// === Setup ===
void setup()
{
    Serial.begin(115200);
    delay(500); // Give time for power stabilization

    detectSensor();            // Detect sensors among sensor_detect.cpp
    formatFlash(flash, fatfs); // Initialize and format local flash using flash_format.cpp

    // Store sensor ID
    // Respond to I2C from F405 with sensor ID
}

// === Loop ===
void loop()
{
    // If I2C START trigger from F405,
    // read sensor data and write to local flash
    // print to serial monitor for debug

    // If I2C STOP trigger from F405,
    // notify over I2C that trigger received
    // If SPI request from F405,
    // Send flash data to main flash over SPI
    // When transfer is done, ..

    // c.f. Sensor logging rates: define standard sample interval for all sensors or configurable via compile-time flags.
}
