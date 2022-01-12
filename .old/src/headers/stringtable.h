#ifndef STRINGTABLE_H
#define STRINGTABLE_H

#include "common.h"
#include "syntaxindex.h"
#include "darray.h"

typedef struct {
    const char *value1;
    const char *value2;
    const char *value3;
    const char *value4;
    const char *value5;
    const char *value6;
    const char *value7;
    void *next;
} StringTableBucket;

typedef struct {
    char *strings;
    int size;
    int cap;

    StringTableBucket *lookup;
    DynamicArray collisions;
} StringTable;

StringTable init_string_table();
void destroy_string_table(StringTable *table);
StringIndex copy_string(StringTable *table, const char *str, size_t len);

static inline const char *get_string(StringTable t, StringIndex index) {
    if (!index.i) return "";
    return t.strings + index.i - 1;
}

#endif