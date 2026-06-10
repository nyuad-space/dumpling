#include <Arduino.h>
#include "sensors/lsm6dso.h"
#include "flash_write.h"
#include "global.h"

// Hardware modules
LSM6DSOnode IMUnode;
FlashLogger flashLogger;

// Data sampling
unsigned long lastSampleMs = 0;
uint32_t sampleCount = 0;

// Flash sanity check
uint32_t flashWriteOkCount = 0;
uint32_t flashWriteFailCount = 0;

void setup()
{
  Serial.begin(115200);
  delay(1500); // for USB serial
  Serial.println();
  Serial.println("dumpling F411 - LSM6DSO32 local pipeline");

  // Initialize sensor comm
  if (SENSOR_READ_MODE)
  {
    if (!IMUnode.begin())
    {
      Serial.println("ERROR: failed to initialize LSM6DSO32 over SPI");
      while (true)
        delay(1000);
    }
    Serial.println("LSM6DSO32 initilaized");
  }

  // Initialize flash logger
  if (FLASH_LOG_MODE || FLASH_DUMP_MODE)
  {
    if (!flashLogger.begin())
    {
      Serial.println("WARNING: flash logger init failed.");
      while (true)
        delay(1000);
    }
    Serial.println("Flash logger initialized.");
    flashLogger.printStatus(Serial);
  }

  // Flash read-only
  if (!SENSOR_READ_MODE && FLASH_DUMP_MODE)
  {
    flashLogger.dumpLogToSerial(Serial);
  }
}

void loop()
{
  // Flash read-only
  if (!SENSOR_READ_MODE)
  {
    delay(1000);
    return;
  }

  // Maintain sample interval
  const unsigned long now = millis();
  if (now - lastSampleMs < SAMPLE_INTERVAL_MS)
    return;
  lastSampleMs = now;

  // Log sensor data
  LSM6DSOsample sample;

  if (IMUnode.readSample(sample))
  {
    sampleCount++;

    if (SERIAL_MONITOR_MODE)
      IMUnode.printCsvSample(Serial, sample);

    if (FLASH_LOG_MODE)
    {
      if (flashLogger.append(sample))
        flashWriteOkCount++;
      else
        flashWriteFailCount++;
    }
  }

  // Periodic heartbeat check
  static unsigned long lastHeartbeatMs = 0;
  if (millis() - lastHeartbeatMs >= 1000)
  {
    lastHeartbeatMs = millis();
    Serial.print("# heartbeat sample_count=");
    Serial.print(sampleCount);
    Serial.print(" flash_ok=");
    Serial.print(flashWriteOkCount);
    Serial.print(" flash fail=");
    Serial.println(flashWriteFailCount);
  }
}
