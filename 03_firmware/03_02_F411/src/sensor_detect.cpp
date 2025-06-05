#include "sensor_detect.h"

// Set up SPI between sensors and F411
Adafruit_SPIDevice spi_lsm6ds = Adafruit_SPIDevice(LSM6DS_CS, 10000, SPI_BITORDER_MSBFIRST, SPI_MODE0, &SENSOR_SPI);
// Points to WHOAMI register of sensor
Adafruit_BusIO_Register regID_lsm6ds = Adafruit_BusIO_Register(&spi_lsm6ds, LSM6DS_WHOAMI_ADDR, ADDRBIT8_HIGH_TOREAD);

Adafruit_SPIDevice spi_dps310 = Adafruit_SPIDevice(DPS310_CS, 10000, SPI_BITORDER_MSBFIRST, SPI_MODE0, &SENSOR_SPI);
Adafruit_BusIO_Register regID_dps310 = Adafruit_BusIO_Register(&spi_dps310, DPS310_WHOAMI_ADDR, ADDRBIT8_HIGH_TOREAD);

Adafruit_SPIDevice spi_bmi = Adafruit_SPIDevice(bmiAccel_CS, 10000, SPI_BITORDER_MSBFIRST, SPI_MODE0, &SENSOR_SPI);
Adafruit_BusIO_Register regID_bmi = Adafruit_BusIO_Register(&spi_bmi, BMI_WHOAMI_ADDR, ADDRBIT8_HIGH_TOREAD);

String detectSensor()
{
    SENSOR_SPI.begin();
    SENSOR_I2C.begin();

    uint8_t id = 0;

    if (spi_lsm6ds.begin())
    {
        if (regID_lsm6ds.read(&id) && id == LSM6DS_WHOAMI)
        {
            detectedSensor = SENSOR_LSM6DSO32;
            return "LSM6DSO32";
        }
    }

    if (spi_dps310.begin())
    {
        if (regID_dps310.read(&id) && id == DPS310_WHOAMI)
        {
            detectedSensor = SENSOR_DPS310;
            return "DPS310";
        }
    }
    if (spi_bmi.begin())
    {
        if (regID_bmi.read(&id) && id == BMI_WHOAMI)
        {
            detectedSensor = SENSOR_Bmi088Accel;
            return "BMI088";
        }
    }

    // ADD else if for I2Cs
    else
    {
        detectedSensor = SENSOR_UNKNOWN;
        return "unknown";
    }
}

// I2C
// else if (bmp3xx.begin_I2C(0x76, &SENSOR_I2C))
// {
//     detectedSensor = SENSOR_BMP3xx;
// }
// else if (lis2mdl.begin(0x1E, &SENSOR_I2C))
// {
//     detectedSensor = SENSOR_LIS2MDL;
// }
// else if (hdc.begin(0x44, &SENSOR_I2C))
// {
//     detectedSensor = SENSOR_HDC302x;
// }