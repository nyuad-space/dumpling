#include "main.h"

void setup()
{
#if DEBUG
    Serial.begin(115200);
#endif

    // Setup logging interrupts pins
    pinMode(LOG_TRIGGER_GPIO, INPUT);
    attachInterrupt(digitalPinToInterrupt(LOG_TRIGGER_GPIO), LOG_TRIGGER_ISR, CHANGE);

    // Clear LED
    neopixel.begin();
    neopixel.clear();
    neopixel.show();

    // Setup SPI communication with main processor
    INTERBOARD_SPI.begin(INTERBOARD_CS, SPI_SLAVE);
    INTERBOARD_SPI.attachSlaveInterrupt(INTERBOARD_CS, INTERBOARD_SPI_ISR);

    // Discover and configure sensor
#if DEBUG
    Serial.print("Connected to: ");
    Serial.println(detectSensor());
#else
    detectSensor();
#endif

    // Catch bad initialization
    success_flag = initSensorComm(detectedSensor);
    configSensor(detectedSensor);

#if DEBUG
    Serial.println("Sensor configured.");
#endif

    // Setup flash
    success_flag= flash_memory.begin();
    success_flag = initFlashWrite();

#if DEBUG
    uint32_t jedec_id = flash_memory.getJEDECID();
    Serial.print("JEDEC ID: 0x");
    Serial.println(jedec_id, HEX);
    Serial.print("Flash size (usable): ");
    Serial.print(flash_memory.size() / 1024);
    Serial.println(" KB");
    Serial.println(getSensorFilename(detectedSensor));
    Serial.print("File size: ");
    getFileSize(getSensorFilename(detectedSensor));
    Serial.print("\n");
#endif

    if (!success_flag || detectedSensor == SENSOR_UNKNOWN)
    {
        Serial.println("Sensor init failed!");
        while (1)
        {
            _blink_red();
        }
    }

}

void loop()
{
    if (INTERBOARD_RCVD)
    {
        INTERBOARD_SPI_PROCESS_MSG();
    }
}

void LOG_TRIGGER_ISR()
{
    // Read current pin state to determine edge direction
    int pinState = digitalRead(LOG_TRIGGER_GPIO);

    if (pinState == LOW)
    {
        // Falling edge detected, start logging
#if DEBUG
        Serial.println("Logging enabled.");
#endif
        logging_allowed = true;
        neopixel.setPixelColor(0, color_green);
        neopixel.show();
    }
    else
    {
        // Rising edge detected, stop logging
#if DEBUG
        Serial.println("Logging disabled.");
#endif
        logging_allowed = false;
        neopixel.setPixelColor(0, color_amber);
        neopixel.show();
    }
}

void prepareResponse()
{
    SPIPacket response(SLAVE_CO1);
    response.addByte(STATUS_OK);
    response.addString("DATA");
    response.serialize(INTERBOARD_TX_BUFFER);
    coproc_response_size = response.getTotalSize();
}

void INTERBOARD_SPI_PROCESS_MSG()
{
    if (first_packet_from_master)
    {
        // Receive real data, send dummy response
        HAL_SPI_Receive(hspi, INTERBOARD_RX_BUFFER, MAX_PACKET_SIZE, 100);

        master_data_size = INTERBOARD_RX_BUFFER[1];
        master_message_size = PACKET_HEADER_SIZE + master_data_size + PACKET_CHECKSUM_SIZE;

        SPIPacket packet;
        packet.deserialize(INTERBOARD_RX_BUFFER, master_message_size);

#if DEBUG
        packet.printPacket();
#endif

        prepareResponse();
        first_packet_from_master = false;
    }
    else
    {
        // Send real response, receive dummy
        uint8_t response_msg[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE};
        uint8_t end_msg[] = {0xBE, 0xEF, 0x00, 0x00, 0x00};
        uint8_t dummy_rx[MAX_PACKET_SIZE];
        counter--;
        if (counter)
        {
            memcpy(INTERBOARD_TX_BUFFER, response_msg, 5);
            HAL_SPI_TransmitReceive(hspi, response_msg, dummy_rx, 5, 100);
        }
        else
        {
            memcpy(INTERBOARD_TX_BUFFER, end_msg, 5);
            HAL_SPI_TransmitReceive(hspi, end_msg, dummy_rx, 5, 100);
            counter = 5;
            first_packet_from_master = true;

            // Set blue to indicate end of transmission
            neopixel.setPixelColor(0, color_blue);
            neopixel.show();
        }
#if DEBUG
        _print_buffer("TX", INTERBOARD_TX_BUFFER, 5);
        _print_buffer("RX", dummy_rx, MAX_PACKET_SIZE);
#endif
    }

    INTERBOARD_RCVD = false;
}

void INTERBOARD_SPI_ISR()
{
    INTERBOARD_RCVD = true;
}

void _print_buffer(const char *label, uint8_t *buffer, uint8_t size)
{
    Serial.print("Packet size: ");
    Serial.print(size);
    Serial.print(" | ");
    Serial.print(label);
    Serial.print(" Buffer: ");
    for (int i = 0; i < size; i++)
    {
        Serial.print("0x");
        if (buffer[i] < 16)
            Serial.print("0");
        Serial.print(buffer[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}

void _blink_red()
{
    static unsigned long lastBlinkTime = 0;
    static bool ledState = false;
    const unsigned long blinkInterval = 500; // 500ms on/off cycle

    unsigned long currentTime = millis();

    if (currentTime - lastBlinkTime >= blinkInterval)
    {
        lastBlinkTime = currentTime;
        ledState = !ledState;

        if (ledState)
        {
            neopixel.setPixelColor(0, color_red);
        }
        else
        {
            neopixel.clear();
        }
        neopixel.show();
    }
}