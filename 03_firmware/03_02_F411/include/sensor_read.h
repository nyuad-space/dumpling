#include "globals.h"

// sensor_read.h

#ifndef SENSOR_READ_H
#define SENSOR_READ_H

void readSensor(SensorType sensorType);

// read data
// LSM6DS
extern float lsm6ds_accel_x_read;
extern float lsm6ds_accel_y_read;
extern float lsm6ds_accel_z_read;
extern float lsm6ds_gyro_x_read;
extern float lsm6ds_gyro_y_read;
extern float lsm6ds_gyro_z_read;
extern float lsm6ds_temp_read;
// DPS310
extern float dps310_temp_read;
extern float dps310_press_read;
// BMI088
extern float bmi088_accel_x_read;
extern float bmi088_accel_y_read;
extern float bmi088_accel_z_read;
extern float bmi088_gyro_x_read;
extern float bmi088_gyro_y_read;
extern float bmi088_gyro_z_read;
extern float bmi088_temp_read;
// BMP390
extern float bmp390_temp_read;
extern float bmp390_press_read;
extern float bmp390_alt_read;
// LIS2MDL
extern float lis2mdl_mag_x_read;
extern float lis2mdl_mag_y_read;
extern float lis2mdl_mag_z_read;
// HDC302
extern float hdc302_hum_read;
extern float hdc302_temp_read;

#endif // SENSOR_READ_R