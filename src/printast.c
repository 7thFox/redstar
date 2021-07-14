#include "headers/printast.h"
#define debugf(...) ;
// #define debugf(...) printf(__VA_ARGS__)

#define SPACE_PER_LEVEL 2
#define INDENT_CONST "| | | | | | | | | | | | | | | | | | | | | | "

void print_unit(SyntaxFactory *f, SyntaxIndex i);
void print_block(SyntaxFactory *f, SyntaxIndex i, int indent);
void print_use(SyntaxFactory *f, SyntaxIndex i, int indent);
void print_ident(SyntaxFactory *f, SyntaxIndex i);
void print_statement(SyntaxFactory *f, StatementIndex i, int indent);
void print_attr_decl(SyntaxFactory *f, SyntaxIndex i, int indent);
void print_func_decl(SyntaxFactory *f, SyntaxIndex i, int indent);
void print_param_list_decl(SyntaxFactory *f, SyntaxIndex i, int indent);
void print_param_decl(SyntaxFactory *f, SyntaxIndex i, int indent);
void print_type(SyntaxFactory *f, SyntaxIndex i);
void print_attr_list(SyntaxFactory *f, SyntaxIndex i);
void print_return_statement(SyntaxFactory *f, SyntaxIndex i, int indent);
void print_expression(SyntaxFactory *f, ExpressionIndex i, int indent);
void print_binary_expression(SyntaxFactory *f, SyntaxIndex i, int indent);
void print_local_decl(SyntaxFactory *f, SyntaxIndex i, int indent);
void print_function_call(SyntaxFactory *f, SyntaxIndex i, int indent);
void print_parameter_list(SyntaxFactory *f, SyntaxIndex i, int indent);
void print_if_statement(SyntaxFactory *f, SyntaxIndex i, int indent);
void print_literal_expression(SyntaxFactory *f, SyntaxIndex i, int indent);

void print_ast(Parser *p) {
    for (int i = 0; i < p->factory->compilation_units.size; i++) {
        print_unit(p->factory, (SyntaxIndex){i + 1});
    }
}

void print_ident(SyntaxFactory *f, SyntaxIndex i) {
    AstIdent *ident = get_ast_node(i, f->identifiers);
    printf("'%s' (%i)", get_string(f, ident->name), i.i);
}

void print_unit(SyntaxFactory *f, SyntaxIndex i) {
    AstCompilationUnit *unit = get_ast_node(i, f->compilation_units);
    printf("COMPILATION UNIT: '%s'\n", get_string(f, unit->filepath));
    print_block(f, unit->block, 0);
}

void print_block(SyntaxFactory *f, SyntaxIndex i, int indent) {
    AstBlock *block = get_ast_node(i, f->blocks);
    printf("%.*s", indent * SPACE_PER_LEVEL, INDENT_CONST);
    printf("BLOCK %i (%i statements):\n", i.i, block->statements.size);
    for (int i = 0; i < block->statements.size; i++) {
        StatementIndex *s = get_ast_node((SyntaxIndex){i + 1}, block->statements);
        print_statement(f, *s, indent + 1);
    }
}

void print_statement(SyntaxFactory *f, StatementIndex stmt_ind, int indent) {
    _TypedIndex i = *((_TypedIndex *)get_ast_node(stmt_ind, f->statements));
    debugf("print_statement %i (t:%i, i:%i)\n", stmt_ind.i, i.kind, i.index.i);

    if (!i.index.i) return;

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
    case AST_RETURN_STATEMENT:
        print_return_statement(f, i.index, indent);
        break;
    case AST_LOCAL_DECL:
        print_local_decl(f, i.index, indent);
        break;
    case AST_IF_STATEMENT:
        print_if_statement(f, i.index, indent);
        break;
    case AST_BLOCK:
        print_block(f, i.index, indent);
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
    printf("USE %i: ''\n", i.i);
}

void print_attr_decl(SyntaxFactory *f, SyntaxIndex i, int indent) {
    AstAttrDecl *attr = get_ast_node(i, f->attribute_declarations);
    printf("%.*s", indent * SPACE_PER_LEVEL, INDENT_CONST);
    printf("ATTR %i: ", i.i);
    print_ident(f, attr->ident);
    printf("\n");
}

void print_func_decl(SyntaxFactory *f, SyntaxIndex i, int indent) {
    printf("%.*s", indent * SPACE_PER_LEVEL, INDENT_CONST);
    AstFuncDecl *func = get_ast_node(i, f->func_declarations);
    printf("FUNC %i: ", i.i);
    print_ident(f, func->ident);
    printf("\n");
    if (func->param_list_opt.i) print_param_list_decl(f, func->param_list_opt, indent + 1);
    if (func->return_type_opt.i) {
        printf("%.*s", (indent + 1) * SPACE_PER_LEVEL, INDENT_CONST);
        printf("RETURN TYPE: ");
        print_type(f, func->return_type_opt);
        printf("\n");
    }
    print_block(f, func->block, indent + 1);
}

void print_param_list_decl(SyntaxFactory *f, SyntaxIndex i, int indent) {
    printf("%.*s", indent * SPACE_PER_LEVEL, INDENT_CONST);
    AstParameterListDecl *list = get_ast_node(i, f->param_list_decls);
    printf("PARAMS %i:\n", i.i);
    for (int pi = 0; pi < list->param_decls.size; pi++){
        print_param_decl(f, (SyntaxIndex){pi+1}, indent + 1);
    }
}

