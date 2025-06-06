#include "globals.h"
#include "sensor_detect.h"

// Set up SPI between sensors and F411
Adafruit_SPIDevice adafruit_spi_device = Adafruit_SPIDevice(SENSOR_CS, 10000, SPI_BITORDER_MSBFIRST, SPI_MODE0, &SENSOR_SPI);
// Points to WHOAMI register of sensor
Adafruit_BusIO_Register regID_lsm6ds = Adafruit_BusIO_Register(&adafruit_spi_device, LSM6DS_WHOAMI_ADDR, ADDRBIT8_HIGH_TOREAD);
Adafruit_BusIO_Register regID_dps310 = Adafruit_BusIO_Register(&adafruit_spi_device, DPS310_WHOAMI_ADDR, ADDRBIT8_HIGH_TOREAD);
Adafruit_BusIO_Register regID_bmi = Adafruit_BusIO_Register(&adafruit_spi_device, BMI_WHOAMI_ADDR, ADDRBIT8_HIGH_TOREAD);

// Set up I2C between sensors and F411
Adafruit_I2CDevice i2c_bmp390 = Adafruit_I2CDevice(BMP390_I2C_ADDR, &SENSOR_I2C);
Adafruit_BusIO_Register regID_bmp390 = Adafruit_BusIO_Register(&i2c_bmp390, BMP390_WHOAMI_ADDR);

Adafruit_I2CDevice i2c_lis2mdl = Adafruit_I2CDevice(LISMDL_I2C_ADDR, &SENSOR_I2C);
Adafruit_BusIO_Register regID_lis2mdl = Adafruit_BusIO_Register(&i2c_lis2mdl, LIS2MDL_WHOAMI_ADDR);

Adafruit_I2CDevice i2c_hdc302 = Adafruit_I2CDevice(HDC302_I2C_ADDR, &SENSOR_I2C);
Adafruit_BusIO_Register regID_hdc302 = Adafruit_BusIO_Register(
    &i2c_hdc302, 
    HDC302_WHOAMI_ADDR, // reg_addr: this will be split as addrbuffer[0]=0x37, addrbuffer[1]=0x81
    3,                  // width: read 3 bytes (2 data + 1 CRC)
    MSBFIRST,           // byteorder: MSB first for the returned data
    2                   // address_width: send 2 bytes for the command
);

String detectSensor()
{
    // SENSOR_SPI.begin();
    adafruit_spi_device.begin();
    SENSOR_I2C.begin();

    uint16_t id = 0xFFFF;

    // == SPI sensors ==
    if (regID_lsm6ds.read(&id) && id == LSM6DS_WHOAMI_VAL)
    {
        detectedSensor = SENSOR_LSM6DSO32;
        return "LSM6DSO32";
    }
    else if (regID_dps310.read(&id) && id == DPS310_WHOAMI_VAL)
    {
        detectedSensor = SENSOR_DPS310;
        return "DPS310";
    }
    
    // Shift first byte (ID) down to second (empty)
    // Then mask first byte
    else if (regID_bmi.read(&id) && ((id >> 8) & 0xFF) == BMI_WHOAMI_VAL)
    {
        detectedSensor = SENSOR_Bmi088Accel;
        return "BMI088";
    }
    // Mask the upper byte
    else if (regID_bmp390.read(&id) && (id & 0xFF) == BMP390_WHOAMI_VAL)
    {
        detectedSensor = SENSOR_BMP3xx;
        return "BMP390";
    }
    else if (regID_lis2mdl.read(&id) && (id & 0xFF) == LIS2MDL_WHOAMI_VAL)
    {
        detectedSensor = SENSOR_LIS2MDL;
        return "LIS2MDLTR";
    }
    else if (regID_hdc302.read(&id) && ((id >> 8) & 0xFF) == HDC302_WHOAMI_VAL)
    {
        detectedSensor = SENSOR_HDC302;
        return "HDC302";
    }
    else
    {
        detectedSensor = SENSOR_UNKNOWN;
        return "unknown";
    }
}