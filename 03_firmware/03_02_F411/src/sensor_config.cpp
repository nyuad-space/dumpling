#include "globals.h"
#include "sensor_config.h"
#include "sensor_detect.h"

void initSensorComm(SensorType sensorType)
{
    switch (sensorType)
    {
    case SENSOR_LSM6DS_ACCEL_GYRO:
        lsm6ds_accel_gyro.begin_SPI(LSM6DS_CS, &SENSOR_SPI);
        break;
    case SENSOR_DPS310_BARO_TEMP:
        dps310_baro_temp.begin_SPI(DPS310_CS, &SENSOR_SPI);
        break;
    case SENSOR_BMI088_ACCEL:
        // BMI088 automatically handles the correct SPI bus!
        break;
    case SENSOR_BMP390_BARO:
        bmp390_baro.begin_I2C(BMP390_I2C_ADDR, &SENSOR_I2C);
        break;
    case SENSOR_LIS2MDL_MAG:
        lis2mdl_mag.begin(LIS2MDL_I2C_ADDR, &SENSOR_I2C);
        break;
    case SENSOR_HDC302_TEMP_HUM:
        hdc302_temp_hum.begin(HDC302_I2C_ADDR, &SENSOR_I2C);
        break;
    case SENSOR_UNKNOWN: // SENSOR_UNKNOWN
        break;
    default:
        break;
    }
    return;
}

void configSensor(SensorType sensorType)
{
    switch (sensorType)
    {
    case SENSOR_LSM6DS_ACCEL_GYRO:
        lsm6ds_accel_gyro.setAccelRange(LSM6DSO32_ACCEL_RANGE_8_G);
        Serial.println("Accelerometer range set to: +-8G");
        lsm6ds_accel_gyro.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS);
        Serial.println("Gyro range set to: 250 degrees/s");
        // Output Data Range (ODR)= 16MB/(data size*600sec)
        lsm6ds_accel_gyro.setAccelDataRate(LSM6DS_RATE_104_HZ);
        Serial.println("Accelerometer data rate set to: 104 Hz");
        lsm6ds_accel_gyro.setGyroDataRate(LSM6DS_RATE_104_HZ);
        Serial.println("Gyro data rate set to: 104 Hz");
        break;
    case SENSOR_DPS310_BARO_TEMP:
        dps310_baro_temp.configurePressure(DPS310_64HZ, DPS310_8SAMPLES);
        Serial.println("Pressure data rate / oversampling set to: 64 Hz / 8x");
        dps310_baro_temp.configureTemperature(DPS310_64HZ, DPS310_8SAMPLES);
        Serial.println("Temperature data rate / oversampling set to: 64 Hz / 8x");
        break;
    case SENSOR_BMI088_ACCEL:
        bmi088_accel.setOdr(Bmi088Accel::ODR_1600HZ_BW_280HZ); // Output data rate and bandwidth
        Serial.println("Accelerometer output data rate / bandwidth set to: 1600 Hz / 280 Hz");
        bmi088_accel.setRange(Bmi088Accel::RANGE_24G);
        Serial.println("Accelerometer range set to: 24 G");
        bmi088_gyro.setOdr(Bmi088Gyro::ODR_200HZ_BW_23HZ);
        Serial.println("Gyro output data rate / bandwidth set to: 1600 Hz / 280 Hz");
        bmi088_gyro.setRange(Bmi088Gyro::RANGE_1000DPS);
        Serial.println("Gyro range set to: 1000 degrees/s");

        bmi088_accel.setOdr(Bmi088Accel::ODR_1600HZ_BW_280HZ); // Output data rate and bandwidth
        Serial.println("Accelerometer output data rate / bandwidth set to: 1600 Hz / 280 Hz");
        bmi088_accel.setRange(Bmi088Accel::RANGE_24G);
        Serial.println("Accelerometer range set to: 24 G");
        bmi088_gyro.setOdr(Bmi088Gyro::ODR_200HZ_BW_23HZ);
        Serial.println("Gyro output data rate / bandwidth set to: 1600 Hz / 280 Hz");
        bmi088_gyro.setRange(Bmi088Gyro::RANGE_1000DPS);
        Serial.println("Gyro range set to: 1000 degrees/s");
        break;
    case SENSOR_BMP390_BARO:
        bmp390_baro.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
        Serial.println("Temperature oversampling set to: 8x");
        bmp390_baro.setPressureOversampling(BMP3_OVERSAMPLING_8X);
        Serial.println("Pressure oversampling set to: 8x");
        bmp390_baro.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3); // IIR (Infiinite Impulse Response) smooth out sudden changes in readings
        Serial.println("IIR Filter coefficient set to: 3");
        bmp390_baro.setOutputDataRate(BMP3_ODR_50_HZ);
        Serial.println("Output data rate set to: 50 Hz");
        break;
    case SENSOR_LIS2MDL_MAG:
        lis2mdl_mag.reset(); // Set up register bits (Temperature compensation, Continuous mode, Block Data Update, etc.)
        lis2mdl_mag.setDataRate(LIS2MDL_RATE_100_HZ);
        Serial.println("Data rate set to: 100 Hz");
        lis2mdl_mag.enableAutoRange(true); // Enable higher precision
        Serial.println("Enabled Auto Range adjustment.");
        break;
    case SENSOR_HDC302_TEMP_HUM:
        hdc302_temp_hum.heaterEnable(HEATER_QUARTER_POWER); // Enable heater for better accuracy against condensation
        Serial.println("Enabled heater.");
        // set measurement resolution, measurement interval, etc. (no function found)
        break;
    case SENSOR_UNKNOWN: // SENSOR_UNKNOWN
        break;
    default:
        break;
    }
    return;
}