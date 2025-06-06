// SPI bus (SPI3)
#define SENSOR_SCK PB3
#define SENSOR_MISO PB4
#define SENSOR_MOSI PB5

// Sensor CS
#define SENSOR_CS PA15
#define LSM6DS_CS PA15
#define DPS310_CS PA15
#define bmiAccel_CS PA15
#define bmiGyro_CS PA2

// inline void initSensorPins()
// {
//     pinMode(LSM6DS_CS, OUTPUT);
//     pinMode(DPS310_CS, OUTPUT);
//     pinMode(bmiAccel_CS, OUTPUT);
//     pinMode(bmiGyro_CS, OUTPUT);

//     digitalWrite(LSM6DS_CS, HIGH);
//     digitalWrite(DPS310_CS, HIGH);
//     digitalWrite(bmiAccel_CS, HIGH);
//     digitalWrite(bmiGyro_CS, HIGH);
// }

// SPI bus (SPI2)
#define INTERBOARD_SCK PB13
#define INTERBOARD_MISO PB14
#define INTERBOARD_MOSI PB15

// SPI bus (SPI1)
#define FLASH_SCK PA5
#define FLASH_MISO PA6
#define FLASH_MOSI PA7

// I2C sensors (I2C1)
#define SENSOR_SCL PB6
#define SENSOR_SDA PB7
