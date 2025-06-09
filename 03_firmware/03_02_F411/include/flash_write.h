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
unsigned long getFileSize(const char* filename);

// Get the appropriate filename for a sensor type
const char* getSensorFilename(SensorType sensorType);

// External variables from sensor_read.cpp
extern float accel_x_read;
extern float accel_y_read;
extern float accel_z_read;
extern float gyro_x_read;
extern float gyro_y_read;
extern float gyro_z_read;
extern float temp_read;
extern float press_read;
extern float bmp390_alt_read;
extern float mag_x_read;
extern float mag_y_read;
extern float mag_z_read;
extern float hum_read;

#endif // FLASHWRITE_H