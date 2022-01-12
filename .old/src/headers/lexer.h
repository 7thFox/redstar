#ifndef LEXER_H
#define LEXER_H

#include "cursor.h"
#include "token.h"
#include "common.h"
#include "stdio.h"
#include <stdlib.h>

typedef uint32_t token_index;

typedef struct {
    char *text;
    Token *tokens;
    token_index n_tokens;
    bool has_error;
} LexResult;

LexResult lex_file(FILE *f, const char *filepath);

#endif