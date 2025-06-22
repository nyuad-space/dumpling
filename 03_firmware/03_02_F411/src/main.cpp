#include "main.h"

void setup()
{

    // ** Run SdFat_format to reset flash to clearn state when actual flight **

#if F411_DEBUG_MODE
    Serial.begin(115200);
    delay(2000);

#endif

    // Setup logging interrupts pins
    pinMode(LOG_TRIGGER_GPIO, INPUT);
    attachInterrupt(digitalPinToInterrupt(LOG_TRIGGER_GPIO), LOG_TRIGGER_ISR, RISING);

    logging_allowed = true;
    logging_circular = true;
    entered_circular_mode = false;
    entered_regular_mode = false;

    // Clear LED
    neopixel.begin();
    neopixel.clear();
    neopixel.show();
    neopixel.setBrightness(30); // Dim it down!

    // Discover and configure sensor
    String s = detectSensor();
    logStatus("Connected to: %s", s.c_str());
#if F411_DEBUG_MODE
    Serial.print("Connected to: ");
    Serial.println(s);
#endif

    // Catch bad initialization
    success_flag_1 = initSensorComm(detectedSensor);
    logStatus("Sensor communication set.");
#if F411_DEBUG_MODE
    Serial.println("Sensor communication set.");
#endif

    configSensor(detectedSensor);
    logStatus("Sensor configured.");

#if F411_DEBUG_MODE
    Serial.println("Sensor configured.");
#endif

    // Setup flash
    regularStorageFull = false; // Storage status tracking
    success_flag_2 = flash_memory.begin();
    success_flag_3 = initFlashWrite();

    // Create headers and open file
    initFilesForSensor(detectedSensor);
    // Create file for logging
    initFileForStatus();

    // Access all file info with this object
    SensorFileInfo info = getSensorFileInfo(detectedSensor);

#if F411_DEBUG_MODE
    uint32_t jedec_id = flash_memory.getJEDECID();
    Serial.print("JEDEC ID: 0x");
    Serial.println(jedec_id, HEX);
    Serial.print("Flash size (usable): ");
    Serial.print(flash_memory.size() / 1024);
    Serial.println(" KB");
    Serial.print("Circular file name: ");
    Serial.println(info.circularName);
    Serial.print("Circular file size (bytes): ");
    getFileSize(info.circularName);
    Serial.print("Regular file name: ");
    Serial.println(info.regularName);
    Serial.print("Regular file size (bytes): ");
    getFileSize(info.regularName);
    Serial.print("\n");
#endif
    logStatus("JEDEC ID: 0x%X", jedec_id);
    logStatus("Flash size (usable): %u KB", flash_memory.size() / 1024);
    logStatus("Circular file name: %s", info.circularName);
    logStatus("Circular file size (bytes): %u", getFileSize(info.circularName));
    logStatus("Regular file name: %s", info.regularName);
    logStatus("Regular file size (bytes): %u", getFileSize(info.regularName));

    if (!success_flag_1 || !success_flag_2 || !success_flag_2 || detectedSensor == SENSOR_UNKNOWN)
    {
#if F411_DEBUG_MODE
        Serial.println("Sensor init failed!");
#endif
        while (1)
        {
            _blink_red();
        }
    }

// READ FROM FLASH MODE
#if READ_FROM_FLASH
    // CIRCUALAR
    // Re-open the file for reading
    File32 circFile = fatfs.open(info.circularName, FILE_READ);

    // Display what is inside files
    if (circFile)
    {
#if F411_DEBUG_MODE
        Serial.print("Circular file size: ");
        Serial.println(circFile.size());
        Serial.println("Reading content of circular file:");
#endif

        // read from the file until there's nothing else in it
        while (circFile.available())
        {
            Serial.write(circFile.read());
        }
        // close the file
        circFile.close();
    }
    else
    {
#if F411_DEBUG_MODE
        Serial.println("error opening circular file");
#endif
    }

    // REGULAR
    File32 regFile = fatfs.open(info.regularName, FILE_READ);
    if (regFile)
    {
#if F411_DEBUG_MODE
        Serial.println("\nReading content of regular file:");
#endif

        // read from the file until there's nothing else in it
        while (regFile.available())
        {
            Serial.write(regFile.read());
        }
#if F411_DEBUG_MODE
        Serial.println("--- End of regular file reached ---");
#endif
        // close the file
        regFile.close();
    }
    else
    {
#if F411_DEBUG_MODE
        Serial.println("error opening regular file");
#endif
    }
    // STATUS
    File32 statFile = fatfs.open("status.txt", FILE_READ);
    if (statFile)
    {
#if F411_DEBUG_MODE
        Serial.println("\n== Reading content of status file == \n");
#endif

        // read from the file until there's nothing else in it
        while (statFile.available())
        {
            Serial.write(statFile.read());
        }
#if F411_DEBUG_MODE
        Serial.println("--- End of status file reached ---");
#endif
        // close the file
        statFile.close();
    }
    else
    {
#if F411_DEBUG_MODE
        Serial.println("error opening status file");
#endif
    }

#endif
}

