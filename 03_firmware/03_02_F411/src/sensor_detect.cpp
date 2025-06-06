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
Adafruit_BusIO_Register regID_lis2mdl = Adafruit_BusIO_Register(&i2c_lis2mdl, LIS2_WHOAMI_ADDR);

Adafruit_I2CDevice i2c_hdc302 = Adafruit_I2CDevice(HDC302_I2C_ADDR, &SENSOR_I2C);
Adafruit_BusIO_Register regID_hdc302 = Adafruit_BusIO_Register(&i2c_hdc302, HDC302_WHOAMI_ADDR);

String detectSensor()
{
    adafruit_spi_device.begin();
    SENSOR_I2C.begin();

    uint8_t id = 0;

    // == SPI sensors ==
    if (regID_lsm6ds.read(&id) && id == LSM6DS_WHOAMI)
    {
        detectedSensor = SENSOR_LSM6DSO32;
        return "LSM6DSO32";
    }

    else if (regID_dps310.read(&id) && id == DPS310_WHOAMI)
    {
        detectedSensor = SENSOR_DPS310;
        return "DPS310";
    }

    else if (regID_bmi.read(&id) && id == BMI_WHOAMI)
    {
        detectedSensor = SENSOR_Bmi088Accel;
        return "BMI088";
    }

    // == I2C sensors ==
    // if (i2c_bmp390.begin_I2C(BMP390_I2C_ADDR, &SENSOR_I2C))
    //{
    else if (regID_bmp390.read(&id) && id == BMP390_WHOAMI)
    {
        detectedSensor = SENSOR_BMP3xx;
        return "BMP390";
    }
    //}
    // if (i2c_lis2mdl.begin_I2C(LISMDL_I2C_ADDR, &SENSOR_I2C))
    //{
    else if (regID_lis2mdl.read(&id) && id == LIS2MDL_WHOAMI)
    {
        detectedSensor = SENSOR_LIS2MDL;
        return "LIS2MDLTR";
    }
    //}

    //
    // else if (regID_hdc302.read(&id))
    // {
    //     Serial.println(id);

    //     if (id == HDC302_WHOAMI)
    //     {
    //         detectedSensor = SENSOR_HDC302;
    //         return "HDC3022x";
    //     }
    // }

    else
    {
        detectedSensor = SENSOR_UNKNOWN;
        return "unknown";
    }
}