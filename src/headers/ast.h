#ifndef AST_H
#define AST_H

#include "lexer.h"
#include "common.h"
#include "token.h"
#include "syntaxindex.h"

typedef struct {
    StringIndex filepath;
    SyntaxIndex use_start;
    SyntaxIndex use_end_noninclusive;
} AstCompilationUnit;

typedef struct {
    // uint16_t    file_index;
    uint32_t    line;// I swear to frick, if someone gets past these these, they deserve what they get.
    uint16_t    col;
} Location;

typedef struct {
    StringIndex name;
} AstIdent;

typedef struct {
    SyntaxIndex ident;
    SyntaxIndex block_opt;
} AstAttrDecl;

typedef struct {
    SyntaxIndex referenced_compilation_unit;
} AstUseStatement;

#endif