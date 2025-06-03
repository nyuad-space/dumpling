// #include "main.h"
// #include "flash_format.h"
// #include "Arduino.h"

// // Include the C-style Elm-Cha FatFs headers
// extern "C"
// {
// #include "ff.h"
// #include "diskio.h"
// #include "ffconf.h"
// #include "flash_config.h"
// }

// #define DISK_LABEL "EXT FLASH"

// static FATFS elmchamFatfs;

// void format_fat12(Adafruit_SPIFlash &flash)
// {
// #ifdef __AVR__
//     uint8_t workbuf[512];
// #else
//     uint8_t workbuf[4096];
// #endif

//     FRESULT r = f_mkfs("", FM_FAT, 0, workbuf, sizeof(workbuf));
//     if (r != FR_OK)
//     {
//         Serial.print(F("f_mkfs failed with error: "));
//         Serial.println(r);
//         while (1)
//             yield();
//     }

//     r = f_mount(&elmchamFatfs, "0:", 1);
//     if (r != FR_OK)
//     {
//         Serial.print(F("f_mount failed with error: "));
//         Serial.println(r);
//         while (1)
//             yield();
//     }

//     Serial.println(F("Setting disk label..."));
//     r = f_setlabel(DISK_LABEL);
//     if (r != FR_OK)
//     {
//         Serial.print(F("f_setlabel failed with error: "));
//         Serial.println(r);
//         while (1)
//             yield();
//     }

//     f_unmount("0:");
//     flash.syncBlocks();

//     Serial.println(F("Formatted flash!"));
// }

// void formatFlash(Adafruit_SPIFlash &flash, FatVolume &fatfs)
// {
//     Serial.println(F("Initializing flash..."));
//     if (!flash.begin())
//     {
//         Serial.println(F("Flash init failed!"));
//         while (1)
//             yield();
//     }

//     Serial.print(F("Flash JEDEC ID: 0x"));
//     Serial.println(flash.getJEDECID(), HEX);
//     Serial.print(F("Size: "));
//     Serial.print(flash.size() / 1024);
//     Serial.println(F(" KB"));

//     Serial.setTimeout(30000);
//     while (!Serial.find((char *)"OK"))
//     {
//         Serial.println(F("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
//         Serial.println(F("This will ERASE ALL DATA on the flash chip."));
//         Serial.println(F("Type OK and press Enter to continue."));
//         Serial.println(F("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
//     }

//     format_fat12(flash);

//     if (!fatfs.begin(&flash))
//     {
//         Serial.println(F("Failed to mount formatted filesystem."));
//         while (1)
//             delay(1);
//     }

//     Serial.println(F("Flash chip successfully formatted and mounted."));
// }
