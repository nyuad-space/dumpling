#pragma once
#include "Arduino.h"

// Packet configuration
#define MAX_PACKET_DATA_SIZE 32 // Adjust based on your needs (8-64 bytes typical)
#define PACKET_HEADER_SIZE 3    // sender_id + length + reserved
#define PACKET_CHECKSUM_SIZE 1  // placeholder for future checksum
#define MAX_PACKET_SIZE (PACKET_HEADER_SIZE + MAX_PACKET_DATA_SIZE + PACKET_CHECKSUM_SIZE)

// Sender IDs - adjust for your system
enum SenderID : uint8_t
{
    MASTER_CONTROLLER = 0x00,
    LOCAL_CONTROLLER = 0x01
    // SLAVE_CO1 = 0x01,
    // SLAVE_CO2 = 0x02,
    // SLAVE_CO3 = 0x03,
    // SLAVE_CO4 = 0x04,
    // SLAVE_CO5 = 0x05,
    // SLAVE_CO6 = 0x06,
    BROADCAST = 0xFF
};

// Communication types enum
enum CommType : uint8_t
{
    HEARTBEAT = 0x01,
    READING = 0x02
};

// Status
enum Status : uint8_t
{
    STATUS_OK = 0x01
};

// Packet class
class SPIPacket
{
public:
    // Constructor
    SPIPacket(SenderID sender = MASTER_CONTROLLER);

    // Data manipulation methods
    bool addData(const uint8_t *bytes, uint8_t length);
    bool addByte(uint8_t byte);
    bool addString(const char *str);
    void clear();

    // Serialization methods
    void serialize(uint8_t *buffer) const;
    bool deserialize(const uint8_t *buffer, uint8_t buffer_size);

    // Getters (inline)
    SenderID getSender() const { return sender_id; }
    uint8_t getDataLength() const { return data_length; }
    const uint8_t *getData() const { return data; }
    uint8_t getChecksum() const { return checksum; }
    uint8_t getTotalSize() const { return PACKET_HEADER_SIZE + data_length + PACKET_CHECKSUM_SIZE; }

    // Debug method
    void printPacket() const;

private:
    SenderID sender_id;
    uint8_t data_length;
    uint8_t reserved; // For future expansion
    uint8_t data[MAX_PACKET_DATA_SIZE];
    uint8_t checksum;

    // Private helper method
    uint8_t calculateChecksum() const;
};