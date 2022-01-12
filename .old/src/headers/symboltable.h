#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "common.h"
#include "parser.h"
#include "darray.h"
#include "syntaxindex.h"

typedef struct {
    StringIndex name;
    ScopeId     decl_scope;
    Cursor      decl_location;
} STSymbol;

typedef struct {
    int count;
    int offset;
    ScopeId parent_id;
} _scope_info;

typedef struct
{
    bool has_error;

    size_t nsymbols;
    STSymbol *symbols;

    int      nscopes;
    _scope_info *scope_info;
} SymbolTable;

SymbolTable *create_symbol_table(Parser *parser);
void destroy_symbol_table(SymbolTable *st);

#endif