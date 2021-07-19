#ifndef SYNTAXFACTORY_H
#define SYNTAXFACTORY_H

#include "common.h"
#include "token.h"
#include "ast.h"
#include "lexer.h"
#include "syntaxindex.h"
#include "darray.h"

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

    DynamicArray compilation_units;
    DynamicArray identifiers;
    DynamicArray types;
    DynamicArray param_list_decls;
    DynamicArray param_decls;
    DynamicArray attr_lists;
    DynamicArray attr_lists_elems;
    DynamicArray param_lists;
    DynamicArray bind_anno_map;
    DynamicArray bind_anno_ordinals;

    DynamicArray statements;
    DynamicArray blocks;
    DynamicArray use_statements;
    DynamicArray attribute_declarations;
    DynamicArray func_declarations;
    DynamicArray return_statements;
    DynamicArray local_decl_statements;
    DynamicArray if_statements;
    DynamicArray annotate_statements;

    // bind/anno
    DynamicArray bind_op_ordinal_statements;
    DynamicArray bind_op_ordinal_return_statements;
    DynamicArray bind_op_wild_statements;
    DynamicArray bind_op_wild_return_statements;
    DynamicArray bind_func_ordinal_statements;
    DynamicArray bind_func_ordinal_return_statements;
    DynamicArray bind_func_wild_statements;
    DynamicArray bind_func_wild_return_statements;
    DynamicArray anno_op_ordinal_return_statements;
    DynamicArray anno_op_wild_return_statements;
    DynamicArray anno_func_ordinal_return_statements;
    DynamicArray anno_func_wild_return_statements;
    DynamicArray bind_wild_statements;
    DynamicArray bind_wild_return_statements;
    DynamicArray anno_wild_return_statements;

    DynamicArray expressions;
    DynamicArray binary_expressions;
    DynamicArray literal_expressions;
    DynamicArray function_calls;

    char        *strings;
    StringIndex strings_size;
    StringIndex strings_capacity;// we're hitting 4GB allocations at this point so I'll pretend we can never hit the max
#define STRINGS_MAX_GROW 1024
} SyntaxFactory;

// Helpers / Meta
static inline const char *get_string(SyntaxFactory *f, StringIndex i){
    return f->strings + i.i;
}
StatementIndex make_statement(SyntaxFactory *f, SyntaxIndex i, SyntaxKind k);
ExpressionIndex make_expression(SyntaxFactory *f, SyntaxIndex i, SyntaxKind k);
SyntaxFactory *make_astfactory();
void destroy_astfactory(SyntaxFactory *factory);

// make_
SyntaxIndex make_compilation_unit(SyntaxFactory *factory, LexResult *lex, const char *file_path);
SyntaxIndex make_block(SyntaxFactory *factory);
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
SyntaxIndex make_param_decl(SyntaxFactory *factory, SyntaxIndex ident, Token *colon, SyntaxIndex type);
SyntaxIndex make_type(SyntaxFactory *factory, SyntaxIndex attr_list_opt, SyntaxIndex ident);
SyntaxIndex make_attr_list(SyntaxFactory *factory);
SyntaxIndex make_attr_list_elem(SyntaxFactory *factory, Token *remove_opt, SyntaxIndex ident);
SyntaxIndex make_binary_expression(SyntaxFactory *factory, ExpressionIndex left, Token *op, ExpressionIndex right);
SyntaxIndex make_return_statement(SyntaxFactory *factory, Token *return_token, ExpressionIndex expression);
SyntaxIndex make_local_decl(SyntaxFactory *factory, SyntaxIndex ident, Token *colon, SyntaxIndex type_opt, Token *equals_opt, ExpressionIndex expresion_opt, Token *semicolon);
SyntaxIndex make_function_call(SyntaxFactory *factory, ExpressionIndex expression, Token *left, SyntaxIndex param_list_opt, Token *right);
SyntaxIndex make_if_statement(SyntaxFactory *factory, Token *if_tok, Token *left, ExpressionIndex cond, Token *right, StatementIndex stmt, Token *else_opt, StatementIndex else_stmt_opt);
SyntaxIndex make_literal_expression(SyntaxFactory *factory, Token *token);
SyntaxIndex make_param_list(SyntaxFactory *factory);
SyntaxIndex make_annotate_statement(SyntaxFactory *factory, SyntaxIndex attr_list);
SyntaxIndex make_bind_anno_ordinals(SyntaxFactory *factory);
SyntaxIndex make_bind_anno_statement(SyntaxFactory *factory,
    BindAnnoStatementKind kind,
    Token *bind_anno,
    SyntaxIndex func, Token *op, Token *wild_target,
    Token *wildcard_kind, SyntaxIndex wildcard_def, SyntaxIndex ordinals,
    Token *arrow_opt, SyntaxIndex return_def_opt);

// add_
SyntaxIndex add_statement(SyntaxFactory *factory, SyntaxIndex block, StatementIndex stmt);
SyntaxIndex add_param(SyntaxFactory *factory, SyntaxIndex list, ExpressionIndex exp);
SyntaxIndex add_attr(SyntaxFactory *factory, SyntaxIndex attr_list, SyntaxIndex attr_elem);
SyntaxIndex add_param_decl(SyntaxFactory *factory, SyntaxIndex param_list, SyntaxIndex param_decl);
SyntaxIndex add_bind_anno_ordinal(SyntaxFactory *factory, SyntaxIndex ordinal_list, SyntaxIndex attr_list_opt);
SyntaxIndex add_ident_to_annotate(SyntaxFactory *factory, SyntaxIndex annotate, SyntaxIndex ident);

#endif
