#ifndef CURSOR_H
#define CURSOR_H

#include "common.h"

typedef struct {
    uint8_t line;
    uint8_t col;
    uint16_t ind;
} Cursor;

#endif