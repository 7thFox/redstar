#ifndef TOKEN_H
#define TOKEN_H

#include "common.h"
#include "cursor.h"

typedef enum {
    TOK_NULL = 0,
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
    TOK_ELSE,
    TOK_EQUALITY,
    TOK_NOT_EQUALS,
    TOK_LESS_EQUALS,
    TOK_GREATER_EQUALS,
    TOK_AND,// 270
    TOK_OR,
    TOK_STRING_LITERAL,
    TOK_NUMERIC_LITERAL,
    TOK_BIND,
    TOK_ANNOTATE,
    TOK_DOUBLE_ARROW,
    TOK_ANY,
    TOK_ALL,
    TOK_NONE,
    // 280

    LEX_ERROR = -99,
} TokenType;

typedef struct {
    TokenType type;
    Cursor p0;
    Cursor p1;
} Token;

#endif