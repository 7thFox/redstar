#ifndef LEXER_H
#define LEXER_H

#include "cursor.h"
#include "token.h"
#include "common.h"
#include "stdio.h"
#include <stdlib.h>

typedef struct {
    Token *tokens;
    size_t length;
    size_t capacity;
} LexResult;

LexResult lex_file(FILE *f);

#endif