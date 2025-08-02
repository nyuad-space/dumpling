#include "globals.h"
#include "sensor_detect.h"

String detectSensor()
{
    String sensor_name;
    uint16_t sensor_id = 0xFFFF;

    Adafruit_SPIDevice spi_device(SENSOR_CS, 10000, SPI_BITORDER_MSBFIRST, SPI_MODE0, &SENSOR_SPI);
    
    if (!spi_device.begin()) {
        return "SPI initialization failed";
    }
    SENSOR_I2C.begin();
    
    // SPI register objects 
    Adafruit_BusIO_Register regID_lsm6ds(&spi_device, LSM6DS_WHOAMI_ADDR, ADDRBIT8_HIGH_TOREAD);
    Adafruit_BusIO_Register regID_dps310(&spi_device, DPS310_WHOAMI_ADDR, ADDRBIT8_HIGH_TOREAD);
    Adafruit_BusIO_Register regID_bmi(&spi_device, BMI_WHOAMI_ADDR, ADDRBIT8_HIGH_TOREAD);
    
    // I2C register objects
    Adafruit_I2CDevice i2c_bmp390(BMP390_I2C_ADDR, &SENSOR_I2C);
    Adafruit_BusIO_Register regID_bmp390(&i2c_bmp390, BMP390_WHOAMI_ADDR);
    Adafruit_I2CDevice i2c_lis2mdl(LIS2MDL_I2C_ADDR, &SENSOR_I2C);
    Adafruit_BusIO_Register regID_lis2mdl(&i2c_lis2mdl, LIS2MDL_WHOAMI_ADDR);
    Adafruit_I2CDevice i2c_hdc302(HDC302_I2C_ADDR, &SENSOR_I2C);
    Adafruit_BusIO_Register regID_hdc302(
        &i2c_hdc302,
        HDC302_WHOAMI_ADDR, // reg_addr: this will be split as addrbuffer[0]=0x37, addrbuffer[1]=0x81
        3,                  // width: read 3 bytes (2 data + 1 CRC)
        MSBFIRST,           // byteorder: MSB first for the name data
        2                   // address_width: send 2 bytes for the command
    );
    
    // == SPI sensors ==
    if (regID_lsm6ds.read(&sensor_id) && (sensor_id & 0xFF) == LSM6DS_WHOAMI_VAL)
    {
        detectedSensor = SENSOR_LSM6DS_ACCEL_GYRO;
        sensor_name = "LSM6DSO32";
    }
    else if (regID_dps310.read(&sensor_id) && (sensor_id & 0xFF) == DPS310_WHOAMI_VAL)
    {
        detectedSensor = SENSOR_DPS310_BARO_TEMP;
        sensor_name = "DPS310";
    }
    // Shift first byte (ID) down to second (empty)
    // Then mask first byte
    else if (regID_bmi.read(&sensor_id) && ((sensor_id >> 8) & 0xFF) == BMI_WHOAMI_VAL)
    {
        detectedSensor = SENSOR_BMI088_ACCEL;
        sensor_name = "BMI088";
    }
    
    // == I2C sensors ==
    else if (regID_bmp390.read(&sensor_id) && (sensor_id & 0xFF) == BMP390_WHOAMI_VAL)
    {
        detectedSensor = SENSOR_BMP390_BARO;
        sensor_name = "BMP390";
    }
    else if (regID_lis2mdl.read(&sensor_id) && (sensor_id & 0xFF) == LIS2MDL_WHOAMI_VAL)
    {
        detectedSensor = SENSOR_LIS2MDL_MAG;
        sensor_name = "LIS2MDLTR";
    }
    else if (regID_hdc302.read(&sensor_id) && ((sensor_id >> 8) & 0xFF) == HDC302_WHOAMI_VAL)
    {
        detectedSensor = SENSOR_HDC302_TEMP_HUM;
        sensor_name = "HDC302";
    }
    else
    {
        detectedSensor = SENSOR_UNKNOWN;
        sensor_name = "unknown";
    }
    
    SENSOR_I2C.end();
    return sensor_name;
}