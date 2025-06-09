/**
 * @file SPIPacket.h
 * @brief SPI Communication Packet Format and Management
 * 
 * =============================================================================
 * SPI PACKET FORMAT SPECIFICATION
 * =============================================================================
 * 
 * This class implements a structured packet format for reliable SPI communication
 * between devices. Each packet contains a fixed header, variable-length data 
 * payload, and checksum for basic error detection.
 * 
 * PACKET STRUCTURE:
 * ┌─────────────┬─────────────┬─────────────┬─────────────────┬─────────────┐
 * │  Sender ID  │ Data Length │  Reserved   │   Data Payload  │  Checksum   │
 * │   (1 byte)  │  (1 byte)   │  (1 byte)   │ (0-N bytes)     │  (1 byte)   │
 * └─────────────┴─────────────┴─────────────┴─────────────────┴─────────────┘
 *  Byte 0        Byte 1        Byte 2       Bytes 3..3+N-1    Byte 3+N
 * 
 * FIELD DESCRIPTIONS:
 * 
 * • Sender ID (1 byte):
 *   - Identifies the originating device/module sending the packet
 *   - Type: SenderID enum (see SenderID definition)
 *   - Used for packet routing and source identification
 * 
 * • Data Length (1 byte):
 *   - Number of bytes in the data payload (0-255)
 *   - Does NOT include header or checksum bytes
 *   - Maximum value limited by MAX_PACKET_DATA_SIZE constant
 * 
 * • Reserved (1 byte):
 *   - Currently unused, set to 0x00
 *   - Reserved for future protocol extensions (e.g., packet flags, sequence numbers)
 *   - Should be initialized to 0 and ignored on reception
 * 
 * • Data Payload (0-N bytes):
 *   - Variable-length application data
 *   - Can contain any binary data (strings, sensor readings, commands, etc.)
 *   - Length specified by Data Length field
 *   - Maximum size: MAX_PACKET_DATA_SIZE bytes
 * 
 * • Checksum (1 byte):
 *   - Error detection mechanism (currently placeholder implementation)
 *   - Calculated over entire packet (header + data)
 *   - TODO: Implement proper checksum algorithm (CRC8, XOR, etc.)
 * 
 * TOTAL PACKET SIZE:
 * Minimum: 4 bytes (header + checksum, no data)
 * Maximum: 4 + MAX_PACKET_DATA_SIZE bytes
 * 
 * USAGE EXAMPLES:
 * 
 * // Creating and sending a packet
 * SPIPacket packet(SENSOR_MODULE);
 * packet.addString("TEMP:25.3");
 * uint8_t buffer[64];
 * packet.serialize(buffer);
 * // Send buffer via SPI...
 * 
 * // Receiving and parsing a packet
 * SPIPacket received_packet;
 * if (received_packet.deserialize(rx_buffer, rx_size)) {
 *     received_packet.printPacket();  // Debug output
 *     // Process packet data...
 * }
 * =============================================================================
 */

#pragma once
#include "Arduino.h"

// Packet configuration
#define MAX_PACKET_DATA_SIZE 32    // Adjust based on your needs (8-64 bytes typical)
#define PACKET_HEADER_SIZE 3       // sender_id + length + reserved
#define PACKET_CHECKSUM_SIZE 1     // placeholder for future checksum
#define MAX_PACKET_SIZE (PACKET_HEADER_SIZE + MAX_PACKET_DATA_SIZE + PACKET_CHECKSUM_SIZE)

// Sender IDs - adjust for your system
enum SenderID : uint8_t {
    MASTER_CONTROLLER = 0x00,
    SLAVE_CO1 = 0x01,
    SLAVE_CO2 = 0x02, 
    SLAVE_CO3 = 0x03,
    SLAVE_CO4 = 0x04,
    SLAVE_CO5 = 0x05,
    SLAVE_CO6 = 0x06,
    BROADCAST = 0xFF
};

// Communication types enum
enum CommType : uint8_t {
    HEARTBEAT = 0x01,
    READING = 0x02, 
    DUMMY = 0x03,
};

// Status
enum Status : uint8_t {
    STATUS_OK = 0x01
};

// Packet class
class SPIPacket {
public:
    // Constructor
    SPIPacket(SenderID sender = MASTER_CONTROLLER);
    
    // Data manipulation methods
    bool addData(const uint8_t* bytes, uint8_t length);
    bool addByte(uint8_t byte);
    bool addString(const char* str);
    void clear();
    
    // Serialization methods
    void serialize(uint8_t* buffer) const;
    bool deserialize(const uint8_t* buffer, uint8_t buffer_size);
    
    // Getters (inline)
    SenderID getSender() const { return sender_id; }
    uint8_t getDataLength() const { return data_length; }
    const uint8_t* getData() const { return data; }
    uint8_t getChecksum() const { return checksum; }
    uint8_t getTotalSize() const { return PACKET_HEADER_SIZE + data_length + PACKET_CHECKSUM_SIZE; }
    
    // Debug method
    void printPacket() const;

private:
    SenderID sender_id;
    uint8_t data_length;
    uint8_t reserved;        // For future expansion
    uint8_t data[MAX_PACKET_DATA_SIZE];
    uint8_t checksum;
    
    // Private helper method
    uint8_t calculateChecksum() const;
};