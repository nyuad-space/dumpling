#include <Arduino.h>
#include "sensors/lsm6dso.h"
#include "flash_write.h"
#include "global.h"

LSM6DSOnode IMUnode;
FlashLogger flashLogger;

unsigned long lastSampleMs = 0;
uint32_t sampleCount = 0;

void setup()
{
  Serial.begin(115200);
  delay(1500); // for USB serial
  Serial.println();
  Serial.println("dumpling F411 - LSM6DSO32 local pipeline");

  if (!IMUnode.begin())
  {
    Serial.println("ERROR: failed to initialize LSM6DSO32 over SPI");
    while (true)
      delay(1000);
  }
  Serial.println("LSM6DSO32 initilaized");

  if (FLASH_LOG_MODE)
  {
    if (!flashLogger.begin())
      Serial.println("WARNING: flash logger init failed.");
    else
    {
      Serial.println("Flash logger initialized.");
      flashLogger.printStatus(Serial);
    }
  }

  if (SERIAL_MONITOR_MODE)
  {
    IMUnode.printCsvHeader(Serial);
  }
}

void loop()
{
  const unsigned long now = millis();
  if (now - lastSampleMs < SAMPLE_INTERVAL_MS)
    return;
  lastSampleMs = now;

  LSM6DSOsample sample;
  sampleCount++;

  if (IMUnode.readSample(sample))
  {
    if (SERIAL_MONITOR_MODE)
      IMUnode.printCsvSample(Serial, sample);
  }
}
