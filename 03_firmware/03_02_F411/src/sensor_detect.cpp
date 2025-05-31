#include "main.h"
#include "sensor_detect.h"
#include <SPI.h>
#include <Wire.h>
#include "Adafruit_LSM6DSOX.h"
#include "Adafruit_DPS310.h"
#include "bmi088.h"
#include "Adafruit_BMP3XX.h"
#include "Adafruit_LIS2MDL.h"
#include "Adafruit_HDC302x.h"
#include "Adafruit_MPU6050.h"

// Declare the sensor objects extern in main.cpp and here extern too, or better yet,
// move all these globals into a separate globals file or keep them in main.cpp and extern here
Adafruit_LSM6DSOX lsm6ds;
Adafruit_DPS310 dps310;
bfs::Bmi088Accel accel;
bfs::Bmi088Gyro gyro;
Adafruit_BMP3XX bmp3xx;
Adafruit_LIS2MDL lis2mdl;
Adafruit_HDC302x hdc;
Adafruit_MPU6050 mpu;

String detectedSensor = "None";

void detectSensor()
{
    SPI.begin();
    Wire.begin();

    if (lsm6ds.begin_SPI(PA15))
    {
        detectedSensor = "LSM6DSOX";
    }
    else if (dps310.begin_SPI(PA15))
    {
        detectedSensor = "DPS310";
    }
    else if (bmi.begin(BMI088_ADDRESS_ACCEL, BMI088_ADDRESS_GYRO, &SPI, PA15, PA2))
    {
        detectedSensor = "BMI088";
    }
    else if (bmp3xx.begin_I2C())
    {
        detectedSensor = "BMP3XX";
    }
    else if (lis2mdl.begin_I2C())
    {
        detectedSensor = "LIS2MDL";
    }
    else if (hdc.begin())
    {
        detectedSensor = "HDC302x";
    }
    else if (mpu.begin())
    {
        detectedSensor = "MPU6050";
    }

    Serial.print("Detected sensor: ");
    Serial.println(detectedSensor);
}
