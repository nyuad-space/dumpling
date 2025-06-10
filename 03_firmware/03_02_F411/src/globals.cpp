#include "globals.h"
#include "pinout.h"

// Bus
SPIClass INTERBOARD_SPI(INTERBOARD_MOSI, INTERBOARD_MISO, INTERBOARD_SCK);
SPIClass SENSOR_SPI(SENSOR_MOSI, SENSOR_MISO, SENSOR_SCK);
SPIClass FLASH_SPI(FLASH_MOSI, FLASH_MISO, FLASH_SCK);
TwoWire SENSOR_I2C(SENSOR_SDA, SENSOR_SCL);

// Declare the sensor objects
Adafruit_LSM6DSO32 lsm6ds_accel_gyro;
Adafruit_DPS310 dps310_baro_temp;
Bmi088Accel bmi088_accel(SENSOR_SPI, BMI088_Accel_CS);
Bmi088Gyro bmi088_gyro(SENSOR_SPI, BMI088_Gyro_CS);
Adafruit_BMP3XX bmp390_baro;
Adafruit_LIS2MDL lis2mdl_mag;
Adafruit_HDC302x hdc302_temp_hum;

// Sensor detect
SensorType detectedSensor = SENSOR_UNKNOWN;

// Flash
Adafruit_FlashTransport_SPI adafruit_flash_spi(FLASH_CS, FLASH_SPI);
Adafruit_SPIFlash flash_memory(&adafruit_flash_spi);
FatVolume fatfs;
File32 lsm6dsFile;
File32 dps310File;
File32 bmi088File;
File32 bmp390File;
File32 lis2mdlFile;
File32 hdc302File;

// Sensor readings
float accel_x_read;
float accel_y_read;
float accel_z_read;
float gyro_x_read;
float gyro_y_read;
float gyro_z_read;
float temp_read;
float press_read;
float bmp390_alt_read;
float mag_x_read;
float mag_y_read;
float mag_z_read;
float hum_read;
