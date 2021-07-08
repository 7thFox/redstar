#include "headers/debug.h"

#include <stdarg.h>
#include "stdio.h"

#define DEBUG 1
#define VERBOSITY VERBOSITY_NONE

int debugf(debug_verbosity v, const char *fmt, ...) {
#if DEBUG
    if (v <= VERBOSITY) {
        int r;
        va_list va;

        va_start(va, fmt);
        r = vprintf(fmt, va);
        va_end(va);
        
        return r;
    }
#endif

    return 0;
}