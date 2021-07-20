#ifndef SYNTAXINDEX_H
#define SYNTAXINDEX_H

#include "common.h"

// 1-based index so we can test if (my_syntax_index)
typedef struct{uint16_t i;} SyntaxIndex;
typedef struct{uint32_t i;} StringIndex;

// index to _TypedIndex
typedef struct{uint16_t i;} StatementIndex;
typedef struct{uint16_t i;} ExpressionIndex;

#define EMPTY_SYNTAX_INDEX (SyntaxIndex){0}

#endif
