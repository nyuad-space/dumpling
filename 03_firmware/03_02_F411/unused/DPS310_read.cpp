#include "main.h"
#include "SPI.h"

#include <Adafruit_DPS310.h>

/** DPS310XTSA1- Barometric pressure and temperature sensor  */
// Read temperature/pressure

Adafruit_DPS310 dps;

#define DPS310_CS PA15
#define DPS310_SCK PB3
#define DPS310_MISO PB4
#define DPS310_MOSI PB5

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        delay(10);

    Serial.println("DPS310");
    if (!dps.begin_SPI(DPS310_CS, DPS310_SCK, DPS310_MISO, DPS310_MOSI))
    {
        Serial.println("Failed to find DPS");
        while (1)
            yield();
    }
    Serial.println("DPS OK!");

    dps.configurePressure(DPS310_128HZ, DPS310_128SAMPLES); // check in higher sampling rate
    dps.configureTemperature(DPS310_64HZ, DPS310_64SAMPLES);
}

void loop()
{
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

    delay(1000);
}
