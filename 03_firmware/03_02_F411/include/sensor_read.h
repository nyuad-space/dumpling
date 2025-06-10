#include "globals.h"

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

// Sensor readings
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

#endif // SENSOR_READ_R