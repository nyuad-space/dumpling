#include "main.h"

// Flash object (ref: sensor2flash.ino)
Adafruit_SPIFlash flash(&flashTransport);
FatVolume fatfs;
File32 sensorFile;

// === Setup ===
// Assuming that flash has been initialied with SdFat_format.ino
void setup()
{
    Serial.begin(115200);
    delay(500); // Give time for power stabilization

    // Open file system on the flash (ref: SdFat_ReadWrite.ino)
    if (!fatfs.begin(&flash))
    {
        Serial.println("Error: file system not existing. Run SdFat_format to create.");
        while (1)
        {
            yield();
            delay(1);
        }
    }

    detectSensor();               // Detect and initialize plugged in sensor
    configSensor(detectedSensor); // Configure corresponding sensor

    // Store sensor ID
    // Respond to I2C from F405 with sensor ID
}

// === Loop ===
void loop()
{
    switch (sensorType)
    {
        case 
    }
    // If I2C START trigger from F405,
    // read sensor data and write to local flash // create .csv of name of sensor, date, time, proper header
    // print to serial monitor for debug

    // If I2C STOP trigger from F405,
    // notify over I2C that trigger received
    // If SPI request from F405,
    // Send flash data to main flash over SPI
    // When transfer is done, ..

    // c.f. Sensor logging rates: define standard sample interval for all sensors or configurable via compile-time flags.
}
