#ifndef FLASHSTATUS_H
#define FLASHSTATUS_H

#include "globals.h"

// Log system status into buffer
void logStatus(const char *format, ...);
void logStatusTimed(unsigned long timestamp, const char *format, ...);

#endif // FLASHSTATUS_H