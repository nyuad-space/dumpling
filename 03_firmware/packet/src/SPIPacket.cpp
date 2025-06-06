#include "SPIPacket.h"

// Constructor
SPIPacket::SPIPacket(SenderID sender) 
    : sender_id(sender), data_length(0), reserved(0), checksum(0) {
    memset(data, 0, MAX_PACKET_DATA_SIZE);
}

// Add data to packet
bool SPIPacket::addData(const uint8_t* bytes, uint8_t length) {
    if (data_length + length > MAX_PACKET_DATA_SIZE) {
        return false; // Not enough space
    }
    
    memcpy(&data[data_length], bytes, length);
    data_length += length;
    return true;
}

// Add single byte
bool SPIPacket::addByte(uint8_t byte) {
    return addData(&byte, 1);
}

// Add string (without null terminator)
bool SPIPacket::addString(const char* str) {
    uint8_t len = strlen(str);
    return addData((const uint8_t*)str, len);
}

// Clear packet data
void SPIPacket::clear() {
    data_length = 0;
    memset(data, 0, MAX_PACKET_DATA_SIZE);
    checksum = 0;
}

// Serialize packet to byte array for transmission
void SPIPacket::serialize(uint8_t* buffer) const {
    buffer[0] = sender_id;
    buffer[1] = data_length;
    buffer[2] = reserved;
    
    // Copy data
    if (data_length > 0) {
        memcpy(&buffer[3], data, data_length);
    }
    
    // Add checksum (placeholder for now)
    buffer[3 + data_length] = calculateChecksum();
}

// Deserialize from received byte array
bool SPIPacket::deserialize(const uint8_t* buffer, uint8_t buffer_size) {
    if (buffer_size < PACKET_HEADER_SIZE + PACKET_CHECKSUM_SIZE) {
        return false; // Too small
    }
    
    sender_id = (SenderID)buffer[0];
    data_length = buffer[1];
    reserved = buffer[2];
    
    if (data_length > MAX_PACKET_DATA_SIZE) {
        return false; // Invalid length
    }
    
    if (buffer_size < getTotalSize()) {
        return false; // Buffer too small for claimed data
    }
    
    // Copy data
    if (data_length > 0) {
        memcpy(data, &buffer[3], data_length);
    }
    
    // Get checksum
    checksum = buffer[3 + data_length];
    
    // TODO: Verify checksum when implemented
    return true;
}

// Debug print
void SPIPacket::printPacket() const {
    Serial.print("Packet: Sender=0x");
    Serial.print(sender_id, HEX);
    Serial.print(", Length=");
    Serial.print(data_length);
    Serial.print(", Data=[");
    
    for (uint8_t i = 0; i < data_length; i++) {
        Serial.print("0x");
        Serial.print(data[i], HEX);
        if (i < data_length - 1) Serial.print(" ");
    }
    
    Serial.print("], Checksum=0x");
    Serial.println(checksum, HEX);
}

// Dummy checksum calculation
uint8_t SPIPacket::calculateChecksum() const {
    // TODO: Implement proper checksum (CRC8, XOR, etc.)
    // For now, return dummy value
    return 0xCC;  // Dummy checksum
    
    // Example XOR checksum when ready:
    // uint8_t sum = sender_id ^ data_length ^ reserved;
    // for (uint8_t i = 0; i < data_length; i++) {
    //     sum ^= data[i];
    // }
    // return sum;
}