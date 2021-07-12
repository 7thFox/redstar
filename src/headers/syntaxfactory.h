#ifndef SYNTAXFACTORY_H
#define SYNTAXFACTORY_H

#include "common.h"
#include "token.h"
#include "ast.h"
#include "lexer.h"
#include "syntaxindex.h"


typedef struct {
    AstCompilationUnit  *current_unit;
    AstBlock            *current_block;
    const char          *current_text;

    Location last_location;

    SyntaxArray compilation_units;
    SyntaxArray identifiers;
    SyntaxArray blocks;

    SyntaxArray statements;
    SyntaxArray use_statements;
    SyntaxArray attribute_declarations;

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

#endif
