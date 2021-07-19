#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "common.h"
#include "parser.h"
#include "darray.h"

typedef struct{uint16_t i;} ScopeId;

typedef struct {
    ScopeId scope_id;
    ScopeId scope_id_parent;
} STScope;

typedef struct {
    DynamicArray scopes;
} SymbolTable;

SymbolTable *create_symbol_table(Parser *parser);
void destroy_symbol_table(SymbolTable *st);

#endif