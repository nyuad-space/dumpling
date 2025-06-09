#include "main.h"

void setup()
{
    /* COMMS SETUP */
    Serial.begin(115200);
    delay(5000); // Give time for power stabilization

    // Initialize interboard SPI in slave mode
    // The CS pin is required, dummy in slave mode
    INTERBOARD_SPI.begin(INTERBOARD_CS, SPI_SLAVE);
    INTERBOARD_SPI.attachSlaveInterrupt(INTERBOARD_CS, INTERBOARD_SPI_ISR);

    Serial.print("Connected to: ");
    Serial.println(detectSensor());

    Serial.println("Initialize communication (SPI or I2C)");
    initSensorComm(detectedSensor);

    Serial.println("Configuring sensor: ");
    configSensor(detectedSensor);

    // Initialize Flash
    Serial.println("Initializing flash memory...");
    if (!flash_memory.begin()) {
        Serial.println("Flash init failed!");
        while (1) yield();
    }

    uint32_t jedec_id = flash_memory.getJEDECID();
    Serial.print("JEDEC ID: 0x");
    Serial.println(jedec_id, HEX);
    Serial.print("Flash size (usable): ");
    Serial.print(flash_memory.size() / 1024);
    Serial.println(" KB");

    // Mount filesystem
    if (!fatfs.begin(&flash_memory)) {
        Serial.println("Filesystem not found. Please run formatter first.");
        while (1) yield();
    }
    Serial.println("Filesystem mounted.");
    
}

void loop()
{
    if (spiRxFlag)
    {
        Serial.println("I received something!");
        Serial.print("BTW I'm using sensor");
        Serial.println(detectedSensor);
        spiRxFlag = false;
    }
    readSensor(detectedSensor);
}

void INTERBOARD_SPI_ISR()
{
    spiRxFlag = true;
}