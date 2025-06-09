#include "globals.h"
#include "sensor_read.h"
#include "pinout.h"

#define SEALEVELPRESSURE_HPA (1013.25)

// delay
unsigned long previousMillis = 0; // store last updated time
const long interval = 1000;       // interval at which to delay (milliseconds)

// LSM6DS
float lsm6ds_accel_x_read;
float lsm6ds_accel_y_read;
float lsm6ds_accel_z_read;
float lsm6ds_gyro_x_read;
float lsm6ds_gyro_y_read;
float lsm6ds_gyro_z_read;
float lsm6ds_temp_read;
// DPS310
float dps310_temp_read;
float dps310_press_read;
// BMI088
float bmi088_accel_x_read;
float bmi088_accel_y_read;
float bmi088_accel_z_read;
float bmi088_gyro_x_read;
float bmi088_gyro_y_read;
float bmi088_gyro_z_read;
float bmi088_temp_read;
// BMP390
float bmp390_temp_read;
float bmp390_press_read;
float bmp390_alt_read;
// LIS2MDL
float lis2mdl_mag_x_read;
float lis2mdl_mag_y_read;
float lis2mdl_mag_z_read;
// HDC302
float hdc302_hum_read;
float hdc302_temp_read;

// TODO: remove Serial prints

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

            // Read sensor data (Accel: m/s^2, Gyro: rad/s) to three decimals
            float x, y, z, temp;
            sensors_event_t temp_event;

            lsm6ds_accel_gyro.readAcceleration(x, y, z);
            lsm6ds_accel_x_read = roundf(x * 1000) / 1000.0f;
            lsm6ds_accel_y_read = roundf(y * 1000) / 1000.0f;
            lsm6ds_accel_z_read = roundf(z * 1000) / 1000.0f;

            lsm6ds_accel_gyro.readGyroscope(x, y, z);
            lsm6ds_gyro_x_read = roundf(x * 1000) / 1000.0f;
            lsm6ds_gyro_y_read = roundf(y * 1000) / 1000.0f;
            lsm6ds_gyro_z_read = roundf(z * 1000) / 1000.0f;

            lsm6ds_accel_gyro.getEvent(NULL, NULL, &temp_event);
            lsm6ds_temp_read = roundf(temp_event.temperature * 1000) / 1000.0f;

            // Timestamp
            Serial.print(millis());
            Serial.print("ms, T: ");
            // Display on Serial (Timestamp done internally)
            Serial.print("Accel X: ");
            Serial.print(lsm6ds_accel_x_read);
            Serial.print(" Accel Y: ");
            Serial.print(lsm6ds_accel_y_read);
            Serial.print(" Accel Z: ");
            Serial.print(lsm6ds_accel_z_read);
            Serial.print(" Gyro X: ");
            Serial.print(lsm6ds_gyro_x_read);
            Serial.print(" Gyro Y: ");
            Serial.print(lsm6ds_gyro_y_read);
            Serial.print(" Gyro Z: ");
            Serial.print(lsm6ds_gyro_z_read);
            Serial.print(" T: ");
            Serial.println(lsm6ds_temp_read, 3);
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
            // Read sensor data (Temp: *C, Pressure: hPa) to three decimals
            sensors_event_t temp_event, pressure_event;
            dps310_baro_temp.getEvents(&temp_event, &pressure_event);
            dps310_temp_read = roundf(temp_event.temperature * 1000) / 1000.0f;
            dps310_press_read = roundf(pressure_event.pressure * 1000) / 1000.0f;

            // Timestamp
            Serial.print(millis());
            Serial.print("ms, T: ");
            // Display on Serial
            Serial.print(dps310_temp_read);
            Serial.print("*C, P: ");
            Serial.print(dps310_press_read);
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

            // Read sensor data (Accel: m/ss, Gyro: rad/s) to three decimals
            bmi088_accel.readSensor();
            bmi088_gyro.readSensor();

            bmi088_accel_x_read = roundf(bmi088_accel.getAccelX_mss() * 1000) / 1000.0f;
            bmi088_accel_y_read = roundf(bmi088_accel.getAccelY_mss() * 1000) / 1000.0f;
            bmi088_accel_z_read = roundf(bmi088_accel.getAccelZ_mss() * 1000) / 1000.0f;
            bmi088_gyro_x_read = roundf(bmi088_gyro.getGyroX_rads() * 1000) / 1000.0f;
            bmi088_gyro_y_read = roundf(bmi088_gyro.getGyroY_rads() * 1000) / 1000.0f;
            bmi088_gyro_z_read = roundf(bmi088_gyro.getGyroZ_rads() * 1000) / 1000.0f;
            bmi088_temp_read = roundf(bmi088_accel.getTemperature_C() * 1000) / 1000.0f;

            // Timestamp
            Serial.print(millis());
            Serial.print("ms, T: ");
            // Display on Serial
            Serial.print(bmi088_accel_x_read);
            Serial.print("\t");
            Serial.print(bmi088_accel_y_read);
            Serial.print("\t");
            Serial.print(bmi088_accel_z_read);
            Serial.print("\t");
            Serial.print(bmi088_gyro_x_read);
            Serial.print("\t");
            Serial.print(bmi088_gyro_y_read);
            Serial.print("\t");
            Serial.print(bmi088_gyro_z_read);
            Serial.print("\t");
            Serial.print(bmi088_temp_read);
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

            // Read sensor data (Temp: *C, Pressure: hPa) to three decimals
            bmp390_baro.performReading();
            bmp390_temp_read = roundf(bmp390_baro.temperature * 1000) / 1000.0f;
            bmp390_press_read = roundf(bmp390_baro.pressure * 1000) / 1000.0f;
            bmp390_alt_read = roundf(bmp390_baro.readAltitude(SEALEVELPRESSURE_HPA) * 1000) / 1000.0f;

            // Timestamp
            Serial.print(millis());
            Serial.print("ms, T: ");
            // Display on Serial
            Serial.print(bmp390_temp_read);
            Serial.print("*C, P: ");
            Serial.print(bmp390_press_read);
            Serial.print("hPa");
            Serial.print(bmp390_alt_read);
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

            // Read sensor data (Magnetic vector: uT) to three decimals
            sensors_event_t mag_event;
            lis2mdl_mag.getEvent(&mag_event);

            lis2mdl_mag_x_read = roundf(mag_event.magnetic.x * 1000) / 1000.0f;
            lis2mdl_mag_y_read = roundf(mag_event.magnetic.y * 1000) / 1000.0f;
            lis2mdl_mag_z_read = roundf(mag_event.magnetic.z * 1000) / 1000.0f;

            // Timestamp
            Serial.print(millis());
            Serial.print("ms, T: ");
            // Display on Serial
            Serial.print("X: ");
            Serial.print(lis2mdl_mag_x_read);
            Serial.print("  ");
            Serial.print("Y: ");
            Serial.print(lis2mdl_mag_y_read);
            Serial.print("  ");
            Serial.print("Z: ");
            Serial.print(lis2mdl_mag_z_read);
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

            // Read sensor data (Relative humidity: %, Temperature: *C) to three decimals
            hdc302_temp_hum.readOffsets(readT, readRH);
            hdc302_hum_read = roundf(readT * 1000) / 1000.0f;
            hdc302_temp_read = roundf(readRH * 1000) / 1000.0f;

            // Timestamp
            Serial.print(millis());
            Serial.print("ms,");
            // Display on Serial
            Serial.print(" RH Offset: ");
            Serial.print(hdc302_hum_read);

            Serial.print(", T Offset: ");
            Serial.print(hdc302_temp_read);
            Serial.print("\n");
        }
        break;
    }
        return;
    }
}