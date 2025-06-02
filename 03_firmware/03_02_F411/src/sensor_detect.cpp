#include "main.h"

// Declare the sensor objects
Adafruit_LSM6DSOX lsm6ds;
Adafruit_DPS310 dps310;
Bmi088Accel bmiAccel(SPI, accel_CS);
Bmi088Gyro bmiGyro(SPI, gyro_CS);
Adafruit_BMP3XX bmp3xx;
Adafruit_LIS2MDL lis2mdl;
Adafruit_HDC302x hdc;

SensorType detectedSensor = SENSOR_UNKNOWN;

void detectSensor()
{
    SPI.begin();
    Wire.begin();

    if (lsm6ds.begin_SPI(LSM6DS_CS, LSM6DS_SCK, LSM6DS_MISO, LSM6DS_MOSI, 0, 1000000)) // CS, SCK, MISO, MOSI, sensorID, freq
    {
        detectedSensor = SENSOR_LSM6DSO32;
        while (1)
        {
            delay(10);
        }
    }

    else if (dps310.begin_SPI(DPS310_CS, &SPI))
    {
        detectedSensor = SENSOR_DPS310;
        while (1)
        {
            delay(10);
        }
    }
    else if (bmiAccel.begin() == 1 && bmiGyro.begin() == 1)
    {
        detectedSensor = SENSOR_Bmi088Accel; // SENSOR_Bmi088Gyro
        while (1)
        {
            delay(10);
        }
    }
    else if (bmp3xx.begin_I2C(0x76, &Wire)) // using I2C
    {
        detectedSensor = SENSOR_BMP3xx;
        while (1)
        {
            delay(10);
        }
    }
    else if (lis2mdl.begin(0x1E, &Wire)) // using I2C
    {
        detectedSensor = SENSOR_LIS2MDL;
        while (1)
        {
            delay(10);
        }
    }
    else if (hdc.begin(0x44, &Wire)) // using I2C
    {
        detectedSensor = SENSOR_HDC302x;
        while (1)
        {
            delay(10);
        }
    }
    else
    {
        Serial.print("Unknown sensor");
        return;
    }

    Serial.print("Detected sensor: ");
    Serial.println(detectedSensor);
}
