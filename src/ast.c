#include "headers/ast.h"

void build_unit(AST_COMPLIATION_UNIT *unit, LexResult *lex);

#define MAX_COMPILATION_UNITS 255
int compilation_units_count = 0;
AST_COMPLIATION_UNIT compilation_units[MAX_COMPILATION_UNITS];

int build_compliation_unit(LexResult *lex, const char *filePath) {
    if (compilation_units_count >= MAX_COMPILATION_UNITS) {
        return -1;
    }

    compilation_units[compilation_units_count].filename = filePath;
    build_unit(&compilation_units[compilation_units_count], lex);
    return compilation_units_count++;
}

void print_ast() {
    for (int i = 0; i < compilation_units_count; i++) {
        printf("Unit '%s':\n", compilation_units[i].filename);
    }
}

void build_unit(AST_COMPLIATION_UNIT *unit, LexResult *lex) {

}