void print_param_decl(SyntaxFactory *f, SyntaxIndex i, int indent) {
    printf("%.*s", indent * SPACE_PER_LEVEL, INDENT_CONST);
    AstParameterDecl *param = get_ast_node(i, f->param_decls);
    printf("PARAM %i: ", i.i);
    print_ident(f, param->ident);
    printf(" of ");
    print_type(f, param->type);
    printf("\n");
}

void print_type(SyntaxFactory *f, SyntaxIndex i) {
    AstType *type = get_ast_node(i, f->types);
    printf("TYPE (%i) ", i.i);
    if (type->attr_list_opt.i) print_attr_list(f, type->attr_list_opt);
    print_ident(f, type->ident);
}

void print_attr_list(SyntaxFactory *f, SyntaxIndex i) {
    AstAttrList *list = get_ast_node(i, f->attr_lists);
    printf("ATTRLIST (%i) ", i.i);
    SyntaxIndex *attrs = (SyntaxIndex *)list->attributes.array;
    for (int i = 0; i < list->attributes.size; i++)
    {
        print_ident(f, attrs[i]);
        printf(" ");
    }
}

void print_return_statement(SyntaxFactory *f, SyntaxIndex i, int indent) {
    AstReturnStatement *smt = get_ast_node(i, f->return_statements);
    printf("%.*s", indent * SPACE_PER_LEVEL, INDENT_CONST);
    printf("RETURN STATEMENT (%i)\n", i.i);
    print_expression(f, smt->expression, indent + 1);
}

void print_expression(SyntaxFactory *f, ExpressionIndex i, int indent) {
    _TypedIndex index = *((_TypedIndex *)get_ast_node(i, f->expressions));
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
    case AST_FUNC_CALL:
        print_function_call(f, index.index, indent);
        break;
    case AST_LITERAL:
        print_literal_expression(f, index.index, indent);
        break;
    default:
        fprintf(stderr, "Unexpected expression type %i\n", index.kind);
        break;
    }
    
}

void print_binary_expression(SyntaxFactory *f, SyntaxIndex i, int indent) {
    printf("%.*s", indent * SPACE_PER_LEVEL, INDENT_CONST);
    AstBinaryOperationExpression *bin = get_ast_node(i, f->binary_expressions);
    const char *op = "ERR";

    switch(bin->operation) {
    case BIN_ADD:        op = "+"; break;
    case BIN_MINUS:      op = "-"; break;
    case BIN_MULTIPLY:   op = "*"; break;
    case BIN_DIVIDE:     op = "/"; break;
    case BIN_EQUALS:     op = "=="; break;
    case BIN_NOT_EQUALS: op = "!="; break;
    }

    printf("BIN EXP (%i) '%s'\n", i.i, op);
    print_expression(f, bin->expression_left, indent + 1);
    print_expression(f, bin->expression_right, indent + 1);
}

void print_local_decl(SyntaxFactory *f, SyntaxIndex i, int indent) {
    AstLocalDecl *decl = get_ast_node(i, f->local_decl_statements);
    printf("%.*s", indent * SPACE_PER_LEVEL, INDENT_CONST);
    printf("LOCAL DECL %i: ", i.i);
    print_ident(f, decl->ident);
        printf("\n");
    if (decl->type_opt.i) {
        printf("%.*s", (indent+1) * SPACE_PER_LEVEL, INDENT_CONST);
        printf("TYPE: ");
        print_type(f, decl->type_opt);
        printf("\n");
    }
    if (decl->expression_opt.i) {
        printf("%.*s", (indent+1) * SPACE_PER_LEVEL, INDENT_CONST);
        printf("ASSIGNMENT:\n");
        print_expression(f, decl->expression_opt, indent + 2);
    }
}

void print_function_call(SyntaxFactory *f, SyntaxIndex i, int indent) {
    AstFunctionCall *func = get_ast_node(i, f->function_calls);
    printf("%.*s", indent * SPACE_PER_LEVEL, INDENT_CONST);
    printf("FUNC CALL %i:\n", i.i);

    print_expression(f, func->expression_left, indent + 1);
    if (func->parameters_opt.i) {
        print_parameter_list(f, func->parameters_opt, indent + 1);
    }
}

void print_parameter_list(SyntaxFactory *f, SyntaxIndex i, int indent) {
    // TODO
}

void print_if_statement(SyntaxFactory *f, SyntaxIndex i, int indent) {
    AstIfStatement *is = get_ast_node(i, f->if_statements);
    printf("%.*s", indent * SPACE_PER_LEVEL, INDENT_CONST);
    printf("IF STMT %i:\n", i.i);
    print_expression(f, is->condition, indent + 1);
    print_statement(f, is->statement, indent + 1);
}

void print_literal_expression(SyntaxFactory *f, SyntaxIndex i, int indent) {
    AstLiteralExpression *lit = get_ast_node(i, f->literal_expressions);
    printf("%.*s", indent * SPACE_PER_LEVEL, INDENT_CONST);
    printf("LITERAL %i: %s\n", i.i, get_string(f, lit->string_value));
}
