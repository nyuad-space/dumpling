#include "main.h"
#define F411_DEBUG_MODE 1

void setup()
{
    // **IN DEBUG MODE, FLASH IS AUTO-CLEARED**

#if F411_DEBUG_MODE
    Serial.begin(115200);
#endif

    // Setup logging interrupts pins
    pinMode(LOG_TRIGGER_GPIO, INPUT);
    attachInterrupt(digitalPinToInterrupt(LOG_TRIGGER_GPIO), LOG_TRIGGER_ISR, CHANGE);

    logging_allowed = true;
    logging_circular = true;

    // Clear LED
    neopixel.begin();
    neopixel.clear();
    neopixel.show();

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
    // clearFlash(detectedSensor, 1);
    // clearFlash(detectedSensor, 0);
#endif

    // Setup flash
    regularStorageFull = false; // Storage status tracking
    success_flag = flash_memory.begin();
    success_flag = initFlashWrite(1);
    // Create headers and open file
    initFilesForSensor(detectedSensor);

#if F411_DEBUG_MODE
    uint32_t jedec_id = flash_memory.getJEDECID();
    Serial.print("JEDEC ID: 0x");
    Serial.println(jedec_id, HEX);
    Serial.print("Flash size (usable): ");
    Serial.print(flash_memory.size() / 1024);
    Serial.println(" KB");
    Serial.println(getSensorFilename(detectedSensor, true)); // bool circular
    Serial.print("File size: ");
    getFileSize(getSensorFilename(detectedSensor, true)); // bool circular
    Serial.print("\n");
#endif

    if (!success_flag || detectedSensor == SENSOR_UNKNOWN)
    {
        Serial.println("Sensor init failed!");
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
    }
}

void LOG_TRIGGER_ISR()
{
    // Read current pin state to determine edge direction
    int pinState = digitalRead(LOG_TRIGGER_GPIO);

    if (pinState == LOW)
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
    else if (pinState == HIGH)
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