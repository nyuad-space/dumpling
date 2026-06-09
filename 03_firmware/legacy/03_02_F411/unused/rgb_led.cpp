#include "main.h"
#include <Adafruit_NeoPixel.h>

// NeoPixel setup
int neoPixelPin = LED_PIN;
int numPixels = 1;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, neoPixelPin, NEO_GRB + NEO_KHZ800);

// Struct to hold color data
struct Color
{
    const char *name;
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

// Rainbow colors in VIBGYOR order
Color rainbowColors[] = {
    {"Violet", 148, 0, 211},
    {"Indigo", 75, 0, 130},
    {"Blue", 0, 0, 255},
    {"Green", 0, 255, 0},
    {"Yellow", 255, 255, 0},
    {"Orange", 255, 127, 0},
    {"Red", 255, 0, 0}};

const int numColors = sizeof(rainbowColors) / sizeof(rainbowColors[0]);
const int delayTime = 1000; // time to show each color (in ms)

void setup()
{
    Serial.begin(9600);
    strip.begin();
    strip.show();
    strip.clear();
}

void loop()
{
    for (int i = 0; i < numColors; i++)
    {
        Color current = rainbowColors[i];

        // Set color
        for (int j = 0; j < numPixels; j++)
        {
            strip.setPixelColor(j, current.r, current.g, current.b);
        }
        strip.show();

        // Print message
        Serial.print("RGB ");
        Serial.print(current.name);
        Serial.print(" (");
        Serial.print(current.r);
        Serial.print(", ");
        Serial.print(current.g);
        Serial.print(", ");
        Serial.print(current.b);
        Serial.println(")");

        delay(delayTime);
    }
}