#ifndef FLASHSTATUS_H
#define FLASHSTATUS_H

#include "globals.h"

// Log system status into buffer
void logStatus(const char *format, ...);
void logStatusTimed(unsigned long timestamp, const char *format, ...);

// Initialize filesystem for flash writing
bool initFileForStatus();
// 
void flushStatusToFile();

#endif // FLASHSTATUS_H