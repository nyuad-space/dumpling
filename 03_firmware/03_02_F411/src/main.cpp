#include "main.h"

void setup()
{
    Serial.begin(115200);
    delay(1000);

    // Detect sensor
    detectSensor();
}

void loop()
{
    Serial.println("Detecting sensor...");
    Serial.print("Connected to: ");
    Serial.println(detectSensor());

    // // Configure sensor
    // Serial.println("Configuring sensor...");
    // configSensor(detectedSensor);
    // Serial.println("Sensor configured.");

    /*
    // If I2C START trigger from F405,
    // read sensor data and write to local flash // create .csv of name of sensor, date, time, proper header
    // print to serial monitor for debug

    // If I2C STOP trigger from F405,
    // notify over I2C that trigger received
    // If SPI request from F405,
    // Send flash data to main flash over SPI
    // When transfer is done, ..

    // c.f. Sensor logging rates: define standard sample interval for all sensors or configurable via compile-time flags.
    */
}
