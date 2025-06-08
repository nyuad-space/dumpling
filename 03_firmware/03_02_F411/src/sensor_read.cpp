#include "globals.h"
#include "sensor_read.h"
#include "pinout.h"

unsigned long previousMillis = 0; // will store last time LED was updated
const long interval = 1000;       // interval at which to blink (milliseconds)
#define SEALEVELPRESSURE_HPA (1013.25)

void readSensor(SensorType sensorType)

{
    unsigned long currentMillis = millis();

    switch (sensorType)
    {
    case SENSOR_LSM6DS_ACCEL_GYRO:
    {
        if (currentMillis - previousMillis >= interval)
        {
            previousMillis = currentMillis;

            // true if new data available to read
            if (!lsm6ds_accel_gyro.accelerationAvailable() || !lsm6ds_accel_gyro.gyroscopeAvailable())
            {
                return;
            }
            // Read sensor data (Accel: m/s^2, Gyro: rad/s)
            sensors_event_t accel_event, gyro_event, temp_event;
            lsm6ds_accel_gyro.getEvent(&accel_event, &gyro_event, &temp_event);

            // Display on Serial (Timestamp done internally)
            Serial.print("Accel X: ");
            Serial.print(accel_event.acceleration.x, 3);
            Serial.print(" Accel Y: ");
            Serial.print(accel_event.acceleration.y, 3);
            Serial.print(" Accel Z: ");
            Serial.print(accel_event.acceleration.z, 3);
            Serial.print(" Gyro X: ");
            Serial.print(gyro_event.gyro.x, 3);
            Serial.print(" Gyro Y: ");
            Serial.print(gyro_event.gyro.y, 3);
            Serial.print(" Gyro Z: ");
            Serial.print(gyro_event.gyro.z, 3);
            Serial.print(" T: ");
            Serial.println(temp_event.temperature, 3);
        }
        break;
    }

    case SENSOR_DPS310_BARO_TEMP:
    {
        if (currentMillis - previousMillis >= interval)
        {
            previousMillis = currentMillis;

            // true if new data available to read
            if (!dps310_baro_temp.temperatureAvailable() || !dps310_baro_temp.pressureAvailable())
            {
                return;
            }
            // Read sensor data (Temp: *C, Pressure: hPa)
            sensors_event_t temp_event, pressure_event;
            dps310_baro_temp.getEvents(&temp_event, &pressure_event);

            // Timestamp
            Serial.print(millis());
            Serial.print("ms, T: ");
            // Display on Serial
            Serial.print(temp_event.temperature, 3);
            Serial.print("*C, P: ");
            Serial.print(pressure_event.pressure, 3);
            Serial.println("hPa");
        }
        break;
    }

    case SENSOR_BMI088_ACCEL:
    {
        if (currentMillis - previousMillis >= interval)
        {
            previousMillis = currentMillis;

            // availability checker not supported

            // Read sensor data (Accel: m/ss, Gyro: rad/s)
            bmi088_accel.readSensor();
            bmi088_gyro.readSensor();

            // Timestamp
            Serial.print(millis());
            Serial.print("ms, T: ");
            // Display on Serial
            Serial.print(bmi088_accel.getAccelX_mss());
            Serial.print("\t");
            Serial.print(bmi088_accel.getAccelY_mss());
            Serial.print("\t");
            Serial.print(bmi088_accel.getAccelZ_mss());
            Serial.print("\t");
            Serial.print(bmi088_gyro.getGyroX_rads());
            Serial.print("\t");
            Serial.print(bmi088_gyro.getGyroY_rads());
            Serial.print("\t");
            Serial.print(bmi088_gyro.getGyroZ_rads());
            Serial.print("\t");
            Serial.print(bmi088_accel.getTemperature_C());
            Serial.print("\n");
        }
        break;
    }

    case SENSOR_BMP390_BARO:
    {
        if (currentMillis - previousMillis >= interval)
        {
            previousMillis = currentMillis;

            // availability checker not supported

            // Read sensor data (Temp: *C, Pressure: hPa)
            bmp390_baro.performReading();

            // Timestamp
            Serial.print(millis());
            Serial.print("ms, T: ");
            // Display on Serial
            Serial.print(bmp390_baro.temperature, 3);
            Serial.print("*C, P: ");
            Serial.print(bmp390_baro.pressure, 3);
            Serial.print("hPa");
            Serial.print(bmp390_baro.readAltitude(SEALEVELPRESSURE_HPA), 3);
            Serial.println("m");
        }
        break;
    }

    case SENSOR_LIS2MDL_MAG:
    {
        if (currentMillis - previousMillis >= interval)
        {
            previousMillis = currentMillis;

            // availability checker not supported

            // Read sensor data (Magnetic vector: uT)
            sensors_event_t mag_event;
            lis2mdl_mag.getEvent(&mag_event);

            // Timestamp
            Serial.print(millis());
            Serial.print("ms, T: ");
            // Display on Serial
            Serial.print("X: ");
            Serial.print(mag_event.magnetic.x);
            Serial.print("  ");
            Serial.print("Y: ");
            Serial.print(mag_event.magnetic.y);
            Serial.print("  ");
            Serial.print("Z: ");
            Serial.print(mag_event.magnetic.z);
            Serial.print("  ");
            Serial.println("uT");
        }
        break;
    }
    case SENSOR_HDC302_TEMP_HUM:
    {
        double readT = 0.0;
        double readRH = 0.0;

        if (currentMillis - previousMillis >= interval)
        {
            previousMillis = currentMillis;

            // availability checker not supported

            // Read sensor data (Relative humidity: %, Temperature: *C)
            hdc302_temp_hum.readOffsets(readT, readRH);

            // Timestamp
            Serial.print(millis());
            Serial.print("ms,");
            // Display on Serial
            Serial.print(" RH Offset: ");
            Serial.print(readRH);

            Serial.print(", T Offset: ");
            Serial.print(readT);
            Serial.print("\n");
        }
        break;
    }

        return;
    }
}