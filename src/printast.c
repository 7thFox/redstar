#include "headers/printast.h"

#define SPACE_PER_LEVEL 2
#define INDENT_CONST "                                            "

void print_unit(SyntaxFactory *f, AstCompilationUnit *n);
void print_use(SyntaxFactory *f,AstUseStatement *n, int indent);

void print_ast(Parser *p) {
    for (int i = 0; i < p->factory->compilation_units.size; i++) {
        print_unit(p->factory, ((AstCompilationUnit*)p->factory->compilation_units.array) + i);
    }
}

void print_unit(SyntaxFactory *f, AstCompilationUnit *n) {
    printf("COMPILATION UNIT: '%s'\n", f->strings + n->filepath);
    for (int i = n->use_start; i < n->use_end_noninclusive; i++) {
        print_use(f, ((AstUseStatement*)f->use_statements.array) + i, 1);
    }
}

void print_use(SyntaxFactory *f, AstUseStatement *n, int indent) {
    printf("%.*s", indent * SPACE_PER_LEVEL, INDENT_CONST);
    printf("USE: ''\n");
}
