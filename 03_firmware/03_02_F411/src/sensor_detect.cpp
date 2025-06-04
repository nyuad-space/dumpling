#include "sensor_detect.h"

Adafruit_SPIDevice spi_lsm6ds = Adafruit_SPIDevice(LSM6DS_CS, 10000, SPI_BITORDER_MSBFIRST, SPI_MODE0, &SENSOR_SPI);
Adafruit_BusIO_Register regID_lsm6ds = Adafruit_BusIO_Register(&spi_lsm6ds, LSM6DS_WHOAMI_ADDR, ADDRBIT8_HIGH_TOREAD);

Adafruit_SPIDevice spi_dps310 = Adafruit_SPIDevice(DPS310_CS, 10000, SPI_BITORDER_MSBFIRST, SPI_MODE0, &SENSOR_SPI);
Adafruit_BusIO_Register regID_dps310 = Adafruit_BusIO_Register(&spi_dps310, DPS310_WHOAMI_ADDR, ADDRBIT8_HIGH_TOREAD);

Adafruit_SPIDevice spi_bmi = Adafruit_SPIDevice(bmiAccel_CS, 10000, SPI_BITORDER_MSBFIRST, SPI_MODE0, &SENSOR_SPI);
Adafruit_BusIO_Register regID_bmi = Adafruit_BusIO_Register(&spi_bmi, BMI_WHOAMI_ADDR, ADDRBIT8_HIGH_TOREAD);

String detectSensor()
{
    SENSOR_I2C.begin();

    if (regID_lsm6ds.read() == LSM6DS_WHOAMI)
    {
        SENSOR_SPI.begin(LSM6DS_CS);
        // lsm6ds.begin_SPI(LSM6DS_CS, SENSOR_SCK, SENSOR_MISO, SENSOR_MOSI, 0, 1000000);
        detectedSensor = SENSOR_LSM6DSO32;
        SENSOR_SPI.end();
        return "lsm";
    }

    else if (regID_dps310.read() == DPS310_WHOAMI)
    {
        SENSOR_SPI.begin(DPS310_CS);
        // dps310.begin_SPI(DPS310_CS, &SENSOR_SPI);
        detectedSensor = SENSOR_DPS310;
        SENSOR_SPI.end();
        return "sdf";
    }
    else if (regID_bmi.read() == BMI_WHOAMI)
    {
        SENSOR_SPI.begin(bmiAccel_CS);
        // bmiAccel.begin() && bmiGyro.begin();
        detectedSensor = SENSOR_Bmi088Accel;
        SENSOR_SPI.end();
        return "weweff";
    }

    // ADD else if for I2Cs
    else
    {
        detectedSensor = SENSOR_UNKNOWN;
        Serial.print("Unknown sensor");
        return "sdfsdfa";
    }

    Serial.print("Detected sensor: ");
    Serial.println(detectedSensor);
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