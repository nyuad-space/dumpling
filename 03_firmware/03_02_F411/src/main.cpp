#include "main.h"

void setup()
{
    Serial.begin(115200);
    delay(5000); // Give time for power stabilization
    Serial.print("Connected to: ");
    Serial.println(detectSensor());

    Serial.println("Initialize communication (SPI or I2C)");
    initSensorComm(detectedSensor);

    Serial.println("Configuring sensor: ");
    configSensor(detectedSensor);
}

void loop()
{
}
