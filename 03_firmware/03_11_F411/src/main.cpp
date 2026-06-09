#include <Arduino.h>
#include "sensors/lsm6dso.h"

LSM6DSOnode IMUnode;

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
  IMUnode.printCsvHeader(Serial);
}

void loop()
{
  LSM6DSOsample sample;
  if (IMUnode.readSample(sample))
    IMUnode.printCsvSample(Serial, sample);
}
