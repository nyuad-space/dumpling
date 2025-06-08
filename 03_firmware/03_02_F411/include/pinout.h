// SPI bus (SPI3)
// Use ALT1 pin names to avoid SPI conflict. 
// This guarantees SENSOR_SPI will use SPI3, as expected.
#define SENSOR_SCK PB3_ALT1
#define SENSOR_MISO PB4_ALT1  
#define SENSOR_MOSI PB5_ALT1

// Sensor CS
#define SENSOR_CS PA15
#define LSM6DS_CS PA15
#define DPS310_CS PA15
#define BMI088_Accel_CS PA15
#define BMI088_Gyro_CS PA2

// SPI bus (SPI2)
#define INTERBOARD_CS PB12 // Dummy
#define INTERBOARD_SCK PB13
#define INTERBOARD_MISO PB14
#define INTERBOARD_MOSI PB15

// SPI bus (SPI1)
#define FLASH_CS PA4
#define FLASH_SCK PA5
#define FLASH_MISO PA6
#define FLASH_MOSI PA7

// I2C sensors (I2C1)
#define SENSOR_SCL PB6
#define SENSOR_SDA PB7
