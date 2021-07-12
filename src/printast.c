#include "headers/printast.h"

#define SPACE_PER_LEVEL 2
#define INDENT_CONST "                                            "

void print_unit(SyntaxFactory *f, AstCompilationUnit *n);
void print_block(SyntaxFactory *f, AstBlock *n, int indent);
void print_use(SyntaxFactory *f, AstUseStatement *n, int indent);
void print_statement(SyntaxFactory *f, StatementIndex i, int indent);
void print_attr_decl(SyntaxFactory *f, AstAttrDecl *n, int indent);

void print_ast(Parser *p) {
    for (int i = 0; i < p->factory->compilation_units.size; i++) {
        print_unit(p->factory, ((AstCompilationUnit*)p->factory->compilation_units.array) + i);
    }
}

void print_unit(SyntaxFactory *f, AstCompilationUnit *n) {
    printf("COMPILATION UNIT: '%s'\n", f->strings + n->filepath);
    print_block(f, ((AstBlock *)f->blocks.array) + n->block, 0);
}

void print_block(SyntaxFactory *f, AstBlock *n, int indent) {
    printf("%.*s", indent * SPACE_PER_LEVEL, INDENT_CONST);
    printf("BLOCK:\n");
    for (int i = 0; i < n->statements.size; i++) {
        print_statement(f, ((StatementIndex*)f->statements.array)[i], 1);
    }
}

void print_statement(SyntaxFactory *f, StatementIndex i, int indent) {
    if (!i.index) return;

    switch (i.kind) {
    case AST_USE:       
        print_use(f, ((AstUseStatement*)f->use_statements.array) + i.index, indent);
        break;
    case AST_ATTR_DECL:
        print_attr_decl(f, ((AstAttrDecl *)f->attribute_declarations.array) + i.index, indent);
        break;
    default:
    case AST_COMPILATION_UNIT:
        break;
    }
}

void print_use(SyntaxFactory *f, AstUseStatement *n, int indent) {
    printf("%.*s", indent * SPACE_PER_LEVEL, INDENT_CONST);
    printf("USE: ''\n");
}
void print_attr_decl(SyntaxFactory *f, AstAttrDecl *n, int indent) {
    printf("%.*s", indent * SPACE_PER_LEVEL, INDENT_CONST);
    AstIdent ident = ((AstIdent *)f->identifiers.array)[n->ident];
    printf("ATTR: '%s'\n", f->strings + ident.name);
}
