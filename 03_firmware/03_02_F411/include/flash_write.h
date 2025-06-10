#ifndef FLASHWRITE_H
#define FLASHWRITE_H

#include "sensor_read.h"
#include "globals.h"
#include "pinout.h"

// Function declarations

// Initialize flash storage system
bool initFlashWrite();

// Create CSV file headers if files don't exist
void createCSVHeaders(SensorType sensorType);

// Write current sensor reading to flash
void writeSensorData(SensorType sensorType);

// Get file size in bytes
unsigned long getFileSize(const char *filename);

// Get the appropriate filename for a sensor type
const char *getSensorFilename(SensorType sensorType);

#endif // FLASHWRITE_H