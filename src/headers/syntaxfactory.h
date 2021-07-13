#ifndef SYNTAXFACTORY_H
#define SYNTAXFACTORY_H

#include "common.h"
#include "token.h"
#include "ast.h"
#include "lexer.h"
#include "syntaxindex.h"

typedef struct {
    AstCompilationUnit   *current_unit;
    AstBlock             *current_block;
    SyntaxIndex          current_block_index;
    AstParameterListDecl *current_param_list_decl;
    AstAttrList          *current_attr_list;
    const char           *current_text;

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

    SyntaxArray statements;
    SyntaxArray blocks;
    SyntaxArray use_statements;
    SyntaxArray attribute_declarations;
    SyntaxArray func_declarations;
    SyntaxArray return_statements;
    SyntaxArray local_decl_statements;
    SyntaxArray function_call_statements;
    SyntaxArray if_statements;

    SyntaxArray expressions;
    SyntaxArray binary_expressions;
    SyntaxArray function_call_expressions;
    SyntaxArray literal_expressions;
    SyntaxArray param_lists;

    char        *strings;
    StringIndex strings_size;
    StringIndex strings_capacity;// we're hitting 4GB allocations at this point so I'll pretend we can never hit the max
#define STRINGS_MAX_GROW 1024
} SyntaxFactory;

SyntaxFactory *make_astfactory();
void destroy_astfactory(SyntaxFactory *factory);

SyntaxIndex begin_compilation_unit(SyntaxFactory *factory, LexResult *lex, const char *file_path);
void end_compilation_unit(SyntaxFactory *factory);

SyntaxIndex begin_block(SyntaxFactory *factory);
void end_block(SyntaxFactory *factory);

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
SyntaxIndex begin_param_list_decl(SyntaxFactory *factory);
void end_param_list_decl(SyntaxFactory *factory);
SyntaxIndex make_param(SyntaxFactory *factory, SyntaxIndex ident, Token *colon, SyntaxIndex type);
SyntaxIndex make_type(SyntaxFactory *factory, SyntaxIndex attr_list_opt, SyntaxIndex ident);
SyntaxIndex begin_attr_list(SyntaxFactory *factory, Token *left);
void end_attr_list(SyntaxFactory *factory);
SyntaxIndex make_unary_expression(SyntaxFactory *factory,
    SyntaxIndex value, SyntaxKind kind);
SyntaxIndex make_binary_expression(SyntaxFactory *factory,
    SyntaxIndex left, Token *op, SyntaxIndex right);
SyntaxIndex make_return_statement(SyntaxFactory *factory, Token *return_token, SyntaxIndex expression);
SyntaxIndex make_local_decl(SyntaxFactory *factory, SyntaxIndex ident, Token *colon, SyntaxIndex type_opt, Token *equals_opt, SyntaxIndex expresion_opt, Token *semicolon);
SyntaxIndex make_function_call_statement(SyntaxFactory *factory, SyntaxIndex func_call_exp, Token *semicolon);
SyntaxIndex make_function_call_expression(SyntaxFactory *factory, SyntaxIndex expression, Token *left, SyntaxIndex param_list_opt, Token *right);
SyntaxIndex make_if_statement(SyntaxFactory *factory, Token *if_tok, Token *left, SyntaxIndex cond, Token *right, SyntaxIndex stmt);
SyntaxIndex make_literal_expression(SyntaxFactory *factory, Token *token);
SyntaxIndex make_param_list(SyntaxFactory *factory);
SyntaxIndex add_param(SyntaxFactory *factory, SyntaxIndex list, SyntaxIndex exp);
SyntaxIndex block_as_statement(SyntaxFactory *factor, SyntaxIndex block);

#endif
