#ifndef FLASHWRITE_H
#define FLASHWRITE_H

#include "globals.h"
#include "pinout.h"

// Function declarations

// Initialize flash storage system
bool initFlashWrite();

// Create CSV file headers if files don't exist
void createCSVHeaders(SensorType sensorType);

// Write current sensor reading to flash
void writeToFlash(SensorType sensorType);

// Get file size in bytes
unsigned long getFileSize(const char *filename);

// Get the appropriate filename for a sensor type
const char *getSensorFilename(SensorType sensorType);

// Read current flash recordings
void readFromFlash(SensorType sensorType, size_t request_size, uint8_t *buffer, size_t buffer_size);
uint8_t *read_by_chunk(File &fileHandle, size_t request_size, uint8_t *buffer, size_t buffer_size);

#endif // FLASHWRITE_H