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
    BIN_EQUALS,
    BIN_NOT_EQUALS,
} BinaryOperationKind;

typedef struct {
    SyntaxIndex index;
    SyntaxKind  kind;
} _TypedIndex;

typedef struct {
    StringIndex filepath;
    SyntaxIndex block;
} AstCompilationUnit;

typedef struct {
    ExpressionIndex expression_left;
    BinaryOperationKind operation;
    ExpressionIndex expression_right;
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
    SyntaxArray param_decls;// [SyntaxIndex] -> AstParameterDecl
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
    SyntaxArray attributes;// [SyntaxIndex] -> AstIdent
} AstAttrList;

typedef struct {
    StringIndex name;
} AstIdent;

typedef struct {
    SyntaxArray statements;// [StatementIndex]
} AstBlock;

typedef struct {
    SyntaxIndex ident;
    SyntaxIndex block_opt;
} AstAttrDecl;

typedef struct {
    SyntaxIndex referenced_compilation_unit;
} AstUseStatement;

typedef struct {
    ExpressionIndex expression;
} AstReturnStatement;

typedef struct {
    SyntaxIndex ident;
    SyntaxIndex type_opt;
    ExpressionIndex expression_opt;
} AstLocalDecl;

typedef struct {
    ExpressionIndex expression_left;
    SyntaxIndex parameters_opt;
} AstFunctionCall;

typedef struct {
    ExpressionIndex condition;
    StatementIndex  statement;
} AstIfStatement;

typedef struct {
    TokenType type;
    StringIndex string_value;
} AstLiteralExpression;

typedef struct {
    SyntaxArray param_expressions;// [ExpressionIndex]
} AstParamList;

#endif