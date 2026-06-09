#include "globals.h"
#include "sensor_config.h"
#include "sensor_detect.h"

bool initSensorComm(SensorType sensorType)
{
    switch (sensorType)
    {
    case SENSOR_LSM6DS_ACCEL_GYRO:
        return lsm6ds_accel_gyro.begin_SPI(LSM6DS_CS, &SENSOR_SPI);
    case SENSOR_DPS310_BARO_TEMP:
        return dps310_baro_temp.begin_SPI(DPS310_CS, &SENSOR_SPI);
    case SENSOR_BMI088_ACCEL:
        // BMI088 automatically handles the correct SPI bus!
        return true;
    case SENSOR_BMP390_BARO:
        return bmp390_baro.begin_I2C(BMP390_I2C_ADDR, &SENSOR_I2C);
    case SENSOR_LIS2MDL_MAG:
        return lis2mdl_mag.begin(LIS2MDL_I2C_ADDR, &SENSOR_I2C);
    case SENSOR_HDC302_TEMP_HUM:
        return hdc302_temp_hum.begin(HDC302_I2C_ADDR, &SENSOR_I2C);
    case SENSOR_UNKNOWN: // SENSOR_UNKNOWN
        return false;
    default:
        return false;
    }
}

void configSensor(SensorType sensorType)
{
    switch (sensorType)
    {
    case SENSOR_LSM6DS_ACCEL_GYRO:
        lsm6ds_accel_gyro.setAccelRange(LSM6DSO32_ACCEL_RANGE_32_G);
        lsm6ds_accel_gyro.setGyroRange(LSM6DS_GYRO_RANGE_2000_DPS);
        // HIGH RATE: Capture vibrations, engine dynamics, separation events
        lsm6ds_accel_gyro.setAccelDataRate(LSM6DS_RATE_833_HZ);
        lsm6ds_accel_gyro.setGyroDataRate(LSM6DS_RATE_833_HZ);
        logStatus("Accelerometer range set to: +-32G");
        logStatus("Gyro range set to: 2000 degrees/s");
        logStatus("Accelerometer data rate set to: 833 Hz");
        logStatus("Gyro data rate set to: 833 Hz");
#if F411_DEBUG_MODE
        Serial.println("Accelerometer range set to: +-32G");
        Serial.println("Gyro range set to: 2000 degrees/s");
        Serial.println("Accelerometer data rate set to: 833 Hz");
        Serial.println("Gyro data rate set to: 833 Hz");
#endif
        break;

    case SENSOR_DPS310_BARO_TEMP:
        // MEDIUM RATE: Pressure changes are slower, but important for altitude
        dps310_baro_temp.configurePressure(DPS310_64HZ, DPS310_8SAMPLES);
        dps310_baro_temp.configureTemperature(DPS310_32HZ, DPS310_8SAMPLES);
        logStatus("Pressure data rate / oversampling set to: 64 Hz / 8x");
        logStatus("Temperature data rate / oversampling set to: 32 Hz / 8x");
#if F411_DEBUG_MODE
        Serial.println("Pressure data rate / oversampling set to: 64 Hz / 8x");
        Serial.println("Temperature data rate / oversampling set to: 32 Hz / 8x");
#endif
        break;

    case SENSOR_BMI088_ACCEL:
        bmi088_accel.begin(); // Activate sensor
        bmi088_gyro.begin();
        bmi088_accel.setOdr(Bmi088Accel::ODR_1600HZ_BW_280HZ); // Output data rate and bandwidth
        bmi088_accel.setRange(Bmi088Accel::RANGE_24G);
        bmi088_gyro.setOdr(Bmi088Gyro::ODR_1000HZ_BW_116HZ);
        bmi088_gyro.setRange(Bmi088Gyro::RANGE_2000DPS);
        logStatus("Accelerometer output data rate / bandwidth set to: 1600 Hz / 280 Hz");
        logStatus("Accelerometer range set to: 24 G");
        logStatus("Gyro output data rate / bandwidth set to: 1000 Hz / 116 Hz");
        logStatus("Gyro range set to: 2000 degrees/s");
#if F411_DEBUG_MODE
        Serial.println("Accelerometer output data rate / bandwidth set to: 1600 Hz / 280 Hz");
        Serial.println("Accelerometer range set to: 24 G");
        Serial.println("Gyro output data rate / bandwidth set to: 1000 Hz / 116 Hz");
        Serial.println("Gyro range set to: 2000 degrees/s");
#endif
        break;

    case SENSOR_BMP390_BARO:
        bmp390_baro.setTemperatureOversampling(BMP3_OVERSAMPLING_4X);
        bmp390_baro.setPressureOversampling(BMP3_OVERSAMPLING_8X);
        bmp390_baro.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_7); // IIR (Infiinite Impulse Response) smooth out sudden changes in readings
        bmp390_baro.setOutputDataRate(BMP3_ODR_100_HZ);
        logStatus("Temperature oversampling set to: 4x");
        logStatus("Pressure oversampling set to: 8x");
        logStatus("IIR Filter coefficient set to: 7");
        logStatus("Output data rate set to: 100 Hz");
#if F411_DEBUG_MODE
        Serial.println("Temperature oversampling set to: 4x");
        Serial.println("Pressure oversampling set to: 8x");
        Serial.println("IIR Filter coefficient set to: 7");
        Serial.println("Output data rate set to: 100 Hz");
#endif
        break;

    case SENSOR_LIS2MDL_MAG:
        lis2mdl_mag.reset(); // Set up register bits (Temperature compensation, Continuous mode, Block Data Update, etc.)
        // LOW RATE: Magnetic field changes slowly, mainly for orientation reference
        lis2mdl_mag.setDataRate(LIS2MDL_RATE_50_HZ);
        lis2mdl_mag.enableAutoRange(true); // Enable higher precision
        logStatus("Data rate set to: 50 Hz");
        logStatus("Enabled Auto Range adjustment.");
#if F411_DEBUG_MODE
        Serial.println("Data rate set to: 50 Hz");
        Serial.println("Enabled Auto Range adjustment.");
#endif
        break;

    case SENSOR_HDC302_TEMP_HUM:
        hdc302_temp_hum.heaterEnable(HEATER_QUARTER_POWER); // Enable heater for better accuracy against condensation
        logStatus("Enabled heater.");
#if F411_DEBUG_MODE
        Serial.println("Enabled heater.");
#endif
        // set measurement resolution, measurement interval, etc. (no function found)
        break;
    case SENSOR_UNKNOWN: // SENSOR_UNKNOWN
        break;
    default:
        break;
    }
    return;
}