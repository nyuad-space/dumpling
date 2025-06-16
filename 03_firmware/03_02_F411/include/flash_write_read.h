#ifndef FLASHWRITE_H
#define FLASHWRITE_H

#include "globals.h"
#include "pinout.h"

// Get sensor-specific information
struct SensorFileInfo
{
    const char *headers;
    const char *regularName;
    const char *circularName;
};

// Initialize flash storage system
bool initFlashWrite(bool clear);

SensorFileInfo getSensorFileInfo(SensorType sensorType);

bool initFilesForSensor(SensorType sensorType);

void closeFiles();

void writeCircular(File &file, uint32_t &writePos, const String &data, SensorType sensorType);

// Write current sensor reading to flash
void writeToFlash(SensorType sensorType, bool circular);

String createDataString(SensorType sensorType, unsigned long timestamp);

// Get file size in bytes
unsigned long getFileSize(const char *filename);

// Get the appropriate filename for a sensor type
const char *getSensorFilename(SensorType sensorType, bool circular);

// Read current flash recordings
bool readFromFlash(SensorType sensorType, uint8_t *buffer, size_t buffer_size, bool circular);

bool clearFlash(SensorType sensorType, bool circular);

#endif // FLASHWRITE_H