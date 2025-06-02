// sensor_detect.h
#ifndef SENSOR_DETECT_H
#define SENSOR_DETECT_H

#include <Arduino.h> // or relevant includes if needed

// Sensor objects
extern Adafruit_LSM6DSOX lsm6ds;
extern Adafruit_DPS310 dps310;
extern Bmi088Accel bmiAccel;
extern Bmi088Gyro bmiGyro;
extern Adafruit_BMP3XX bmp3xx;
extern Adafruit_LIS2MDL lis2mdl;
extern Adafruit_HDC302x hdc;

// Others
extern String detectedSensor; // Make detectedSensor accessible externally

// Declare your detectSensor function
void detectSensor();

#endif // SENSOR_DETECT_H
