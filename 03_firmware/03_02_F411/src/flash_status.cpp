#include "flash_status.h"
#include <stdarg.h>

#define STATUS_FILE_SIZE (64 * 1024) // 64KB status logging buffer

// Track system status
char statusBuffer[STATUS_FILE_SIZE];
int statusBufferIndex = 0;

// Log system status into buffer
void logStatus(const char *format, ...)
{
    char message[200];

    // Handle variable arguments
    va_list args;
    va_start(args, format);
    vsnprintf(message, sizeof(message), format, args);
    va_end(args);

    // Logging
    int len = strlen(message);
    if (statusBufferIndex + len + 1 < STATUS_FILE_SIZE) // +1 for newline
    {
        strcpy(&statusBuffer[statusBufferIndex], message);
        statusBufferIndex += len;
        statusBuffer[statusBufferIndex++] = '\n';
        statusBuffer[statusBufferIndex] = '\0'; // Null terminate
    }
}

// TIMED
void logStatusTimed(unsigned long timestamp, const char *format, ...)
{
    char message[100];

    // Handle variable arguments
    va_list args;
    va_start(args, format);
    vsnprintf(message, sizeof(message), format, args);
    va_end(args);

    // Create timestamped message
    char timedMessage[150];
    snprintf(timedMessage, sizeof(timedMessage), "%lu,%s", timestamp, message);

    // Logging
    int len = strlen(timedMessage);
    if (statusBufferIndex + len + 1 < STATUS_FILE_SIZE) // +1 for newline
    {
        strcpy(&statusBuffer[statusBufferIndex], timedMessage);
        statusBufferIndex += len;
        statusBuffer[statusBufferIndex++] = '\n';
        statusBuffer[statusBufferIndex] = '\0'; // Null terminate
    }
}

// === TO FLASH ===
// Initialize filesystem for flash writing
bool initFileForStatus()
{
    statusFile = fatfs.open("status.txt", FILE_WRITE);
    if (!statusFile)
    {
#if F411_DEBUG_MODE
        Serial.println("Failed to open status file.");
#endif
        return false;
    }
#if F411_DEBUG_MODE
    Serial.println("Created status file.");
#endif
    statusFile.close();
    return true;
}

// write logStatus to flash
void logStatusToFlash()
{
}