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
void print_annotate_statement(SyntaxFactory *f, SyntaxIndex i, int indent);
void print_bind_anno_statement(SyntaxFactory *f, SyntaxIndex i, int indent);
void print_operation(SyntaxFactory *f, TokenType op);

void print_ast(Parser *p) {
    for (int i = 0; i < p->factory->compilation_units.size; i++) {
        print_unit(p->factory, (SyntaxIndex){i + 1});
    }
}

void print_ident(SyntaxFactory *f, SyntaxIndex i) {
    AstIdent *ident = get_ast_node(i, f->identifiers);
    printf("'%s' (%i)", get_string(f->string_table, ident->name), i.i);
}

void print_unit(SyntaxFactory *f, SyntaxIndex i) {
    AstCompilationUnit *unit = get_ast_node(i, f->compilation_units);
    printf("COMPILATION UNIT: '%s'\n", get_string(f->string_table, unit->filepath));
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
        case AST_ANNOTATE_LOCAL:
            print_annotate_statement(f, i.index, indent);
            break;
        case AST_BIND_ANNO:
            print_bind_anno_statement(f, i.index, indent);
            break;
        case AST_COMPILATION_UNIT:
        default:
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
    case BIN_MODULUS:    op = "MOD"; break;
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
    AstParamList *l = get_ast_node(i, f->param_list_decls);
    printf("%.*s", indent * SPACE_PER_LEVEL, INDENT_CONST);
    printf("PARAMS %i:\n", i.i);
    for (int j = 0; j < l->param_expressions.size; j++) {
        ExpressionIndex param = ((ExpressionIndex *)l->param_expressions.array)[j];
        print_expression(f, param, indent + 1);
    }
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
    printf("LITERAL %i: ", i.i);
    switch (lit->type) {
        case TOK_NUMERIC_LITERAL:
            printf("NUMBER %li", lit->int_value);
            break;
        default:
            fprintf(stderr, "Unexpected literal type\n");
    }
    //get_string(f->string_table, lit->string_value)
    printf("\n");
}

void print_annotate_statement(SyntaxFactory *f, SyntaxIndex i, int indent) {
    AstAnnotateStatement *a = get_ast_node(i, f->annotate_statements);
    printf("%.*s", indent * SPACE_PER_LEVEL, INDENT_CONST);
    printf("ANNO LOCAL %i:\n", i.i);

    printf("%.*s", (indent+1) * SPACE_PER_LEVEL, INDENT_CONST);
    printf("ATTRS: ");
    print_attr_list(f, a->attr_list);
    printf("\n");

    printf("%.*s", (indent + 1) * SPACE_PER_LEVEL, INDENT_CONST);
    printf("IDENTS:\n");
    for (int i = 0; i < a->ident_list.size; i++) {
        printf("%.*s", (indent + 2) * SPACE_PER_LEVEL, INDENT_CONST);
        print_ident(f, ((SyntaxIndex*)a->ident_list.array)[i]);
        printf("\n");
    }
}

void print_bind_anno_statement(SyntaxFactory *f, SyntaxIndex i, int indent) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch"
    AstBindAnnoMap *m = get_ast_node(i, f->bind_anno_map);
    AstBindAnnoStatement *s = get_ast_node(m->index, (*m->arr));
    printf("%.*s", indent * SPACE_PER_LEVEL, INDENT_CONST);

    switch (m->kind & 0x11)
    {
        case BAS_BIND:
            printf("BIND");
            break;
        case BAS_ANNO:
            printf("ANNO");
            break;
    }
    printf(":\n");

    printf("%.*s", (indent+1) * SPACE_PER_LEVEL, INDENT_CONST);
    printf("TARGET: ");
    switch (m->kind & (0x111 << 2))
    {
        case BAS_TARGET_FUNC:
            print_ident(f, s->func_target);
            break;
        case BAS_TARGET_OP:
            printf("op ");
            print_operation(f, s->op_target);
            break;
        case BAS_TARGET_WILD:
            printf("WILD");
            break;
    }
    printf("\n");

    printf("%.*s", (indent+1) * SPACE_PER_LEVEL, INDENT_CONST);
    printf("DEFS:\n");
    switch (m->kind & (0x11 << 5))
    {
        case BAS_DEFS_ORDINAL:
        {
            AstBindAnnoOrdinals *ords = get_ast_node(s->ordinals, f->bind_anno_ordinals);
            for (int j = 0; j < ords->ordinals.size; j++) {
                printf("%.*s", (indent+2) * SPACE_PER_LEVEL, INDENT_CONST);
                SyntaxIndex ordi = ((SyntaxIndex *)ords->ordinals.array)[j];
                if (ordi.i) {
                    print_attr_list(f, ordi);
                }
                else {
                    printf("NONE");
                }
                printf("\n");
            }
        }
            break;
        case BAS_DEFS_WILD:
            printf("%.*s", (indent+2) * SPACE_PER_LEVEL, INDENT_CONST);
            switch (s->wildcard) {
            case TOK_ANY:
                printf("ANY");
            case TOK_ALL:
                printf("ALL");
            case TOK_NONE:
                printf("NONE");
            }
            printf(" ");
            print_attr_list(f, s->wildcard_list);
            printf("\n");
            break;
    }

    if ((m->kind & BAS_RETURN) == BAS_RETURN) {
        printf("%.*s", (indent+2) * SPACE_PER_LEVEL, INDENT_CONST);
        printf("RETURN: ");
        print_attr_list(f, s->return_def);
        printf("\n");
    }
#pragma GCC diagnostic pop
}

void print_operation(SyntaxFactory *f, TokenType op) {
    if (op <= 255) {
        printf("%c", op);
        return;
    }
    switch (op)
    {
        case TOK_INC:
            printf("++");
            break;
        case TOK_DEC:
            printf("--");
            break;
        case TOK_EQUALITY:
            printf("==");
            break;
        case TOK_NOT_EQUALS:
            printf("!=");
            break;
        case TOK_LESS_EQUALS:
            printf("<=");
            break;
        case TOK_GREATER_EQUALS:
            printf(">=");
            break;
        case TOK_AND:
            printf("&&");
            break;
        case TOK_OR:
            printf("||");
            break;
        case TOK_USE:
        case TOK_USEPATH:
        case TOK_ATTR:
        case TOK_FUNC:
        case TOK_RETURN:
        case TOK_IDENT:
        case TOK_IF:
        case TOK_NUMERIC_LITERAL:
        case TOK_BIND:
        case TOK_ANNOTATE:
        case TOK_DOUBLE_ARROW:
        default:
            fprintf(stderr, "Unexpected operation type %i\n", op);
            break;
        }
}