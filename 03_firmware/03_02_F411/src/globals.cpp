#include "globals.h"
#include "pinout.h"

// Bus
SPIClass INTERBOARD_SPI(INTERBOARD_MOSI, INTERBOARD_MISO, INTERBOARD_SCK);
SPIClass SENSOR_SPI(SENSOR_MOSI, SENSOR_MISO, SENSOR_SCK);
SPIClass FLASH_SPI(FLASH_MOSI, FLASH_MISO, FLASH_SCK);
TwoWire SENSOR_I2C(SENSOR_SDA, SENSOR_SCL);

// Declare the sensor objects
Adafruit_LSM6DSO32 lsm6ds;
Adafruit_DPS310 dps310;
Bmi088Accel bmiAccel(SENSOR_SPI, bmiAccel_CS);
Bmi088Gyro bmiGyro(SENSOR_SPI, bmiGyro_CS);
Adafruit_BMP3XX bmp3xx;
Adafruit_LIS2MDL lis2mdl;
Adafruit_HDC302x hdc;
