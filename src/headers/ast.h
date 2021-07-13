#ifndef AST_H
#define AST_H

#include "lexer.h"
#include "common.h"
#include "token.h"
#include "syntaxindex.h"
#include "syntaxkind.h"

typedef enum {
    BIN_MULTIPLY,
    BIN_DIVIDE,
    BIN_ADD,
    BIN_MINUS,
} BinaryOperationKind;

typedef struct {
    SyntaxIndex index;
    SyntaxKind  kind;

} TypedIndex;

typedef struct {
    StringIndex filepath;
    SyntaxIndex block;
} AstCompilationUnit;

typedef struct {
    SyntaxIndex expression_left;
    BinaryOperationKind operation;
    SyntaxIndex expression_right;
} AstBinaryOperationExpression;

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
    SyntaxArray statements;// TypedIndex (statement)
} AstBlock;

typedef struct {
    SyntaxIndex ident;
    SyntaxIndex block_opt;
} AstAttrDecl;

typedef struct {
    SyntaxIndex referenced_compilation_unit;
} AstUseStatement;

typedef struct {
    SyntaxIndex expression;
} AstReturnStatement;

typedef struct {
    SyntaxIndex ident;
    SyntaxIndex type_opt;
    SyntaxIndex expression_opt;
} AstLocalDecl;

typedef struct {
    SyntaxIndex expression_left;
    SyntaxIndex parameters_opt;// TODO
} AstFunctionCallExpression;

typedef struct {
    SyntaxIndex func_call_exp;
} AstFunctionCallStatement;

#endif