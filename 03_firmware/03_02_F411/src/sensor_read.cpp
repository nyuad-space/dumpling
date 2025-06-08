#include "globals.h"
#include "sensor_read.h"
#include "pinout.h"

void readSensor(SensorType sensorType)

{
    switch (sensorType)
    {
    case SENSOR_LSM6DS_ACCEL_GYRO:
        // NOTE: remove below line when sensor_config.cpp is resolved
        lsm6ds_accel_gyro.begin_SPI(LSM6DS_CS, &SENSOR_SPI);

        // Read sensor data
        sensors_event_t accel_event, gyro_event, temp_event;
        dps310_baro_accel.getEvents(&accel_event, &gyro_event, &temp_event);

        // Display on Serial
        Serial.print(millis());
        Serial.print("ms, T: ");
        Serial.print(accel_event.acceleration, 3);
        Serial.print("*C, P: ");
        Serial.print(gyro_event.gyro, 3);
        Serial.println("hPa");

        break;

    case SENSOR_DPS310_BARO_TEMP:

        // NOTE: remove below line when sensor_config.cpp is resolved
        dps310_baro_temp.begin_SPI(DPS310_CS, &SENSOR_SPI);

        // true if new data available to read
        if (!dps310_baro_temp.temperatureAvailable() || !dps310_baro_temp.pressureAvailable())
        {
            delay(100);
            return;
        }
        // Read sensor data
        sensors_event_t temp_event, pressure_event;
        dps310_baro_temp.getEvents(&temp_event, &pressure_event);

        // Display on Serial
        Serial.print(millis());
        Serial.print("ms, T: ");
        Serial.print(temp_event.temperature, 3);
        Serial.print("*C, P: ");
        Serial.print(pressure_event.pressure, 3);
        Serial.println("hPa");

        // into SPI Packet

        break;

        // case SENSOR_BMI088_ACCEL:

        //     break;

        // case SENSOR_BMI088_GYRO:

        //     break;

        // case SENSOR_BMP390_BARO:

        //     break;

        // case SENSOR_LIS2MDL_MAG:

        //     break;

        // case SENSOR_HDC302_TEMP_HUM:

        //     break;
    }

    return;
}