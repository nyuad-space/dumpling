#include "main.h"

SPIClass INTERBOARD_SPI(INTERBOARD_MOSI, INTERBOARD_MISO, INTERBOARD_SCK);
SPIClass FLASH_SPI(FLASH_MOSI, FLASH_MISO, FLASH_SCK);
SPIClass SENSOR_SPI(SENSOR_MOSI, SENSOR_MISO, SENSOR_SCK);

// Adafruit_NeoPixel rgb_led(1, RGB_LED, NEO_GRB + NEO_KHZ800);

// // Flash object (ref: sensor2flash.ino)
// Adafruit_SPIFlash flash(&flashTransport);
// FatVolume fatfs;
// File32 sensorFile;

bool knowSensor = false;
uint8_t heartbeat_status = 0x00;

// Timing variables
const unsigned long HEARTBEAT_INTERVAL = 3000;

// === Setup ===
// Assuming that flash has been initialied with SdFat_format.ino
void setup()
{
    Serial.begin(115200);
    delay(1000); // Give time for power stabilization
    // initSensorPins(); // Initialize CS pins

    // Initialize SPI buses
    // Interboard SPI (slave mode)
    INTERBOARD_SPI.begin();
    pinMode(INTERBOARD_CS, INPUT);

    // Flash SPI (master mode)
    FLASH_SPI.begin();
    pinMode(FLASH_CS, OUTPUT);
    digitalWrite(FLASH_CS, HIGH);

    // Sensor SPI (master mode)
    SENSOR_SPI.begin();
    pinMode(SENSOR_CS, OUTPUT);
    digitalWrite(SENSOR_CS, HIGH);

    /*
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
    if (knowSensor == false)
    {
        Serial.println("Detecting sensor...");
        Serial.print("Connected to: ");
        Serial.println(detectSensor());
        knowSensor = true;
    }

    // Configure sensor
    Serial.println("Configuring sensor...");
    configSensor(detectedSensor);
    Serial.println("Sensor configured.");

    // Respond heartbeat
    create_heartbeat_response(heartbeat_status);
    Serial.print("Heartbeat status: ");
    Serial.println(heartbeat_status);
    delay(HEARTBEAT_INTERVAL);

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

// Functions
void create_heartbeat_response(uint8_t heartbeat_status)
{
    SPIPacket response_packet;
    uint8_t tx_buf[MAX_PACKET_SIZE];
    uint8_t rx_buf[MAX_PACKET_SIZE];

    // Send status as response
    response_packet.addByte(heartbeat_status);
    // Serialize packet into buffer
    response_packet.serialize(tx_buf);
    uint8_t packet_size = response_packet.getTotalSize();
}