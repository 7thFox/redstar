#ifndef SYNTAXINDEX_H
#define SYNTAXINDEX_H

#include "common.h"

// typedef enum {
//     AST_COMPILIATION_UNIT,
//     AST_USE_STATEMENT,
//     AST_IDENT,
//     AST_ATTR_DECL,

//     AST_ERROR = 0xFFFF,
//     AST_MISSING = 0xFFFE,
// } SyntaxKind;

typedef uint32_t StringIndex;

// 1-based index so we can test if (my_syntax_index)
typedef uint16_t SyntaxIndex;

#define EMPTY_SYNTAX_INDEX (SyntaxIndex){0}

typedef struct {
    uint8_t     *array;
    size_t      elem_size;
    uint16_t    size;
    uint16_t    capacity;
#define SYNTAX_ARRAY_MAX_CAPACITY 0xFFFF
#define SYNTAX_ARRAY_MAX_GROW 64
} SyntaxArray;

#endif
