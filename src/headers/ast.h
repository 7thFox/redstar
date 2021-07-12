#ifndef AST_H
#define AST_H

#include "lexer.h"
#include "common.h"
#include "token.h"
#include "syntaxindex.h"
#include "syntaxkind.h"

typedef struct {
    SyntaxIndex index;
    SyntaxKind  kind;

} StatementIndex;

typedef struct {
    StringIndex filepath;
    SyntaxIndex block;
} AstCompilationUnit;

typedef struct {
    // uint16_t    file_index;
    uint32_t    line;// I swear to frick, if someone gets past these these, they deserve what they get.
    uint16_t    col;
} Location;

typedef struct {
    SyntaxIndex ident;
    SyntaxIndex param_list_opt;
    SyntaxIndex return_type_opt;
    SyntaxIndex block;
} AstFuncDecl;

typedef struct {
    SyntaxIndex param_start;
    SyntaxIndex param_end_noninclusive;
} AstParameterListDecl;

typedef struct {
    SyntaxIndex ident;
    SyntaxIndex type;
} AstParameterDecl;

typedef struct {
    SyntaxIndex attr_list_opt;
    SyntaxIndex ident;
} AstType;

typedef struct {
    SyntaxArray attributes;// SyntaxIndex -> AstIdent
} AstAttrList;

typedef struct {
    StringIndex name;
} AstIdent;

typedef struct {
    SyntaxIndex parent_block;
    SyntaxArray statements;// StatementIndex
} AstBlock;

typedef struct {
    SyntaxIndex ident;
    SyntaxIndex block_opt;
} AstAttrDecl;

typedef struct {
    SyntaxIndex referenced_compilation_unit;
} AstUseStatement;

#endif