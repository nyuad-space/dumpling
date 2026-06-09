#pragma once
#include <Arduino.h>
#include <Adafruit_LSM6DSO32.h>

struct LSM6DSOsample
{
    uint32_t timestamp_ms;
    float ax_mps2;
    float ay_mps2;
    float az_mps2;
    float gx_rads;
    float gy_rads;
    float gz_rads;
    float temp_c;
}; // data packet

class LSM6DSOnode
{
public:
    bool begin();
    bool readSample(LSM6DSOsample &sample);
    void printCsvHeader(Stream &out);
    void printCsvSample(Stream &out, const LSM6DSOsample &sample);

private:
    Adafruit_LSM6DSO32 imu_; // Adafruit library object
}; // custom sensor driver object