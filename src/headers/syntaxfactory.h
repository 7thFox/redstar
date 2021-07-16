#ifndef SYNTAXFACTORY_H
#define SYNTAXFACTORY_H

#include "common.h"
#include "token.h"
#include "ast.h"
#include "lexer.h"
#include "syntaxindex.h"

typedef struct {
    // AstCompilationUnit   *current_unit;
    // AstBlock             *current_block;
    // SyntaxIndex          current_block_index;
    // AstParameterListDecl *current_param_list_decl;
    // AstAttrList          *current_attr_list;
    const char  *current_text;
    StringIndex current_filepath;

    // kinda hackish but I want that location...
    Token *tokens;
    token_index *current_token;
    // Location last_location;

    SyntaxArray compilation_units;
    SyntaxArray identifiers;
    SyntaxArray types;
    SyntaxArray param_list_decls;
    SyntaxArray param_decls;
    SyntaxArray attr_lists;
    SyntaxArray param_lists;
    SyntaxArray bind_anno_map;

    SyntaxArray statements;
    SyntaxArray blocks;
    SyntaxArray use_statements;
    SyntaxArray attribute_declarations;
    SyntaxArray func_declarations;
    SyntaxArray return_statements;
    SyntaxArray local_decl_statements;
    SyntaxArray if_statements;
    SyntaxArray annotate_statements;
    SyntaxArray bind_op_statements;
    SyntaxArray bind_func_statements;
    SyntaxArray anno_op_statements;
    SyntaxArray anno_func_statements;

    SyntaxArray expressions;
    SyntaxArray binary_expressions;
    SyntaxArray literal_expressions;
    SyntaxArray function_calls;

    char        *strings;
    StringIndex strings_size;
    StringIndex strings_capacity;// we're hitting 4GB allocations at this point so I'll pretend we can never hit the max
#define STRINGS_MAX_GROW 1024
} SyntaxFactory;

static inline const char *get_string(SyntaxFactory *f, StringIndex i){
    return f->strings + i.i;
}

StatementIndex make_statement(SyntaxFactory *f, SyntaxIndex i, SyntaxKind k);
ExpressionIndex make_expression(SyntaxFactory *f, SyntaxIndex i, SyntaxKind k);

SyntaxFactory *make_astfactory();
void destroy_astfactory(SyntaxFactory *factory);

SyntaxIndex make_compilation_unit(SyntaxFactory *factory, LexResult *lex, const char *file_path);

SyntaxIndex make_block(SyntaxFactory *factory);
SyntaxIndex add_statement(SyntaxFactory *factory, SyntaxIndex block, StatementIndex stmt);

SyntaxIndex make_use_statement(SyntaxFactory *factory, Token *use, Token *name);
SyntaxIndex make_ident(SyntaxFactory *factory, Token *name);
SyntaxIndex make_attr_decl(SyntaxFactory *factory, Token *attr, SyntaxIndex ident, SyntaxIndex block);
SyntaxIndex make_func_decl(SyntaxFactory *factory, 
    Token *func, 
    SyntaxIndex ident, 
    Token *left_paran, 
    SyntaxIndex param_list_opt, 
    Token *right_paran, 
    SyntaxIndex return_type_opt, 
    SyntaxIndex block);
SyntaxIndex make_param_list_decl(SyntaxFactory *factory);
SyntaxIndex add_param_decl(SyntaxFactory *factory, SyntaxIndex param_list, SyntaxIndex param_decl);
SyntaxIndex make_param_decl(SyntaxFactory *factory, SyntaxIndex ident, Token *colon, SyntaxIndex type);
SyntaxIndex make_type(SyntaxFactory *factory, SyntaxIndex attr_list_opt, SyntaxIndex ident);
SyntaxIndex make_attr_list(SyntaxFactory *factory);
SyntaxIndex add_attr(SyntaxFactory *factory, SyntaxIndex attr_list, SyntaxIndex attr_ident);
// void end_attr_list(SyntaxFactory *factory);
// SyntaxIndex make_unary_expression(SyntaxFactory *factory, SyntaxIndex value, SyntaxKind kind);
SyntaxIndex make_binary_expression(SyntaxFactory *factory, ExpressionIndex left, Token *op, ExpressionIndex right);
SyntaxIndex make_return_statement(SyntaxFactory *factory, Token *return_token, ExpressionIndex expression);
SyntaxIndex make_local_decl(SyntaxFactory *factory, SyntaxIndex ident, Token *colon, SyntaxIndex type_opt, Token *equals_opt, ExpressionIndex expresion_opt, Token *semicolon);
// SyntaxIndex make_function_call_statement(SyntaxFactory *factory, SyntaxIndex func_call_exp, Token *semicolon);
SyntaxIndex make_function_call(SyntaxFactory *factory, ExpressionIndex expression, Token *left, SyntaxIndex param_list_opt, Token *right);
SyntaxIndex make_if_statement(SyntaxFactory *factory, Token *if_tok, Token *left, ExpressionIndex cond, Token *right, StatementIndex stmt);
SyntaxIndex make_literal_expression(SyntaxFactory *factory, Token *token);
SyntaxIndex make_param_list(SyntaxFactory *factory);
SyntaxIndex add_param(SyntaxFactory *factory, SyntaxIndex list, ExpressionIndex exp);
SyntaxIndex make_annotate_statement(SyntaxFactory *factory, SyntaxIndex list, SyntaxIndex ident, Token *semi);

SyntaxIndex make_bind_operation_statement(SyntaxFactory *factory, Token* bind, Token *op);
SyntaxIndex make_bind_function_statement(SyntaxFactory *factory, Token* bind, SyntaxIndex ident);
SyntaxIndex make_anno_operation_statement(SyntaxFactory *factory, Token* anno, Token *op);
SyntaxIndex make_anno_function_statement(SyntaxFactory *factory, Token* anno, SyntaxIndex ident);
SyntaxIndex add_bind_anno_definition(SyntaxFactory *factory, SyntaxIndex stmt, SyntaxIndex def);
SyntaxIndex add_bind_anno_definition_ignore(SyntaxFactory *factory, SyntaxIndex stmt, Token *ignore);
void add_bind_anno_return(SyntaxFactory *factory, SyntaxIndex stmt, Token *arrow, SyntaxIndex return_def);

#endif
