#ifndef PARSER_H
#define PARSER_H

#include "common.h"
#include "syntaxfactory.h"
#include "lexer.h"
#include "stringtable.h"

typedef struct {
    SyntaxFactory *factory;
    
    Token       *tokens;
    token_index n_tokens;
    token_index current;

    bool has_error;
} Parser;

Parser *make_parser();
void destroy_parser(Parser *parser);
void parse_lex(Parser *parser, LexResult *lex, const char *file_path);

#endif