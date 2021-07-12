#include "headers/printast.h"

#define SPACE_PER_LEVEL 2
#define INDENT_CONST "                                            "

void print_unit(SyntaxFactory *f, SyntaxIndex i);
void print_block(SyntaxFactory *f, SyntaxIndex i, int indent);
void print_use(SyntaxFactory *f, SyntaxIndex i, int indent);
void print_ident(SyntaxFactory *f, SyntaxIndex i);
void print_statement(SyntaxFactory *f, StatementIndex i, int indent);
void print_attr_decl(SyntaxFactory *f, SyntaxIndex i, int indent);
void print_func_decl(SyntaxFactory *f, SyntaxIndex i, int indent);

void print_ast(Parser *p) {
    for (int i = 0; i < p->factory->compilation_units.size; i++) {
        print_unit(p->factory, i + 1);
    }
}

void print_ident(SyntaxFactory *f, SyntaxIndex i) {
    AstIdent *ident = ((AstIdent *)f->identifiers.array) + i -1;
    printf("'%s' (%i)", f->strings + ident->name, i);
}

void print_unit(SyntaxFactory *f, SyntaxIndex i) {
    AstCompilationUnit *unit = ((AstCompilationUnit *)f->compilation_units.array) + i - 1;
    printf("COMPILATION UNIT: '%s'\n", f->strings + unit->filepath);
    print_block(f, unit->block, 0);
}

void print_block(SyntaxFactory *f, SyntaxIndex i, int indent) {
    AstBlock *block = ((AstBlock *)f->blocks.array) + i - 1;
    printf("%.*s", indent * SPACE_PER_LEVEL, INDENT_CONST);
    printf("BLOCK %i:\n", i);
    for (int i = 0; i < block->statements.size; i++) {
        print_statement(f, ((StatementIndex*)f->statements.array)[i], 1);
    }
}

void print_statement(SyntaxFactory *f, StatementIndex i, int indent) {
    if (!i.index) return;

    switch (i.kind) {
    case AST_USE:       
        print_use(f, i.index, indent);
        break;
    case AST_ATTR_DECL:
        print_attr_decl(f, i.index, indent);
        break;
    case AST_FUNC_DECL:
        print_func_decl(f, i.index, indent);
        break;
    default:
    case AST_COMPILATION_UNIT:
        fprintf(stderr, "Unexpected statement ast type %i\n", i.kind);
        break;
    }
}

void print_use(SyntaxFactory *f, SyntaxIndex i, int indent) {
    // AstUseStatement *use = ((AstUseStatement *)f->use_statements.array) + i - 1;
    printf("%.*s", indent * SPACE_PER_LEVEL, INDENT_CONST);
    printf("USE %i: ''\n", i);
}

void print_attr_decl(SyntaxFactory *f, SyntaxIndex i, int indent) {
    AstAttrDecl *attr = ((AstAttrDecl *)f->attribute_declarations.array) + i - 1;
    printf("%.*s", indent * SPACE_PER_LEVEL, INDENT_CONST);
    printf("ATTR %i: ", i);
    print_ident(f, attr->ident);
    printf("\n");
}

void print_func_decl(SyntaxFactory *f, SyntaxIndex i, int indent) {
    printf("%.*s", indent * SPACE_PER_LEVEL, INDENT_CONST);
    AstFuncDecl *func = ((AstFuncDecl *)f->func_declarations.array) + i - 1;
    printf("FUNC %i: ", i);
    print_ident(f, func->ident);
    // ...
    printf("\n");
    print_block(f, func->block, indent + 1);
}
