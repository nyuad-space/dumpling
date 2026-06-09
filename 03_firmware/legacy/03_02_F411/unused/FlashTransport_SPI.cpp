#include "main.h"
#include "SPI.h"
#include "SdFat_Adafruit_Fork.h"
#include <Adafruit_SPIFlash.h>
#include "flash_config.h"

Adafruit_FlashTransport_SPI flashTransport(FLASH_CS, &SPI);
Adafruit_SPIFlash flash(&flashTransport);

#define FLASH_CS PA4
#define FLASH_SCK PA5
#define FLASH_MISO PB4
#define FLASH_MOSI PA7

// int RTC_init()
// {
//     pinMode(FLASH_CS, OUTPUT);
//     SPI.begin();
//     SPI.setBitOrder(MSBFIRST);
//     SPI.setDataMode(SPI_MODE1);
//     digitalWrite(FLASH_CS, LOW); // start communication
//     SPI.transfer(0x8E);
//     SPI.transfer(0x60);
//     digitalWrite(FLASH_CS, HIGH); // end communication
// }
// the setup function runs once when you press reset or power the board
void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
        delay(100); // wait for native usb
    }

    Serial.println("Adafruit Serial Flash Info example");
    flash.begin();

    // Using a flash device not already listed? Start the flash memory by passing
    // it the array of device settings defined above, and the number of elements
    // in the array.
    // flash.begin(my_flash_devices, flashDevices);

    uint32_t jedec_id = flash.getJEDECID();
    Serial.print("JEDEC ID: 0x");
    Serial.println(jedec_id, HEX);
    Serial.print("Flash size (usable): ");
    Serial.print(flash.size() / 1024);
    Serial.println(" KB");

#ifdef ARDUINO_ARCH_RP2040
    // For rp2 since flash device is also used for storing code, the flash.size()
    // only return usable flash size for data which is dictated by e.g Menu->Flash
    // Size -> 2MB (Sketch 1920KB, FS 64KB) --> size = 64KB For reference purpose,
    // we only try to find and print actual flash device size using JEDEC here
    Serial.print("Flash size (raw): ");
    Serial.print(1 << ((jedec_id & 0xff) - 10));
    Serial.println(" KB");
#endif
}

void loop()
{
    // nothing to do
}
