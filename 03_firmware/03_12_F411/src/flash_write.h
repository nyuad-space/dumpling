#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_SPIFlash.h>
#include <SdFat_Adafruit_Fork.h>
#include "sensors/lsm6dso.h"

class FlashLogger
{
public:
    FlashLogger();

    bool beginForLog();    // write-enabled
    bool beginForExport(); // read-only
    bool append(const LSM6DSOsample &sample);
    void printStatus(Stream &out) const; // read-only
    void exportLogToSerial(Stream &out); // called in export mode (flash -> serial)

private:
    SPIClass flashSpi_;                          // SPI bus object
    Adafruit_FlashTransport_SPI flashTransport_; // wrapper for generic SPI and flashSpi
    Adafruit_SPIFlash flash_;                    // Adafruit flash library object
    FatVolume fatfs_;                            // mounted filesystem manager
    File32 file_;                                // one open file handle inside fs

    bool mounted_ = false;
    bool fileReady_ = false;

    static constexpr const char *logPath = "/lsm6dso.csv";
};

// Using SdFat temporarily. Change to raw binary for high-speed logging.