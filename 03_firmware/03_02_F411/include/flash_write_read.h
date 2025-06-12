#ifndef FLASHWRITE_H
#define FLASHWRITE_H

#include "globals.h"
#include "pinout.h"

// Function declarations

// Initialize flash storage system
bool initFlashWrite(bool clear);

// Create CSV file headers if files don't exist
void createCSVHeaders(SensorType sensorType);

// Write current sensor reading to flash
void writeToFlash(SensorType sensorType, bool circular);

// Get file size in bytes
unsigned long getFileSize(const char *filename);

// Get the appropriate filename for a sensor type
const char *getSensorFilename(SensorType sensorType, bool circular);

// Read current flash recordings
bool readFromFlash(SensorType sensorType, uint8_t *buffer, size_t buffer_size, bool circular);
size_t read_by_chunk(File &fileHandle, uint8_t *buffer, size_t buffer_size, bool circular);
bool clearFlash(SensorType sensorType);

#endif // FLASHWRITE_H