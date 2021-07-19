#ifndef DARRAY_H
#define DARRAY_H

#include "common.h"

typedef struct {
    uint8_t     *array;
    size_t      elem_size;
    uint16_t    size;
    uint16_t    capacity;
#define SYNTAX_ARRAY_MAX_CAPACITY 0xFFFF
#define SYNTAX_ARRAY_MAX_GROW 64
#define get_ast_node(index, arr) ((void*)(arr.array + (arr.elem_size * (index.i - 1))))
} DynamicArray;

DynamicArray init_array(size_t elem_size, uint16_t cap);
void *next_array_elem(DynamicArray *arr);

#endif
