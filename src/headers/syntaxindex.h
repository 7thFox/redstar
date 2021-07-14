#ifndef SYNTAXINDEX_H
#define SYNTAXINDEX_H

#include "common.h"

typedef struct{uint32_t i;} StringIndex;

// 1-based index so we can test if (my_syntax_index)
typedef struct{uint16_t i;} SyntaxIndex;

// index to _TypedIndex
typedef struct{uint16_t i;} StatementIndex;
typedef struct{uint16_t i;} ExpressionIndex;

#define EMPTY_SYNTAX_INDEX (SyntaxIndex){0}

typedef struct {
    uint8_t     *array;
    size_t      elem_size;
    uint16_t    size;
    uint16_t    capacity;
#define SYNTAX_ARRAY_MAX_CAPACITY 0xFFFF
#define SYNTAX_ARRAY_MAX_GROW 64
#define get_ast_node(index, arr) ((void*)(arr.array + (arr.elem_size * (index.i - 1))))
} SyntaxArray;

#endif
