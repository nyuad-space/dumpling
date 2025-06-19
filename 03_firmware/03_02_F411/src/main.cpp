#include "main.h"
#define F411_DEBUG_MODE 1

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

    // Clear LED
    neopixel.begin();
    neopixel.clear();
    neopixel.show();
    neopixel.setBrightness(30); // Dim it down!

    // Discover and configure sensor
#if F411_DEBUG_MODE
    Serial.print("Connected to: ");
    Serial.println(detectSensor());
#else
    detectSensor();
#endif

    // Catch bad initialization
    success_flag = initSensorComm(detectedSensor);

    configSensor(detectedSensor);

#if F411_DEBUG_MODE
    Serial.println("Sensor configured.");
#endif

    // Setup flash
    regularStorageFull = false; // Storage status tracking
    success_flag = flash_memory.begin();
    success_flag = initFlashWrite();

    // Create headers and open file
    initFilesForSensor(detectedSensor);

    // TODO: earlier, circular is only writing header and not data. check write to circular logic again

    // Access all file info with this object
    SensorFileInfo info = getSensorFileInfo(detectedSensor);

    // Check if header is in
#if F411_DEBUG_MODE
    // Re-open the file for reading
    File32 circFile;
    File32 regFile;
    circFile = fatfs.open(info.circularName);

    // Display what is inside files
    if (circFile)
    {
        Serial.println("Reading content of file:");

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
        Serial.println("error opening circular file");
    }

    regFile = fatfs.open(info.regularName);
    if (regFile)
    {
        Serial.println("Reading content of file:");

        // read from the file until there's nothing else in it
        while (regFile.available())
        {
            Serial.write(regFile.read());
        }
        // close the file
        regFile.close();
    }
    else
    {
        Serial.println("error opening regular file");
    }
#endif

#if F411_DEBUG_MODE
    uint32_t jedec_id = flash_memory.getJEDECID();
    Serial.print("JEDEC ID: 0x");
    Serial.println(jedec_id, HEX);
    Serial.print("Flash size (usable): ");
    Serial.print(flash_memory.size() / 1024);
    Serial.println(" KB");
    Serial.print("Circular file: ");
    Serial.println(info.circularName);
    Serial.print("Circular file size: ");
    getFileSize(info.circularName);
    Serial.print("Regular file: ");
    Serial.println(info.regularName);
    Serial.print("Regular file size: ");
    getFileSize(info.regularName);
    Serial.print("\n");
#endif

    if (!success_flag || detectedSensor == SENSOR_UNKNOWN)
    {
#if F411_DEBUG_MODE
        Serial.println("Sensor init failed!");
#endif
        while (1)
        {
            _blink_red();
        }
    }
}

void loop()
{
    // Only log when allowed and regular storage has space
    if (logging_allowed && !regularStorageFull)
    {
#if F411_DEBUG_MODE
        Serial.println("entered logging mode");
#endif
        // Read sensor + Write to flash in circular/regular buffer
        readSensor(detectedSensor, logging_circular);

        // TODO: LED indication for circular/ regular logging etc.
        // TODO: clean up code & consider edge cases
        // TODO: separate project for reading from flash
    }
}

void LOG_TRIGGER_ISR()
{
    // Read current pin state to determine edge direction
    int pinState = digitalRead(LOG_TRIGGER_GPIO);

    if (pinState == HIGH)
    {
        // Falling edge detected, start logging in circular
#if F411_DEBUG_MODE
        Serial.println("Low pin. Logging in main flash.");
#endif
        logging_allowed = true;
        logging_circular = false;
        neopixel.setPixelColor(0, color_green);
        neopixel.show();
    }
    else if (pinState == LOW)
    {
        // Rising edge detected, start logging in main
#if F411_DEBUG_MODE
        Serial.println("High pin. Logging in circular buffer.");
#endif
        logging_allowed = true;
        logging_circular = true;
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