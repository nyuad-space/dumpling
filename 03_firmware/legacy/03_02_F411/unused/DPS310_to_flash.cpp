#include "main.h"
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_DPS310.h>
#include <SPIFlash.h>

// rid of LED, make more concise

// === DPS310 CONFIG ===
#define DPS310_CS PA15
#define DPS310_SCK PB3
#define DPS310_MISO PB4
#define DPS310_MOSI PB5

Adafruit_DPS310 dps;

// === FLASH CONFIG ===
#define FLASH_SS PA4 // Chip Select for SPI Flash (Winbond W25Q16 for example)
#define FLASH_SCK PA5
#define FLASH_MISO PB4
#define FLASH_MOSI PA7

SPIFlash flash(FLASH_SS, 0xEF30); // Change JEDEC ID to 0xEF40 for 16Mbit chips
uint32_t writeAddress = 0;        // Start writing at 0

// LED blink config
int LEDTIME = 500;

// Data struct for flash
struct SensorData
{
    float temperature;
    float pressure;
    uint32_t timestamp;
};

void setup()
{
    Serial.begin(115200);
    Serial.println("I'm Alive!");

    while (!Serial)
        delay(10);

    // === FLASH INIT ===
    flash.wakeup();
    if (flash.initialize())
    {
        Serial.println("SPI Flash Init OK!");
        LEDTIME = 1000; // Blink slow
    }
    else
    {
        Serial.println("SPI Flash Init FAIL! (is chip connected?)");
        LEDTIME = 100; // Blink fast
    }

    // === SENSOR INIT ===
    Serial.println("DPS310 init...");
    if (!dps.begin_SPI(DPS310_CS, DPS310_SCK, DPS310_MISO, DPS310_MOSI))
    {
        Serial.println("Failed to find DPS310 sensor");
        while (1)
            yield();
    }
    Serial.println("DPS310 OK");

    dps.configurePressure(DPS310_64HZ, DPS310_64SAMPLES);
    dps.configureTemperature(DPS310_64HZ, DPS310_64SAMPLES);
}

void loop()
{
    sensors_event_t temp_event, pressure_event;

    // Check if data is ready
    if (!dps.temperatureAvailable() || !dps.pressureAvailable())
    {
        delay(100);
        return;
    }

    dps.getEvents(&temp_event, &pressure_event);

    // Log to Serial
    Serial.print("Temp: ");
    Serial.print(temp_event.temperature);
    Serial.print(" *C | ");

    Serial.print("Pressure: ");
    Serial.print(pressure_event.pressure);
    Serial.println(" hPa");

    // Create data struct
    SensorData data;
    data.temperature = temp_event.temperature;
    data.pressure = pressure_event.pressure;
    data.timestamp = millis();

    // Write to flash
    flash.writeBytes(writeAddress, (uint8_t *)&data, sizeof(SensorData));
    Serial.print("Wrote to flash at address 0x");
    Serial.println(writeAddress, HEX);

    // Read back to verify
    SensorData verify;
    flash.readBytes(writeAddress, (uint8_t *)&verify, sizeof(SensorData));

    if (verify.temperature == data.temperature && verify.pressure == data.pressure)
    {
        Serial.println("Write verified!");
    }
    else
    {
        Serial.println("Verification failed.");
    }

    writeAddress += sizeof(SensorData);

    // Blink LED
    digitalWrite(LED_BUILTIN, HIGH);
    delay(LEDTIME / 2);
    digitalWrite(LED_BUILTIN, LOW);
    delay(LEDTIME / 2);
}
