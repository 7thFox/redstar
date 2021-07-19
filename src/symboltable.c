#include "headers/symboltable.h"

void make_ident_symbols(SymbolTable *st, Parser *p);
void set_ident_symbol_ids(SymbolTable *st, Parser *p);

SymbolTable *create_symbol_table(Parser *parser) {
    SymbolTable *st = malloc(sizeof(SymbolTable));
    st->nsymbols = 0;
    st->symbols = 0;

    make_ident_symbols(st, parser);

    return st;
}

void destroy_symbol_table(SymbolTable *st) {
    if (st->symbols) free(st->symbols);
    free(st);
}

void make_ident_symbols(SymbolTable *st, Parser *p) {
    // st->nsymbols = p->factory->ident_decls.size;
    // st->symbols = malloc(sizeof(STSymbol) * st->nsymbols);

    // int *collision_counts = malloc(sizeof(int) * 0x100);
    // memset(collision_counts, 0, sizeof(int) * 0x100);

    // AstIdent *ident;
    // SyntaxIndex ident_index;
    // STSymbol *symbol;
    // for (int i = 0; i < p->factory->ident_decls.size; i++)
    // {
    //     ident_index = ((SyntaxIndex *)p->factory->ident_decls.array)[i];
    //     ident = get_ast_node(ident_index, p->factory->identifiers);
    //     symbol = st->symbols + i;
    //     symbol->name = ident->name;
    //     symbol->decl_scope = ident->scope;
    //     scope_decl_counts[ident->scope.i - 1]++;
    // }


}

void set_ident_symbol_ids(SymbolTable *st, Parser *p) {

}