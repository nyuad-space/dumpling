#include "main.h"

void setup()
{
    /* COMMS SETUP */
    Serial.begin(115200);
    // delay(5000); // Give time for power stabilization

    // Initialize Interboard comms
    INTERBOARD_SPI.begin(INTERBOARD_CS, SPI_SLAVE);
    INTERBOARD_SPI.attachSlaveInterrupt(INTERBOARD_CS, INTERBOARD_SPI_ISR);

    // Initialize RGB LED
    neopixel.begin();
    neopixel.clear();

    // Sensor setup
    Serial.print("Connected to: ");
    Serial.println(detectSensor());
    initSensorComm(detectedSensor);

    Serial.println("Configuring sensor... ");
    configSensor(detectedSensor);

    // Initialize Flash
    Serial.println("Initializing flash memory...");
    if (!flash_memory.begin())
    {
        Serial.println("Flash init failed!");
        while (1)
            yield();
    }

    uint32_t jedec_id = flash_memory.getJEDECID();
    if (debug)
    {
        Serial.print("JEDEC ID: 0x");
        Serial.println(jedec_id, HEX);
        Serial.print("Flash size (usable): ");
        Serial.print(flash_memory.size() / 1024);
        Serial.println(" KB");
    }
    initFlashWrite();


    if (debug)
    {
        Serial.println(getSensorFilename(detectedSensor));
        Serial.print("File size: ");
        getFileSize(getSensorFilename(detectedSensor));
        Serial.print("\n");
    }
    uint8_t buffer[500];
    readFromFlash(detectedSensor, 10, buffer, 500);
    readFromFlash(detectedSensor, 10, buffer, 500);
    readFromFlash(detectedSensor, 10, buffer, 500);




}

void loop()
{
    if(INTERBOARD_RCVD)
    {
        INTERBOARD_SPI_PROCESS_MSG();
    }
}

void INTERBOARD_SPI_PROCESS_MSG()
{
    // Read maximum possible bytes
    HAL_SPI_Receive(hspi, INTERBOARD_RX_BUFFER, MAX_PACKET_SIZE, 100);

    // Calculate actual message length
    data_size = INTERBOARD_RX_BUFFER[1];    
    SPI_message_size = PACKET_HEADER_SIZE + data_size + PACKET_CHECKSUM_SIZE;
    
    // Validate the calculated length
    if(SPI_message_size <= MAX_PACKET_SIZE && SPI_message_size >= (PACKET_HEADER_SIZE + PACKET_CHECKSUM_SIZE))
    {
        Serial.print("Message length: ");
        Serial.print(SPI_message_size);
        Serial.print(" bytes (data size: ");
        Serial.print(data_size);
        Serial.println(")");
        
        // Print only the valid bytes
        for (int i = 0; i < SPI_message_size; i++) {
            Serial.print("0x");
            Serial.print(INTERBOARD_RX_BUFFER[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
    }
    INTERBOARD_RCVD = false;
}

void INTERBOARD_SPI_ISR()
{
    INTERBOARD_RCVD = true;
}