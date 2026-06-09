#include "comm.h"

uint8_t buildDataRequest(uint8_t* buffer) {
    SPIPacket packet(MASTER_CONTROLLER);
    packet.clear();
    packet.addByte(READING);
    
    packet.serialize(buffer);
    return packet.getTotalSize();
}

uint8_t buildDummyRequest(uint8_t* buffer) {
    SPIPacket packet(MASTER_CONTROLLER);
    packet.clear();
    packet.addByte(DUMMY);
    
    packet.serialize(buffer);
    return packet.getTotalSize();
}