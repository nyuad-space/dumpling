#include "globals.h"
#include "sensor_read.h"
#include "pinout.h"

void readSensor(SensorType sensorType)

{
    switch (sensorType)
    {
    case SENSOR_LSM6DS_ACCEL_GYRO:
        break;

    case SENSOR_DPS310_BARO_TEMP:

        // remove below line when sensor_config.cpp is resolved
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
        float T = temp_event.temperature;
        float P = pressure_event.pressure;

        // Display on Serial

        Serial.print(millis());
        Serial.print("ms, T: ");
        Serial.print(T, 3);
        Serial.print("*C, P: ");
        Serial.print(P, 3);
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