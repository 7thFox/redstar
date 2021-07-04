#ifndef TOKEN_H
#define TOKEN_H

#include "common.h"
#include "cursor.h"

typedef enum {
    // 0-255: literal ascii
    TOK_USE = 256,
    TOK_USEPATH,
    TOK_ATTR,
    TOK_FUNC,
    TOK_RETURN,// 260
    TOK_IDENT,
    TOK_INC,
    TOK_DEC,
    TOK_IF,
    TOK_EQUALITY,
    TOK_NOT_EQUALS,
    TOK_LESS_EQUALS,
    TOK_GREATER_EQUALS,
    TOK_AND,
    TOK_OR,// 270
    TOK_NUMERIC_LITERAL,

    LEX_ERROR = -99,
} TokenType;

typedef struct {
    TokenType type;
    Cursor p0;
    Cursor p1;
    union {
        char *literal_text;
    };
} Token;

#endif