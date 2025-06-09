#include "main.h"

void setup()
{
  Serial.begin(115200);
  delay(1000);

  // Initialize RGB LED
  rgb_led.begin();

  // Chip selects high
  // Must select chips manually!
  initialize_spi_system();

  // Build heartbeat packet
  dummy_packet_size = buildDummyRequest(dummy_buffer);
  command_packet_size = buildDataRequest(command_buffer);
}

void loop() {
  switch (currentState) {

    case FLIGHT_MONITORING:
      // Flight monitoring indication
      rgb_led.setPixelColor(0, color_green);
      rgb_led.show();
      
      // Implement real flight end logic
      delay(5000);
      currentState = DATA_COLLECTION;
      break;

    case DATA_COLLECTION:
      // Collect data from all coprocessors once
      // make_data_request(COPROCESSOR_1_CS);
      make_data_request(INTERBOARD_SPI_CO2_CS);

      // store_to_sd(data);
      // currentState = STANDBY;
      break;
      
    case STANDBY:
      rgb_led.setPixelColor(0, color_blue);
      rgb_led.show();

      delay(1000);
      break;
  }
}

bool make_data_request(uint8_t CS_PIN) {  
  // Step 0: Amber flash
  rgb_led.setPixelColor(0, color_amber);
  rgb_led.show();
  delay(500);
  rgb_led.clear();
  rgb_led.show();

  // Debug: Print command_buffer contents
  Serial.print("Packet size: ");
  Serial.print(command_packet_size);
  Serial.print(" | TX Buffer: ");
  for(int i = 0; i < command_packet_size; i++) {
    Serial.print("0x");
    if(command_buffer[i] < 16) Serial.print("0");
    Serial.print(command_buffer[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  // Step 1: Send request data packet
  digitalWrite(CS_PIN, LOW);
  INTERBOARD_SPI.transfer(command_buffer, rx_buffer, command_packet_size, SPI_LAST);
  digitalWrite(CS_PIN, HIGH);

  // Step 2: Wait for coprocessor, red flash
  delay(1000);
  rgb_led.setPixelColor(0, color_red);
  rgb_led.show();
  delay(500);
  rgb_led.clear();
  rgb_led.show();

  // Step 3: Poll with dummy bytes until we get the end sequence or timeout
  unsigned long start_time = millis();

  while (millis() - start_time < POLLING_TIMEOUT_MS) {
    // Debug: Print dummy_buffer contents
    Serial.print("Packet size: ");
    Serial.print(dummy_packet_size);
    Serial.print(" | TX Buffer: ");
    for(int i = 0; i < dummy_packet_size; i++) {
      Serial.print("0x");
      if(dummy_buffer[i] < 16) Serial.print("0");
      Serial.print(dummy_buffer[i], HEX);
      Serial.print(" ");
    }
    Serial.println();

    digitalWrite(CS_PIN, LOW);
    INTERBOARD_SPI.transfer(dummy_buffer, rx_buffer, dummy_packet_size, SPI_LAST);
    digitalWrite(CS_PIN, HIGH);
    
    // Check if we got the end sequence
    if (check_end_sequence(rx_buffer)) {
      return true;  // Success - data complete
    }
    
    // Small delay between polls to not spam the coprocessor
    delay(500);
}
  
  return true;  // Timeout - failed to get end sequence
}

bool check_end_sequence(uint8_t* buffer) {
  // Example: look for 0xBEEF pattern
  // if (buffer[0] == 0xDE && buffer[1] == 0xAD) return true;
  return false;  // Placeholder
}

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