#ifndef DEBUG_H
#define DEBUG_H

#include "common.h"

typedef enum
{
    VERBOSITY_NONE,
    VERBOSITY_LOW,
    VERBOSITY_NORMAL,
    VERBOSITY_HIGH,
} debug_verbosity;

int debugf(debug_verbosity v, const char *fmt, ...);

#endif