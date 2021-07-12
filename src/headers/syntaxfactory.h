#ifndef SYNTAXFACTORY_H
#define SYNTAXFACTORY_H

#include "common.h"
#include "token.h"
#include "ast.h"
#include "lexer.h"
#include "syntaxindex.h"

typedef struct {
    uint8_t     *array;
    size_t      elem_size;
    uint16_t    size;
    uint16_t    capacity;
#define SYNTAX_ARRAY_MAX_CAPACITY 0xFFFF
#define SYNTAX_ARRAY_MAX_GROW 64
} SyntaxArray;

typedef struct
{
    AstCompilationUnit *current_unit;
    const char *current_text;

    Location last_location;

    SyntaxArray compilation_units;
    SyntaxArray identifiers;

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

SyntaxIndex make_use_statement(SyntaxFactory *factory, Token *use, Token *name);
SyntaxIndex make_ident(SyntaxFactory *factory, Token *name);
SyntaxIndex make_attr_decl(SyntaxFactory *factory, Token *attr, SyntaxIndex ident, SyntaxIndex block);

#endif
