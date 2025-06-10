#include "globals.h"
#include "flash_write.h"

// sensor_read.h

#ifndef SENSOR_READ_H
#define SENSOR_READ_H

// Central function
void readSensor(SensorType sensorType);

// Sensor reading functions
void read_LSM6DS();
void read_DPS310();
void read_BMI088();
void read_BMP390();
void read_LIS2MDL();
void read_HDC302();

#endif // SENSOR_READ_R