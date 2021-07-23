#include "headers/symboltable.h"

void make_ident_symbols(SymbolTable *st, Parser *p);
void set_ident_symbol_ids(SymbolTable *st, Parser *p);

SymbolTable *create_symbol_table(Parser *parser) {
    SymbolTable *st = malloc(sizeof(SymbolTable));
    st->nsymbols = 0;
    st->symbols = 0;
    st->scope_info = 0;
    st->has_error = false;

    make_ident_symbols(st, parser);
    set_ident_symbol_ids(st, parser);

    return st;
}

void destroy_symbol_table(SymbolTable *st) {
    if (st->symbols) free(st->symbols);
    if (st->scope_info) free(st->scope_info);
    free(st);
}

void make_ident_symbols(SymbolTable *st, Parser *p) {
    st->nsymbols = p->factory->ident_decls.size;
    st->symbols = malloc(sizeof(STSymbol) * st->nsymbols);

    st->nscopes = p->factory->scopes.size;
    st->scope_info = malloc(sizeof(_scope_info) * st->nscopes);
    memset(st->scope_info, 0, sizeof(_scope_info) * st->nscopes);

    {
        AstIdent *ident;
        SyntaxIndex ident_index;
        STSymbol *symbol;
        for (int i = 0; i < p->factory->ident_decls.size; i++) {
            ident_index = ((SyntaxIndex *)p->factory->ident_decls.array)[i];
            ident = get_ast_node(ident_index, p->factory->identifiers);
            symbol = st->symbols + i;
            symbol->name = ident->name;
            symbol->decl_scope = ident->scope;
            symbol->decl_location = ident->location;
            st->scope_info[ident->scope.i - 1].count++;
        }
    }
    {
        int sum = 0;
        for (int i = 0; i < st->nscopes; i++)
        {
            _scope_info *info = st->scope_info + i;
            info->offset = sum;
            sum += info->count;
            info->count = 0;
            info->parent_id = ((STScope *)p->factory->scopes.array)[i].id_parent;
        }
    }
    {
        STSymbol *symbols_unsorted = st->symbols;
        st->symbols = malloc(sizeof(STSymbol) * st->nsymbols);
        for (int i = 0; i < st->nsymbols; i++) {
            STSymbol sym = symbols_unsorted[i];
            _scope_info *info = st->scope_info + sym.decl_scope.i - 1;
            st->symbols[info->offset + info->count] = sym;
            info->count++;
        }
        free(symbols_unsorted);
    }
}

void set_ident_symbol_ids(SymbolTable *st, Parser *p) {
    for (int i = 0; i < p->factory->identifiers.size; i++) {
        AstIdent *ident = ((AstIdent *)p->factory->identifiers.array) + i;

        ScopeId scope = ident->scope;
        while (scope.i) {
            _scope_info info = st->scope_info[scope.i - 1];
            for (int j = 0; j < info.count; j++) {
                StringIndex si = st->symbols[info.offset + j].name;
                if (si.i == ident->name.i) {
                    ident->symbol = (SymbolId){info.offset + j + 1};
                    goto ident_set;
                }
            }

                scope = info.parent_id;
        }
    ident_set:;
    }
}