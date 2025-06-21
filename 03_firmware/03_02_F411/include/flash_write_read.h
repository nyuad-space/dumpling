#ifndef FLASHWRITE_H
#define FLASHWRITE_H

#include "globals.h"
#include "pinout.h"
#include "flash_status.h"

// Get sensor-specific information
struct SensorFileInfo
{
    const char *headers;
    const char *regularName;
    const char *circularName;
};

// Initialize flash storage system
bool initFlashWrite();

SensorFileInfo getSensorFileInfo(SensorType sensorType);

bool initFilesForSensor(SensorType sensorType);

// Write to circular buffer
void writeCircular(const String &data, SensorType sensorType);

// Write current sensor reading to flash
void writeToFlash(SensorType sensorType, bool circular);

// Create data string
String createDataString(SensorType sensorType, unsigned long timestamp);

// Get file size in bytes
unsigned long getFileSize(const char *filename);

#endif // FLASHWRITE_H