#ifndef AST_H
#define AST_H

#include "lexer.h"
#include "common.h"
#include "stdio.h"
#include <stdlib.h>

typedef struct {
    const char *filename;
} AST_COMPLIATION_UNIT;

int build_compliation_unit(LexResult *lex, const char *filePath);

void print_ast();

#endif