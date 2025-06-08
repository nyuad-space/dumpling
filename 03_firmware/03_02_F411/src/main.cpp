#include "main.h"

void setup()
{
    // To be safe?
    delay(1000);

    /* COMMS SETUP */
    Serial.begin(115200);
    delay(5000); // Give time for power stabilization
    Serial.print("Connected to: ");
    Serial.println(detectSensor());

    Serial.println("Initialize communication (SPI or I2C)");
    initSensorComm(detectedSensor);

    Serial.println("Configuring sensor: ");
    configSensor(detectedSensor);

    // Initialize interboard SPI in slave mode
    // The CS pin is required, dummy in slave mode
    INTERBOARD_SPI.begin(INTERBOARD_CS, SPI_SLAVE);
    INTERBOARD_SPI.attachSlaveInterrupt(INTERBOARD_CS, INTERBOARD_SPI_ISR); 
}

void loop()
{    
    if (spiRxFlag) {
        Serial.println("I received something!");
        Serial.print("BTW I'm using sensor");
        Serial.println(detectedSensor);
        spiRxFlag = false;
    }
}

void INTERBOARD_SPI_ISR() {
  spiRxFlag = true;
}
