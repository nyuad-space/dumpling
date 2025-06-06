#include "main.h"

SPIClass INTERBOARD_SPI(INTERBOARD_SPI_MOSI, INTERBOARD_SPI_MISO, INTERBOARD_SPI_SCK);
Adafruit_NeoPixel rgb_led(1, RGB_LED, NEO_GRB + NEO_KHZ800);
SPIPacket heartbeat_request(MASTER_CONTROLLER); // Global declaration
uint8_t heartbeat_statuses[InterboardSPI_CS::COUNT] = {0}; // Initialize to zero

// Timing variables
const unsigned long HEARTBEAT_INTERVAL = 3000;

void setup()
{
  Serial.begin(9600);
  delay(1000);

  // Chip selects high
  initialize_spi_system();

  // Build heartbeat request packet
  heartbeat_request.addByte(CommType::HEARTBEAT);
}

void loop()
{
  broadcast_heartbeat_request(heartbeat_request, heartbeat_statuses);
  
  // Print all heartbeat statuses
  Serial.print("Heartbeat statuses: ");
  for(int i = 0; i < InterboardSPI_CS::COUNT; i++) {
    Serial.print(heartbeat_statuses[i]);
    Serial.print(" ");
  }
  Serial.println();
  
  delay(HEARTBEAT_INTERVAL);
}

// Functions
void initialize_spi_system()
{
  // Initialize all CS pins as outputs (HIGH = inactive, safe default)
  for (size_t i = 0; i < InterboardSPI_CS::COUNT; i++)
  {
    pinMode(InterboardSPI_CS::PINS_LIST[i], OUTPUT);
    digitalWrite(InterboardSPI_CS::PINS_LIST[i], HIGH);
    Serial.print(InterboardSPI_CS::PINS_LIST[i]);
  }

  // Initialize SPI bus
  INTERBOARD_SPI.begin();
  Serial.println("SPI bus initialized");
}
void broadcast_heartbeat_request(SPIPacket request_packet, uint8_t* statuses) {
  uint8_t tx_buf[MAX_PACKET_SIZE];
  uint8_t rx_buf[MAX_PACKET_SIZE];
  
  // Serialize packet into buffer
  request_packet.serialize(tx_buf);
  uint8_t packet_size = request_packet.getTotalSize();
  
  // Iterate through all co-procesors
  for (size_t i = 0; i < InterboardSPI_CS::COUNT; i++) {

    // Transfer
    digitalWrite(InterboardSPI_CS::PINS_LIST[i], LOW);
    INTERBOARD_SPI.transfer(tx_buf, rx_buf, packet_size);
    digitalWrite(InterboardSPI_CS::PINS_LIST[i], HIGH);
    
    // Get response data
    SPIPacket response;
    if (response.deserialize(rx_buf, packet_size) && response.getDataLength() > 0) {
      statuses[i] = response.getData()[0];  // First data byte is status
    } else {
      statuses[i] = 0x00;  // Offline/no response
    }
  }
}