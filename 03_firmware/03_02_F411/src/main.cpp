#include "main.h"
#include <Adafruit_DPS310.h>

#include "SdFat_Adafruit_Fork.h"
#include <SPI.h>
#include <Adafruit_SPIFlash.h>

#include "flash_config.h"

/** DPS310XTSA1- Barometric pressure and temperature sensor  */
// Read temperature/pressure

Adafruit_DPS310 dps;
Adafruit_SPIFlash flash(&flashTransport);

#define DPS310_CS PA15
#define DPS310_SCK PB3
#define DPS310_MISO PB4
#define DPS310_MOSI PB5

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        delay(10);

    // DPS310 config
    Serial.println("DPS310");
    if (!dps.begin_SPI(DPS310_CS, DPS310_SCK, DPS310_MISO, DPS310_MOSI))
    {
        Serial.println("Failed to find DPS");
        while (1)
            yield();
    }
    Serial.println("DPS OK!");

    dps.configurePressure(DPS310_64HZ, DPS310_64SAMPLES); // check in higher sampling rate
    dps.configureTemperature(DPS310_64HZ, DPS310_64SAMPLES);

    // Flash detection
    Serial.println("Adafruit Serial Flash Info example");
    flash.begin();
    Serial.println("bruh");

    uint32_t jedec_id = flash.getJEDECID();
    Serial.print("JEDEC ID: 0x");
    Serial.println(jedec_id, HEX);
    Serial.print("Flash size (usable): ");
    Serial.print(flash.size() / 1024);
    Serial.println(" KB");
}

void loop()
{

    // DPS310 read data
    sensors_event_t temp_event, pressure_event;

    while (!dps.temperatureAvailable() || !dps.pressureAvailable())
    {
        return; // wait until there's something to read
    }

    dps.getEvents(&temp_event, &pressure_event);
    Serial.print(F("Temperature = "));
    Serial.print(temp_event.temperature);
    Serial.println(" *C");

    Serial.print(F("Pressure = "));
    Serial.print(pressure_event.pressure);
    Serial.println(" hPa");

    Serial.println();

    delay(500);
}
