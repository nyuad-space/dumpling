#include "sensor_read.h"

#define SEALEVELPRESSURE_HPA (1013.25)

// Time buffer
unsigned long previousMillis = 0; // store last updated time
const long interval = 1000;       // interval at which to delay (milliseconds)

// == readings for each sensors ==

void read_LSM6DS()
{
    // true if new data available to read
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

    lsm6ds_accel_gyro.readGyroscope(x, y, z);
    gyro_x_read = roundf(x * 1000) / 1000.0f;
    gyro_y_read = roundf(y * 1000) / 1000.0f;
    gyro_z_read = roundf(z * 1000) / 1000.0f;

    lsm6ds_accel_gyro.getEvent(NULL, NULL, &temp_event);
    temp_read = roundf(temp_event.temperature * 1000) / 1000.0f;

    // Timestamp
    Serial.print(millis());
    Serial.print("ms, T: ");

    // Display on Serial (Timestamp done internally)
    Serial.print("Accel X: ");
    Serial.print(accel_x_read);
    Serial.print(" Accel Y: ");
    Serial.print(accel_y_read);
    Serial.print(" Accel Z: ");
    Serial.print(accel_z_read);
    Serial.print(" Gyro X: ");
    Serial.print(gyro_x_read);
    Serial.print(" Gyro Y: ");
    Serial.print(gyro_y_read);
    Serial.print(" Gyro Z: ");
    Serial.print(gyro_z_read);
    Serial.print(" T: ");
    Serial.println(temp_read);
}
void read_DPS310()
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

    // // TimestampAdd commentMore actions
    // Serial.print(millis());
    // Serial.print("ms, T: ");

    // // Display on Serial
    // Serial.print(temp_read);
    // Serial.print("*C, P: ");
    // Serial.print(press_read);
    // Serial.println("hPa");
}
void read_BMI088()
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

    // // Timestamp
    // Serial.print(millis());
    // Serial.print("ms, T: ");

    // // Display on Serial
    // Serial.print(accel_x_read);
    // Serial.print("\t");
    // Serial.print(accel_y_read);
    // Serial.print("\t");
    // Serial.print(accel_z_read);
    // Serial.print("\t");
    // Serial.print(gyro_x_read);
    // Serial.print("\t");
    // Serial.print(gyro_y_read);
    // Serial.print("\t");
    // Serial.print(gyro_z_read);
    // Serial.print("\t");
    // Serial.print(temp_read);
    // Serial.print("\n");
}
void read_BMP390()
{
    // availability checker not supported

    // Read sensor data (Temp: *C, Pressure: hPa) to three decimals
    bmp390_baro.performReading();
    temp_read = roundf(bmp390_baro.temperature * 1000) / 1000.0f;
    press_read = roundf(bmp390_baro.pressure * 1000) / 1000.0f;
    bmp390_alt_read = roundf(bmp390_baro.readAltitude(SEALEVELPRESSURE_HPA) * 1000) / 1000.0f;

    // // Timestamp
    // Serial.print(millis());
    // Serial.print("ms, T: ");

    // // Display on Serial
    // Serial.print(temp_read);
    // Serial.print("*C, P: ");
    // Serial.print(press_read);
    // Serial.print("hPa");
    // Serial.print(bmp390_alt_read);
    // Serial.println("m");
}
void read_LIS2MDL()
{
    // availability checker not supported

    // Read sensor data (Magnetic vector: uT) to three decimals
    sensors_event_t mag_event;
    lis2mdl_mag.getEvent(&mag_event);

    mag_x_read = roundf(mag_event.magnetic.x * 1000) / 1000.0f;
    mag_y_read = roundf(mag_event.magnetic.y * 1000) / 1000.0f;
    mag_z_read = roundf(mag_event.magnetic.z * 1000) / 1000.0f;

    // // Timestamp
    // Serial.print(millis());
    // Serial.print("ms, T: ");

    // // Display on Serial
    // Serial.print("X: ");
    // Serial.print(mag_x_read);
    // Serial.print("  ");
    // Serial.print("Y: ");
    // Serial.print(mag_y_read);
    // Serial.print("  ");
    // Serial.print("Z: ");
    // Serial.print(mag_z_read);
    // Serial.print("  ");
    // Serial.println("uT");
}
void read_HDC302()
{
    double readT = 0.0;
    double readRH = 0.0;

    // availability checker not supported

    // Read sensor data (Relative humidity: %, Temperature: *C) to three decimals
    hdc302_temp_hum.readOffsets(readT, readRH);
    hum_read = roundf(readT * 1000) / 1000.0f;
    temp_read = roundf(readRH * 1000) / 1000.0f;

    // // Timestamp
    // Serial.print(millis());
    // Serial.print("ms,");

    // // Display on Serial
    // Serial.print(" RH Offset: ");
    // Serial.print(hum_read);
    // Serial.print(", T Offset: ");
    // Serial.print(temp_read);
    // Serial.print("\n");
}

// == central function ==
void readSensor(SensorType sensorType)
{
    // Give buffer time between new sensor reading
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis;

        // Read sensors
        switch (sensorType)
        {
        case SENSOR_LSM6DS_ACCEL_GYRO:
        {
            read_LSM6DS();
            break;
        }

        case SENSOR_DPS310_BARO_TEMP:
        {
            read_DPS310();
            break;
        }

        case SENSOR_BMI088_ACCEL:
        {
            read_BMI088();
            break;
        }

        case SENSOR_BMP390_BARO:
        {
            read_BMP390();
            break;
        }

        case SENSOR_LIS2MDL_MAG:
        {
            read_LIS2MDL();
            break;
        }
        case SENSOR_HDC302_TEMP_HUM:
        {
            read_HDC302();
            break;
        }
            return;
        }
        // Write to flash in same rate as reading from sensor
        writeToFlash(sensorType);
        // // Read the data (for debug)
        // readFromFlash(sensorType);
    }
}