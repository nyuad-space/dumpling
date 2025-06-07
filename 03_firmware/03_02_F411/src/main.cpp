#include "main.h"

void setup()
{
    Serial.begin(115200);
    delay(5000); // Give time for power stabilization
}

void loop()
{
    Serial.println("Detecting sensor...");
    Serial.print("Connected to: ");
    Serial.println(detectSensor());
    delay(5000);
}
