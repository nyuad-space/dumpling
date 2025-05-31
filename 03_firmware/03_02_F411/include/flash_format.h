#ifndef FLASH_FORMAT_H
#define FLASH_FORMAT_H

#include <Adafruit_SPIFlash.h>
#include <SdFat_Adafruit_Fork.h>

// Function to call from main.cpp
void formatFlash(Adafruit_SPIFlash &flash, FatVolume &fatfs);

#endif