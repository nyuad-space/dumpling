#include "main.h"

// Flash object (ref: sensor2flash.ino)
// Adafruit_SPIFlash flash(&flashTransport);
// FatVolume fatfs;
// File32 sensorFile;

Adafruit_SPIDevice spi_lsm6ds = Adafruit_SPIDevice(LSM6DS_CS, 10000, SPI_BITORDER_MSBFIRST, SPI_MODE0, &SENSOR_SPI);
Adafruit_BusIO_Register regID_lsm6ds = Adafruit_BusIO_Register(&spi_lsm6ds, LSM6DS_WHOAMI_ADDR, ADDRBIT8_HIGH_TOREAD);

// === Setup ===
// Assuming that flash has been initialied with SdFat_format.ino
void setup()
{
    Serial.begin(115200);
    delay(500); // Give time for power stabilization
    SENSOR_SPI.begin();

    /*
// Configure sensor
Serial.println("Configuring sensor...");
configSensor(detectedSensor);
Serial.println("Sensor configured.");


// Initialize Flash
Serial.println("Initializing flash memory...");
if (!flash.begin())
{
   Serial.println("Flash init failed!");
   while (1)
       yield();
}
// Mount filesystem
if (!fatfs.begin(&flash))
{
   Serial.println("Filesystem not found. Please run formatter first.");
   while (1)
       yield();
}
Serial.println("Filesystem mounted.");
*/

    // Store sensor ID
    // Respond to I2C from F405 with sensor ID
}

// === Loop ===
void loop()
{

    // Detect and initialize sensor
    // Serial.println("Detecting sensor...");
    // detectSensor();
    // delay(1000);
    Serial.println(regID_lsm6ds.read());

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
