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
    AstParameterListDecl *current_param_list;
    AstAttrList          *current_attr_list;
    const char           *current_text;

    Location last_location;

    SyntaxArray compilation_units;
    SyntaxArray identifiers;
    SyntaxArray blocks;

    SyntaxArray statements;
    SyntaxArray use_statements;
    SyntaxArray attribute_declarations;
    SyntaxArray func_declarations;
    SyntaxArray param_list_decls;
    SyntaxArray param_decls;
    SyntaxArray types;
    SyntaxArray attr_lists;

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
SyntaxIndex begin_param_list(SyntaxFactory *factory);
void end_param_list(SyntaxFactory *factory);
SyntaxIndex make_param(SyntaxFactory *factory, SyntaxIndex ident, Token *colon, SyntaxIndex type);
SyntaxIndex make_type(SyntaxFactory *factory, SyntaxIndex attr_list_opt, SyntaxIndex ident);
SyntaxIndex begin_attr_list(SyntaxFactory *factory, Token *left);
void end_attr_list(SyntaxFactory *factory);

#endif
