#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "common.h"
#include "parser.h"
#include "darray.h"

// typedef struct{uint16_t i} SymbolId;

// typedef struct {
//     // uint8_t hash;
//     int collisions;
// } STSymbolBucket;

// typedef struct {
//     StringIndex name;
//     SymbolId    id;
// } STSymbolBucketEntry;

typedef struct {
    StringIndex name;
    ScopeId     decl_scope;
} STSymbol;

typedef struct
{
    size_t nsymbols;
    STSymbol *symbols;
} SymbolTable;

SymbolTable *create_symbol_table(Parser *parser);
void destroy_symbol_table(SymbolTable *st);

#endif