#include <Adafruit_DPS310.h>
#include "SdFat_Adafruit_Fork.h"
#include <SPI.h>
#include <Adafruit_SPIFlash.h>
#include "flash_config.h"

// Sensor SPI pin definitions
#define DPS310_CS    PA15
#define DPS310_SCK   PB3
#define DPS310_MISO  PB4
#define DPS310_MOSI  PB5

// Sensor object
Adafruit_DPS310 dps;

// Flash object
Adafruit_SPIFlash flash(&flashTransport);
FatVolume fatfs;
File32 sensorFile;

void printSensorFile() {
  sensorFile = fatfs.open("sensor.txt", FILE_READ);
  if (sensorFile) {
    Serial.println("Contents of sensor.txt:");
    while (sensorFile.available()) {
      Serial.write(sensorFile.read());
    }
    sensorFile.close();
  } else {
    Serial.println("Failed to open sensor.txt for reading.");
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  // Initialize DPS310 sensor
  Serial.println("Initializing DPS310...");
  if (!dps.begin_SPI(DPS310_CS, DPS310_SCK, DPS310_MISO, DPS310_MOSI)) {
    Serial.println("Failed to find DPS310 sensor!");
    while (1) yield();
  }
  Serial.println("DPS310 initialized.");

  dps.configurePressure(DPS310_64HZ, DPS310_64SAMPLES);
  dps.configureTemperature(DPS310_64HZ, DPS310_64SAMPLES);

  // Initialize flash memory
  Serial.println("Initializing flash memory...");
  if (!flash.begin()) {
    Serial.println("Flash init failed!");
    while (1) yield();
  }

  // Mount filesystem
  if (!fatfs.begin(&flash)) {
    Serial.println("Filesystem not found. Please run formatter first.");
    while (1) yield();
  }

  Serial.println("Filesystem mounted.");
  Serial.println("Type 'read' to view log file contents.");
  Serial.println("Waiting for input (5 seconds)...");

  unsigned long start = millis();
  while (millis() - start < 5000) {
    if (Serial.available()) {
      String input = Serial.readStringUntil('\n');
      input.trim(); // Remove trailing newline/whitespace
      if (input.equalsIgnoreCase("read")) {
        printSensorFile();
      } else {
        Serial.println("Unknown command.");
      }
    }
    delay(50);
  }
}

void loop() {
  if (!dps.temperatureAvailable() || !dps.pressureAvailable()) {
    delay(100);
    return;
  }

  sensors_event_t temp_event, pressure_event;
  dps.getEvents(&temp_event, &pressure_event);

  float temperature = temp_event.temperature;
  float pressure = pressure_event.pressure;

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" *C");

  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" hPa");

  // Open file and append data
  sensorFile = fatfs.open("sensor.txt", FILE_WRITE);
  if (sensorFile) {
    sensorFile.print("Temp: ");
    sensorFile.print(temperature);
    sensorFile.print(" *C, Pressure: ");
    sensorFile.print(pressure);
    sensorFile.println(" hPa");
    sensorFile.close();
    Serial.println("Data written to sensor.txt");
  } else {
    Serial.println("Failed to open sensor.txt for writing");
  }

  Serial.println();
  delay(10000); // wait 10 seconds
}
