#include "globals.h"
#include "sensor_config.h"

void configSensor(SensorType sensorType)
{
    switch (sensorType)
    {
    case SENSOR_LSM6DSO32:
        lsm6ds.setAccelRange(LSM6DSO32_ACCEL_RANGE_8_G);
        Serial.print("Accelerometer range set to: +-8G"); // 4, 8, 16, 32G
        lsm6ds.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS);
        Serial.print("Gyro range set to: 250 degrees/s"); // 125, 250, 500, 1000, 2000
        lsm6ds.setAccelDataRate(LSM6DS_RATE_12_5_HZ);
        Serial.print("Accelerometer data rate set to: 12.5 Hz"); // 0,12.5,26,52,104,208,416,833,1.66k,3.33k,6.66k
        lsm6ds.setGyroDataRate(LSM6DS_RATE_12_5_HZ);
        Serial.print("Gyro data rate set to: 12.5 Hz"); // 0,12.5,26,52,104,208,416,833,1.66k,3.33k,6.66k
        break;
    case SENSOR_DPS310:
        dps310.configurePressure(DPS310_64HZ, DPS310_64SAMPLES);
        dps310.configureTemperature(DPS310_64HZ, DPS310_64SAMPLES);
        break;
    case SENSOR_Bmi088Accel:
        bmiAccel.setOdr(Bmi088Accel::ODR_1600HZ_BW_280HZ); // Output data rate and bandwidth
        bmiAccel.setRange(Bmi088Accel::RANGE_24G);
        bmiGyro.setOdr(Bmi088Gyro::ODR_200HZ_BW_23HZ);
        bmiGyro.setRange(Bmi088Gyro::RANGE_1000DPS);
        break;
    case SENSOR_Bmi088Gyro:
        bmiAccel.setOdr(Bmi088Accel::ODR_1600HZ_BW_280HZ); // Output data rate and bandwidth
        bmiAccel.setRange(Bmi088Accel::RANGE_24G);
        bmiGyro.setOdr(Bmi088Gyro::ODR_200HZ_BW_23HZ);
        bmiGyro.setRange(Bmi088Gyro::RANGE_1000DPS);
        break;
    case SENSOR_BMP3xx:
        bmp3xx.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
        bmp3xx.setPressureOversampling(BMP3_OVERSAMPLING_4X);
        bmp3xx.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
        bmp3xx.setOutputDataRate(BMP3_ODR_50_HZ);
        break;
    case SENSOR_LIS2MDL:
        lis2mdl.setDataRate(LIS2MDL_RATE_100_HZ);
        lis2mdl.enableAutoRange(true);
        break;
    case SENSOR_HDC302: // no configuration settings?!
        break;
    default: // SENSOR_UNKNOWN
        break;
    }
}