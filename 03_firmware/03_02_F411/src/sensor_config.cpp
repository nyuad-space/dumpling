// #include "sensor_config.h"
// #include "sensor_detect.h"

// void configSensor(SensorType sensor_type) {
//     switch (sensor_type) {
//         case SENSOR_UNKNOWN:
//             // No configuration needed for unknown sensor
//             printf("Warning: Unknown sensor type\n");
//             break;
            
//         case SENSOR_LSM6DS_ACCEL_GYRO:
//             // Configure LSM6DS accelerometer and gyroscope
//             printf("Configuring LSM6DS Accel/Gyro sensor\n");
//             // Set accelerometer range to ±2g, gyro range to ±250dps
//             // Enable data ready interrupt
//             // Set ODR to 104Hz
//             break;
            
//         case SENSOR_BMI088_ACCEL:
//             // Configure BMI088 accelerometer
//             printf("Configuring BMI088 Accelerometer\n");
//             // Set range to ±3g
//             // Set bandwidth to 40Hz
//             // Enable active mode
//             break;
            
//         case SENSOR_BMI088_GYRO:
//             // Configure BMI088 gyroscope
//             printf("Configuring BMI088 Gyroscope\n");
//             // Set range to ±500dps
//             // Set bandwidth to 47Hz
//             // Enable normal mode
//             break;
            
//         case SENSOR_DPS310_BARO_TEMP:
//             // Configure DPS310 barometer and temperature sensor
//             printf("Configuring DPS310 Barometer/Temperature sensor\n");
//             // Set pressure oversampling to 8x
//             // Set temperature oversampling to 8x
//             // Set measurement rate to 1Hz
//             break;
            
//         case SENSOR_BMP390_BARO:
//             // Configure BMP390 barometer
//             printf("Configuring BMP390 Barometer\n");
//             // Set pressure oversampling to 16x
//             // Set temperature oversampling to 2x
//             // Enable IIR filter coefficient 3
//             // Set ODR to 50Hz
//             break;
            
//         case SENSOR_LIS2MDL_MAG:
//             // Configure LIS2MDL magnetometer
//             printf("Configuring LIS2MDL Magnetometer\n");
//             // Set ODR to 100Hz
//             // Enable temperature compensation
//             // Set operating mode to continuous
//             break;
            
//         case SENSOR_HDC302_TEMP_HUM:
//             // Configure HDC302 temperature and humidity sensor
//             printf("Configuring HDC302 Temperature/Humidity sensor\n");
//             // Set measurement resolution to 14-bit
//             // Enable heater for humidity accuracy
//             // Set measurement interval to 1 second
//             break;
            
//         default:
//             printf("Error: Unsupported sensor type %d\n", sensor_type);
//             break;
//     }
// }
// // void configSensor(String sensorType)
// // {
// //     switch (sensorType)
// //     {
// //     case SENSOR_LSM6DSO32:
// //         lsm6ds.setAccelRange(LSM6DSO32_ACCEL_RANGE_8_G);
// //         Serial.print("Accelerometer range set to: +-8G"); // 4, 8, 16, 32G
// //         dso32.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS);
// //         Serial.print("Gyro range set to: 250 degrees/s"); // 125, 250, 500, 1000, 2000
// //         dso32.setAccelDataRate(LSM6DS_RATE_12_5_HZ);
// //         Serial.print("Accelerometer data rate set to: 12.5 Hz"); // 0,12.5,26,52,104,208,416,833,1.66k,3.33k,6.66k
// //         dso32.setGyroDataRate(LSM6DS_RATE_12_5_HZ);
// //         Serial.print("Gyro data rate set to: 12.5 Hz"); // 0,12.5,26,52,104,208,416,833,1.66k,3.33k,6.66k
// //         break;
// //     case SENSOR_DPS310:
// //         dps310.configurePressure(DPS310_64HZ, DPS310_64SAMPLES);
// //         dps310.configureTemperature(DPS310_64HZ, DPS310_64SAMPLES);
// //         break;
// //     case SENSOR_Bmi088Accel:
// //         bmiAccel.setOdr(ODR_1600HZ_BW_280HZ); // Output data rate and bandwidth
// //         bmiAccel.setRange(RANGE_24G);
// //         bmiGyro.setOdr(ODR_1600HZ_BW_280HZ);
// //         bmiGyro.setRange(RANGE_24G);
// //         break;
// //     case SENSOR_Bmi088Gyro:
// //         bmiAccel.setOdr(ODR_1600HZ_BW_280HZ); // Output data rate and bandwidth
// //         bmiAccel.setRange(RANGE_24G);
// //         bmiGyro.setOdr(ODR_1600HZ_BW_280HZ);
// //         bmiGyro.setRange(RANGE_24G);
// //         break;
// //     case SENSOR_BMP3xx:
// //         bmp3xx.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
// //         bmp3xx.setPressureOversampling(BMP3_OVERSAMPLING_4X);
// //         bmp3xx.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
// //         bmp3xx.setOutputDataRate(BMP3_ODR_50_HZ);
// //         break;
// //     case SENSOR_LIS2MDL:
// //         lis2mdl.setDataRate(LIS2MDL_RATE_100_HZ);
// //         lis2mdl.enableAutoRange(true);
// //         break;
// //     case SENSOR_HDC302x: // no configuration settings?!
// //         break;
// //     default: // SENSOR_UNKNOWN
// //         break;
// //     }
// // }