void loop()
{
    // Tracking status flush
    static unsigned long lastStatusFlush = 0;
    static int flushCounter = 0;
    static bool final_status_logged = false;
    static bool was_circular = false;

    // Handle circular mode entry
    if (entered_circular_mode)
    {
        entered_circular_mode = false; // Clear flag
        // logStatusTimed(millis(), "High pin. Logging in circular file.");
    }

    // Handle regular mode entry (final message)
    if (entered_regular_mode)
    {
        entered_regular_mode = false; // Clear flag
        logStatusTimed(millis(), "Low pin. Logging in main file.");
        flushStatusToFile();
        final_status_logged = true;
    }

// WRITE TO FLASH MODE
#if WRITE_TO_FLASH
    // Only log when allowed and regular storage has space
    if (logging_allowed && !regularStorageFull)
    {
        // Read sensor + Write to flash in circular/regular buffer
        readSensor(detectedSensor, logging_circular);

        // Periodic flushes until final message
        if (!final_status_logged && logging_circular)
        {
            // Use the delay period for occasional status flushing (circular mode only)
            flushCounter++;
            if (flushCounter >= 1200) // Every ~1 minute
            {
                unsigned long flushStart = millis();
                flushStatusToFile();
                unsigned long flushTime = millis() - flushStart;

                // Adjust remaining delay to maintain timing
                int remainingDelay = 50 - flushTime;
                if (remainingDelay > 0)
                {
                    delay(remainingDelay);
                }

                flushCounter = 0;
            }
            else
            {
                delay(50); // Normal delay
            }
        }
        else
        {
            delay(50); // Normal delay, no more status logging
        }
        // TODO: LED indication for circular/ regular logging etc.
    }
#endif
}

void LOG_TRIGGER_ISR()
{
    // Read current pin state to determine edge direction
    int pinState = digitalRead(LOG_TRIGGER_GPIO);

    if (pinState == HIGH)
    {
        // Rising edge detected, start logging in main
        logging_allowed = true;
        logging_circular = false;
        entered_regular_mode = true;
        neopixel.setPixelColor(0, color_green);
        neopixel.show();
    }
    else if (pinState == LOW)
    {
        // Logging in circular
        logging_allowed = true;
        logging_circular = true;
        entered_circular_mode = true;
        neopixel.setPixelColor(0, color_amber);
        neopixel.show();
    }
}

void _blink_red()
{
    static unsigned long lastBlinkTime = 0;
    static bool ledState = false;
    const unsigned long blinkInterval = 500; // 500ms on/off cycle

    unsigned long currentTime = millis();

    if (currentTime - lastBlinkTime >= blinkInterval)
    {
        lastBlinkTime = currentTime;
        ledState = !ledState;

        if (ledState)
        {
            neopixel.setPixelColor(0, color_amber);
        }
        else
        {
            neopixel.clear();
        }
        neopixel.show();
    }
}