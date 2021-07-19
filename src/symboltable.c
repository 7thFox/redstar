#include "headers/symboltable.h"

SymbolTable *create_symbol_table(Parser *parser) {
    SymbolTable *st = malloc(sizeof(SymbolTable));

    return st;
}

void destroy_symbol_table(SymbolTable *st) {
    free(st);
}