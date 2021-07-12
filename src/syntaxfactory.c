#include "headers/syntaxfactory.h"

SyntaxArray init_array(size_t elem_size, uint16_t cap);
void *next_array_elem(SyntaxArray *arr);
StringIndex copy_string(SyntaxFactory *f, const char *str);
void set_last_location(SyntaxFactory *f, Token *x);

#define assert(f, x, kind)                              \
    if (!x)                                             \
    {                                                   \
        fprintf(stderr, "%s:%i:%i: Missing %s\n",       \
                f->strings + f->current_unit->filepath, \
                f->last_location.line,                  \
                f->last_location.col,                   \
                kind);                                  \
        return EMPTY_SYNTAX_INDEX;                      \
    }                                                   \
    set_last_location(f, x);

SyntaxFactory *make_astfactory() {
    debugf(VERBOSITY_NORMAL, "make_astfactory\n");
    SyntaxFactory *f = malloc(sizeof(SyntaxFactory));
    f->current_unit = 0;
    f->compilation_units = init_array(sizeof(AstCompilationUnit), 8);
    debugf(VERBOSITY_NORMAL, "f->compilation_units (%x.%x) : %x (%i/%i) x %i \n", f, &(f->compilation_units) ,f->compilation_units.array, f->compilation_units.size, f->compilation_units.capacity, f->compilation_units.elem_size);
    f->use_statements = init_array(sizeof(AstUseStatement), 16);
    f->identifiers = init_array(sizeof(AstIdent), 128);
    return f;
}

void destroy_astfactory(SyntaxFactory *f) {
    free(f->strings);
    free(f->compilation_units.array);
    free(f->use_statements.array);
    free(f->identifiers.array);
}

SyntaxArray init_array(size_t elem_size, uint16_t cap) {
    SyntaxArray a;
    a.elem_size = elem_size;
    a.capacity = cap;
    a.size = 0;
    a.array = malloc(elem_size * cap);
    debugf(VERBOSITY_HIGH, "init_array: %x\n", a.array);
    return a;
}

void set_last_location(SyntaxFactory *f, Token *x) {

}

void *next_array_elem(SyntaxArray *arr) {
    debugf(VERBOSITY_HIGH, "next_array_elem");
    debugf(VERBOSITY_HIGH, ": %x (%i/%i)x%i\n", arr->array, arr->size, arr->capacity, arr->elem_size);
    arr->size++;
    if (arr->size > arr->capacity) {
        uint16_t newcap = arr->capacity < SYNTAX_ARRAY_MAX_GROW
            ? arr->capacity * 2
            : arr->capacity + SYNTAX_ARRAY_MAX_GROW;
        if (newcap > SYNTAX_ARRAY_MAX_CAPACITY) {
            if (arr->capacity == SYNTAX_ARRAY_MAX_CAPACITY) {
                // TODO: be sad now
            }
            newcap = SYNTAX_ARRAY_MAX_CAPACITY;
        }
        arr->capacity = newcap;
        arr->array = realloc(arr->array, arr->elem_size * arr->capacity);
    }

    return arr->array + ((arr->size - 1) * arr->elem_size);
}

StringIndex copy_string(SyntaxFactory *f, const char *str) {
    size_t len = strlen(str);
    if (f->strings_size + len > f->strings_capacity) {
        StringIndex newcap = f->strings_capacity < STRINGS_MAX_GROW
            ? f->strings_capacity * 2
            : f->strings_capacity + STRINGS_MAX_GROW;
        f->strings_capacity = newcap;
        f->strings = realloc(f->strings, sizeof(char) * f->strings_capacity);
    }
    StringIndex index = f->strings_size;
    strcpy(f->strings + index, str);
    f->strings_size += len;
    return index;
}

SyntaxIndex begin_compilation_unit(SyntaxFactory *f, LexResult *lex, const char *file_path) {
    debugf(VERBOSITY_NORMAL, "begin_compilation_unit\n");
    // TODO: Search for referenced comp unit first
    f->current_unit = next_array_elem(&(f->compilation_units));
    f->current_unit->filepath = copy_string(f, file_path);
    f->current_unit->use_start = f->use_statements.size;
    return f->compilation_units.size - 1;
}

void end_compilation_unit(SyntaxFactory *f) {
    debugf(VERBOSITY_NORMAL, "end_compilation_unit\n");
    f->current_unit->use_end_noninclusive = f->use_statements.size;

    f->current_unit = 0;
}

SyntaxIndex make_use_statement(SyntaxFactory *f, Token *use_tok, Token *name) {
    debugf(VERBOSITY_NORMAL, "make_use_statement\n");
    assert(f, use_tok, "Use Keyword");
    assert(f, name, "Use Path");
    AstUseStatement *use = next_array_elem(&(f->use_statements));
    use->referenced_compilation_unit = EMPTY_SYNTAX_INDEX;// TODO: Create future comp unit
    return f->use_statements.size - 1;
}