#include "headers/printast.h"

#define SPACE_PER_LEVEL 2
#define INDENT_CONST "                                            "

void print_unit(SyntaxFactory *f, SyntaxIndex i);
void print_block(SyntaxFactory *f, SyntaxIndex i, int indent);
void print_use(SyntaxFactory *f, SyntaxIndex i, int indent);
void print_ident(SyntaxFactory *f, SyntaxIndex i);
void print_statement(SyntaxFactory *f, SyntaxIndex i, int indent);
void print_attr_decl(SyntaxFactory *f, SyntaxIndex i, int indent);
void print_func_decl(SyntaxFactory *f, SyntaxIndex i, int indent);
void print_param_list_decl(SyntaxFactory *f, SyntaxIndex i, int indent);
void print_param_decl(SyntaxFactory *f, SyntaxIndex i, int indent);
void print_type(SyntaxFactory *f, SyntaxIndex i);
void print_attr_list(SyntaxFactory *f, SyntaxIndex i);
void print_return_statement(SyntaxFactory *f, SyntaxIndex i, int indent);
void print_expression(SyntaxFactory *f, SyntaxIndex i, int indent);
void print_binary_expression(SyntaxFactory *f, SyntaxIndex i, int indent);

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
        print_statement(f, i + 1, 1);
    }
}

void print_statement(SyntaxFactory *f, SyntaxIndex i, int indent) {
    TypedIndex ind = ((TypedIndex*)f->statements.array)[i - 1];
    if (!ind.index) return;

    switch (ind.kind) {
    case AST_USE:       
        print_use(f, ind.index, indent);
        break;
    case AST_ATTR_DECL:
        print_attr_decl(f, ind.index, indent);
        break;
    case AST_FUNC_DECL:
        print_func_decl(f, ind.index, indent);
        break;
    case AST_RETURN_STATEMENT:
        print_return_statement(f, ind.index, indent);
        break;
    default:
    case AST_COMPILATION_UNIT:
        fprintf(stderr, "Unexpected statement ast type %i\n", ind.kind);
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
    printf("\n");
    if (func->param_list_opt) print_param_list_decl(f, func->param_list_opt, indent + 1);
    if (func->return_type_opt) {
        printf("%.*s", (indent + 1) * SPACE_PER_LEVEL, INDENT_CONST);
        printf("RETURN TYPE: ");
        print_type(f, func->return_type_opt);
        printf("\n");
    }
    print_block(f, func->block, indent + 1);
}

void print_param_list_decl(SyntaxFactory *f, SyntaxIndex i, int indent) {
    printf("%.*s", indent * SPACE_PER_LEVEL, INDENT_CONST);
    AstParameterListDecl *list = ((AstParameterListDecl *)f->param_list_decls.array) + i - 1;
    printf("PARAMS %i:\n", i);
    for (SyntaxIndex pi = list->param_start; pi < list->param_end_noninclusive; pi++){
        print_param_decl(f, pi, indent + 1);
    }
}

void print_param_decl(SyntaxFactory *f, SyntaxIndex i, int indent) {
    printf("%.*s", indent * SPACE_PER_LEVEL, INDENT_CONST);
    AstParameterDecl *param = ((AstParameterDecl *)f->param_decls.array) + i - 1;
    printf("PARAM %i: ", i);
    print_ident(f, param->ident);
    printf(" of ");
    print_type(f, param->type);
    printf("\n");
}

void print_type(SyntaxFactory *f, SyntaxIndex i) {
    AstType *type = ((AstType *)f->types.array) + i - 1;
    printf("TYPE (%i) ", i);
    if (type->attr_list_opt) print_attr_list(f, type->attr_list_opt);
    print_ident(f, type->ident);
}

void print_attr_list(SyntaxFactory *f, SyntaxIndex i) {
    AstAttrList *list = ((AstAttrList *)f->attr_lists.array) + i - 1;
    printf("ATTRLIST (%i) ", i);
    SyntaxIndex *attrs = (SyntaxIndex *)list->attributes.array;
    for (int i = 0; i < list->attributes.size; i++)
    {
        print_ident(f, attrs[i]);
        printf(" ");
    }
}

void print_return_statement(SyntaxFactory *f, SyntaxIndex i, int indent) {
    AstReturnStatement *smt = ((AstReturnStatement *)f->return_statements.array) + i - 1;
    printf("RETURN STATEMENT (%i)\n", i);
    print_expression(f, smt->expression, indent + 1);
}

void print_expression(SyntaxFactory *f, SyntaxIndex i, int indent) {
    TypedIndex index = ((TypedIndex *)f->expressions.array)[i - 1];
    switch (index.kind)
    {
    case AST_BINARY_EXPRESSION:
        print_binary_expression(f, index.index, indent);
        break;
    case AST_IDENT:
        printf("%.*s", indent * SPACE_PER_LEVEL, INDENT_CONST);
        print_ident(f, index.index);
        printf("\n");
        break;
    default:
        fprintf(stderr, "Unexpected expression type %i\n", index.kind);
        break;
    }
    
}

void print_binary_expression(SyntaxFactory *f, SyntaxIndex i, int indent) {
    printf("%.*s", indent * SPACE_PER_LEVEL, INDENT_CONST);
    AstBinaryOperationExpression *bin = ((AstBinaryOperationExpression *)f->binary_expressions.array) + i - 1;
    const char *op = "ERR";

    switch(bin->operation) {
    case BIN_ADD:      op = "+"; break;
    case BIN_MINUS:    op = "-"; break;
    case BIN_MULTIPLY: op = "*"; break;
    case BIN_DIVIDE:   op = "/"; break;
    }

    printf("BIN EXP (%i) '%s'\n", i, op);
    print_expression(f, bin->expression_left, indent + 1);
    print_expression(f, bin->expression_right, indent + 1);
}
