#ifndef AST_H
#define AST_H

#include "lexer.h"
#include "common.h"
#include "token.h"
#include "syntaxindex.h"
#include "darray.h"
#include "syntaxkind.h"
#include "scope.h"

typedef struct {
    SyntaxIndex index;
    SyntaxKind  kind;
} _TypedIndex;

typedef struct {
    StringIndex filepath;
    SyntaxIndex block;
} AstCompilationUnit;

typedef enum {
    BIN_ADD,
    BIN_MINUS,
    BIN_MULTIPLY,
    BIN_DIVIDE,
    BIN_EQUALS,
    BIN_NOT_EQUALS,
    BIN_MODULUS,
} BinaryOperationKind;

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
    DynamicArray param_decls;// [SyntaxIndex] -> AstParameterDecl
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
    DynamicArray attributes;// [SyntaxIndex] -> AstAttrListElem
} AstAttrList;

typedef struct {
    SyntaxIndex ident;
    bool        remove;
} AstAttrListElem;

typedef struct {
    StringIndex name;
    ScopeId     scope;
} AstIdent;

typedef struct {
    DynamicArray statements;// [StatementIndex]
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
    StatementIndex  else_statement;
} AstIfStatement;

typedef struct {
    TokenType type;
    StringIndex string_value;
} AstLiteralExpression;

typedef struct {
    DynamicArray param_expressions;// [ExpressionIndex]
} AstParamList;

typedef struct {
    SyntaxIndex attr_list;
    DynamicArray ident_list;// [SyntaxIndex] => AstIdent
} AstAnnotateStatement;

typedef enum {
    BAS_BIND = 0x01,
    BAS_ANNO = 0x10,

    BAS_TARGET_OP   = 0x001 << 2,
    BAS_TARGET_FUNC = 0x010 << 2,
    BAS_TARGET_WILD = 0x100 << 2,

    BAS_DEFS_ORDINAL = 0x01 << 5,
    BAS_DEFS_WILD    = 0x10 << 5,

    BAS_RETURN = 0x1 << 7,

    _BAS_BIND_OP_ORDINAL = BAS_BIND | BAS_TARGET_OP | BAS_DEFS_ORDINAL,
    _BAS_BIND_OP_ORDINAL_RETURN = BAS_BIND | BAS_TARGET_OP | BAS_DEFS_ORDINAL | BAS_RETURN,
    _BAS_BIND_OP_WILD = BAS_BIND | BAS_TARGET_OP | BAS_DEFS_WILD,
    _BAS_BIND_OP_WILD_RETURN = BAS_BIND | BAS_TARGET_OP | BAS_DEFS_WILD | BAS_RETURN,
    _BAS_BIND_FUNC_ORDINAL = BAS_BIND | BAS_TARGET_FUNC | BAS_DEFS_ORDINAL,
    _BAS_BIND_FUNC_ORDINAL_RETURN = BAS_BIND | BAS_TARGET_FUNC | BAS_DEFS_ORDINAL | BAS_RETURN,
    _BAS_BIND_FUNC_WILD = BAS_BIND | BAS_TARGET_FUNC | BAS_DEFS_WILD,
    _BAS_BIND_FUNC_WILD_RETURN = BAS_BIND | BAS_TARGET_FUNC | BAS_DEFS_WILD | BAS_RETURN,
    _BAS_BIND_WILD = BAS_BIND | BAS_TARGET_WILD,
    _BAS_BIND_WILD_RETURN = BAS_BIND | BAS_TARGET_WILD | BAS_RETURN,
    _BAS_ANNO_OP_ORDINAL_RETURN = BAS_ANNO | BAS_TARGET_OP | BAS_DEFS_ORDINAL | BAS_RETURN,
    _BAS_ANNO_OP_WILD_RETURN = BAS_ANNO | BAS_TARGET_OP | BAS_DEFS_WILD | BAS_RETURN,
    _BAS_ANNO_FUNC_ORDINAL_RETURN = BAS_ANNO | BAS_TARGET_FUNC | BAS_DEFS_ORDINAL | BAS_RETURN,
    _BAS_ANNO_FUNC_WILD_RETURN = BAS_ANNO | BAS_TARGET_FUNC | BAS_DEFS_WILD | BAS_RETURN,
    _BAS_ANNO_WILD_RETURN = BAS_ANNO | BAS_TARGET_WILD | BAS_RETURN,


} BindAnnoStatementKind;

typedef struct {
    SyntaxIndex           index;
    DynamicArray           *arr;
    BindAnnoStatementKind kind;
} AstBindAnnoMap;

typedef struct {
    SyntaxIndex func_target;   // BAS_TARGET_FUNC
    TokenType   op_target;     // BAS_TARGET_OP
    TokenType   wildcard;      // BAS_DEFS_WILD
    SyntaxIndex wildcard_list; // BAS_DEFS_WILD
    SyntaxIndex ordinals;      // BAS_DEFS_ORDINAL
    SyntaxIndex return_def;    // BAS_RETURN

    // NOTES:
    // Anno MUST define a non-empty return
} AstBindAnnoStatement;

typedef struct {
    DynamicArray ordinals;// [SyntaxIndex?] => AstAttrList
} AstBindAnnoOrdinals;

#endif