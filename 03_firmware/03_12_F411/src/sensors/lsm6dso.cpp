#include "lsm6dso.h"
#include "pinout.h"
#include <SPI.h>

namespace
{
    constexpr float ROUND_SCALE = 1000.0f;
    float round3(float value)
    {
        return roundf(value * ROUND_SCALE) / ROUND_SCALE;
    }
    SPIClass sensorSpi(SENSOR_MOSI, SENSOR_MISO, SENSOR_SCK); // SPI bus object
} // private internals to prevent name collision

bool LSM6DSOnode::begin()
{
    sensorSpi.begin();
    if (!imu_.begin_SPI(LSM6DS_CS, &sensorSpi))
        return false;
    imu_.setAccelRange(LSM6DSO32_ACCEL_RANGE_16_G);
    imu_.setGyroRange(LSM6DS_GYRO_RANGE_2000_DPS);
    imu_.setAccelDataRate(LSM6DS_RATE_104_HZ);
    imu_.setGyroDataRate(LSM6DS_RATE_104_HZ);
    return true;
}
bool LSM6DSOnode::readSample(LSM6DSOsample &sample)
{
    if (!imu_.accelerationAvailable() || !imu_.gyroscopeAvailable())
        return false;
    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;

    imu_.getEvent(&accel, &gyro, &temp);

    sample.timestamp_ms = millis();
    sample.ax_mps2 = round3(accel.acceleration.x);
    sample.ay_mps2 = round3(accel.acceleration.y);
    sample.az_mps2 = round3(accel.acceleration.z);
    sample.gx_rads = round3(gyro.gyro.x);
    sample.gy_rads = round3(gyro.gyro.y);
    sample.gz_rads = round3(gyro.gyro.z);
    sample.temp_c = round3(temp.temperature);

    return true;
}
void LSM6DSOnode::printCsvHeader(Stream &out)
{
    out.println("timestamp_ms, ax_mps2, ay_mps2, az_mps2, gx_rads, gy_rads, gz_rads, temp_c");
}
void LSM6DSOnode::printCsvSample(Stream &out, const LSM6DSOsample &sample)
{
    out.print(sample.timestamp_ms);
    out.print(',');
    out.print(sample.ax_mps2, 3);
    out.print(',');
    out.print(sample.ay_mps2, 3);
    out.print(',');
    out.print(sample.az_mps2, 3);
    out.print(',');
    out.print(sample.gx_rads, 3);
    out.print(',');
    out.print(sample.gy_rads, 3);
    out.print(',');
    out.print(sample.gz_rads, 3);
    out.print(',');
    out.println(sample.temp_c, 3);
}