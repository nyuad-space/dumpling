#include "globals.h"
#include "sensor_read.h"
#include "pinout.h"

#define SEALEVELPRESSURE_HPA (1013.25)

// Time buffer
unsigned long previousMillis = 0; // store last updated time
const long interval = 1000;       // interval at which to delay (milliseconds)

// Sensor readings
float accel_x_read;
float accel_y_read;
float accel_z_read;

float gyro_x_read;
float gyro_y_read;
float gyro_z_read;

float temp_read;
float press_read;

float bmp390_alt_read;

float mag_x_read;
float mag_y_read;
float mag_z_read;

float hum_read;

// dispatcher function
void readSensor(SensorType sensorType)

{
    unsigned long currentMillis = millis();

    // Give buffer time between new sensor reading
    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis;

        switch (sensorType)
        {
        case SENSOR_LSM6DS_ACCEL_GYRO:
        { // true if new data available to read
            if (!lsm6ds_accel_gyro.accelerationAvailable() || !lsm6ds_accel_gyro.gyroscopeAvailable())
            {
                return;
            }

            // Read sensor data (Accel: m/s^2, Gyro: rad/s) to three decimals
            float x, y, z, temp;
            sensors_event_t temp_event;

            lsm6ds_accel_gyro.readAcceleration(x, y, z);
            accel_x_read = roundf(x * 1000) / 1000.0f;
            accel_y_read = roundf(y * 1000) / 1000.0f;
            accel_z_read = roundf(z * 1000) / 1000.0f;

            accel_gyro.readGyroscope(x, y, z);
            gyro_x_read = roundf(x * 1000) / 1000.0f;
            gyro_y_read = roundf(y * 1000) / 1000.0f;
            gyro_z_read = roundf(z * 1000) / 1000.0f;

            accel_gyro.getEvent(NULL, NULL, &temp_event);
            temp_read = roundf(temp_event.temperature * 1000) / 1000.0f;

            // Timestamp
            Serial.print(millis());
            Serial.print("ms, T: ");

            break;
        }

        case SENSOR_DPS310_BARO_TEMP:
        {
            // true if new data available to read
            if (!dps310_baro_temp.temperatureAvailable() || !dps310_baro_temp.pressureAvailable())
            {
                return;
            }
            // Read sensor data (Temp: *C, Pressure: hPa) to three decimals
            sensors_event_t temp_event, pressure_event;
            dps310_baro_temp.getEvents(&temp_event, &pressure_event);
            temp_read = roundf(temp_event.temperature * 1000) / 1000.0f;
            press_read = roundf(pressure_event.pressure * 1000) / 1000.0f;

            // Timestamp
            Serial.print(millis());
            Serial.print("ms, T: ");

            break;
        }

        case SENSOR_BMI088_ACCEL:
        {
            // availability checker not supported

            // Read sensor data (Accel: m/ss, Gyro: rad/s) to three decimals
            bmi088_accel.readSensor();
            bmi088_gyro.readSensor();

            accel_x_read = roundf(bmi088_accel.getAccelX_mss() * 1000) / 1000.0f;
            accel_y_read = roundf(bmi088_accel.getAccelY_mss() * 1000) / 1000.0f;
            accel_z_read = roundf(bmi088_accel.getAccelZ_mss() * 1000) / 1000.0f;
            gyro_x_read = roundf(bmi088_gyro.getGyroX_rads() * 1000) / 1000.0f;
            gyro_y_read = roundf(bmi088_gyro.getGyroY_rads() * 1000) / 1000.0f;
            gyro_z_read = roundf(bmi088_gyro.getGyroZ_rads() * 1000) / 1000.0f;
            temp_read = roundf(bmi088_accel.getTemperature_C() * 1000) / 1000.0f;

            // Timestamp
            Serial.print(millis());
            Serial.print("ms, T: ");

            break;
        }

        case SENSOR_BMP390_BARO:
        {
            // availability checker not supported

            // Read sensor data (Temp: *C, Pressure: hPa) to three decimals
            bmp390_baro.performReading();
            temp_read = roundf(bmp390_baro.temperature * 1000) / 1000.0f;
            press_read = roundf(bmp390_baro.pressure * 1000) / 1000.0f;
            bmp390_alt_read = roundf(bmp390_baro.readAltitude(SEALEVELPRESSURE_HPA) * 1000) / 1000.0f;

            // Timestamp
            Serial.print(millis());
            Serial.print("ms, T: ");

            break;
        }

        case SENSOR_LIS2MDL_MAG:
        {
            // availability checker not supported

            // Read sensor data (Magnetic vector: uT) to three decimals
            sensors_event_t mag_event;
            lis2mdl_mag.getEvent(&mag_event);

            mag_x_read = roundf(mag_event.magnetic.x * 1000) / 1000.0f;
            mag_y_read = roundf(mag_event.magnetic.y * 1000) / 1000.0f;
            mag_z_read = roundf(mag_event.magnetic.z * 1000) / 1000.0f;

            // Timestamp
            Serial.print(millis());
            Serial.print("ms, T: ");

            break;
        }
        case SENSOR_HDC302_TEMP_HUM:
        {
            double readT = 0.0;
            double readRH = 0.0;

            // availability checker not supported

            // Read sensor data (Relative humidity: %, Temperature: *C) to three decimals
            hdc302_temp_hum.readOffsets(readT, readRH);
            hum_read = roundf(readT * 1000) / 1000.0f;
            temp_read = roundf(readRH * 1000) / 1000.0f;

            // Timestamp
            Serial.print(millis());
            Serial.print("ms,");

            break;
        }
            return;
        }
    }
}