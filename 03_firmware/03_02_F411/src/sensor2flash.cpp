#include "main.h"

// LSM6DSO32
void log_lsm6dso32(FatVolume &fatfs)
{
    // standardized struct by Adafruit Unified Sensor library
    sensor_event_t accel_event, gyro_event, temp_event;
    lsm6ds.getEvent(&accel_event, &gyro_event, &temp_event);

    // Copy sensor reading into custom struct
    SensorData data;
    data.ax = accel_event.acceleration.x;
    data.ay = accel_event.acceleration.y;
    data.az = accel_event.acceleration.z;
    data.gx = gyro_event.gyro.x;
    data.gy = gyro_event.gyro.y;
    data.gz = gyro_event.gyro.z;
    data.temperature = temp_event.temperature;

    // Write to SdFat
    File sensorFile = fatfs.open("sensor.txt", FILE_WRITE);
    if (sensorFile)
    {
        sensorFile.print("Accel, x: ");
        sensorFile.print(data.ax); // m/s^2
        sensorFile.print("Accel, y: ");
        sensorFile.print(data.ay); // m/s^2
        sensorFile.print("Accel, z: ");
        sensorFile.print(data.az); // m/s^2
        sensorFile.print("Gyro, x: ");
        sensorFile.print(data.gx); // rad/s
        sensorFile.print("Gyro, y: ");
        sensorFile.print(data.gy); // rad/s
        sensorFile.print("Gyro, z: ");
        sensorFile.print(data.gz); // rad/s
        sensorFile.print("Temp: ");
        sensorFile.print(temp_event.temperature); // *C
        sensorFile.close();
    }
    else
    {
        Serial.println("Failed to open sensor.txt for writing");
    }
}

// DPS310
void log_DPS310(FatVolume &fatfs)
{
    // standardized struct by Adafruit Unified Sensor library
    sensors_event_t temp_event, pressure_event;
    dps310.getEvents(&temp_event, &pressure_event);

    // Copy sensor reading into custom struct
    SensorData data;
    data.temperature = temp_event.temperature;
    data.pressure = pressure_event.pressure;

    File sensorFile = fatfs.open("sensor.txt", FILE_WRITE);
    if (sensorFile)
    {
        sensorFile.print("Temp: ");
        sensorFile.print(data.temperature); // *C
        sensorFile.print("Pressure: ");
        sensorFile.print(data.pressure); // hPa
        sensorFile.close();
    }
    else
    {
        Serial.println("Failed to open sensor.txt for writing");
    }
}

// BMI088
void log_Bmi088Accel(FatVolume &fatfs)
{
    File sensorFile = fatfs.open("sensor.txt", FILE_WRITE);
    if (sensorFile)
    {
        sensorFile.print("Accel (m/s^2): ");
        sensorFile.print(ax);
        sensorFile.print(", ");
        sensorFile.print(ay);
        sensorFile.print(", ");
        sensorFile.println(az);

        sensorFile.print("Gyro (dps): ");
        sensorFile.print(gx);
        sensorFile.print(", ");
        sensorFile.print(gy);
        sensorFile.print(", ");
        sensorFile.println(gz);

        sensorFile.close();
    }
    else
    {
        Serial.println("Failed to open sensor.txt for writing");
    }
}

// dispatcher function
void logSensorData(SensorType sensorType, Adafruit_FlashTransport_SPI &flash, FatVolume &fatfs)
{
    switch (sensorType)
    {
    case SENSOR_LSM6DSO32:
        log_lsm6dso32(fatfs);
        break;
    case SENSOR_DPS310:
        log_DPS310(fatfs);
        break;
    case SENSOR_Bmi088Accel:
        log_Bmi088Accel(fatfs);
        break;
    case SENSOR_Bmi088Gyro:
        log_Bmi088Gyro(fatfs);
        break;
    case SENSOR_BMP3xx:
        log_BMP3xx(fatfs);
        break;
    case SENSOR_LIS2MDL:
        log_lis2mdl(fatfs);
        break;
    case SENSOR_HDC302x:
        log_hdc302x(fatfs);
        break;
    default:
        Serial.println("Unsupported sensor for temperature/pressure logging.");
        break;
    }
}
