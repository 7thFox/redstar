#include "headers/syntaxfactory.h"
#define debugf(...) ;
// #define debugf(...) printf("=>"); printf(__VA_ARGS__)

DynamicArray init_array(size_t elem_size, uint16_t cap);
void *next_array_elem(DynamicArray *arr);
StringIndex copy_token_string(SyntaxFactory *f, Token *tok);
void add_ident_to_decls(SyntaxFactory *f, SyntaxIndex ident);

void factory_error(SyntaxFactory *f, const char *msg) {
    fprintf(stderr, "SYNTAX_FACTORY_ERROR in %s:%i:%i: %s\n",
            get_string(f->string_table, f->current_filepath),
            f->tokens[*(f->current_token) + 1].p0.line,
            f->tokens[*(f->current_token) + 1].p0.col,
            msg);
    error_common();
}

void factory_error_missing(SyntaxFactory *f, const char *missing) {
    fprintf(stderr, "SYNTAX_FACTORY_ERROR in %s:%i:%i: Missing: %s\n",
            get_string(f->string_table, f->current_filepath),
            f->tokens[*(f->current_token) + 1].p0.line,
            f->tokens[*(f->current_token) + 1].p0.col,
            missing);
    error_common();
}

#define assert(f, x, kind)              \
    if (!x)                             \
    {                                   \
        factory_error_missing(f, kind); \
        return EMPTY_SYNTAX_INDEX;      \
    }

#define assert_node(f, x, kind)         \
    if (!x.i)                           \
    {                                   \
        factory_error_missing(f, kind); \
        return EMPTY_SYNTAX_INDEX;      \
    }

SyntaxFactory *make_astfactory() {
    debugf("make_astfactory\n");
    SyntaxFactory *f = malloc(sizeof(SyntaxFactory));
    f->current_text = 0;
    f->current_filepath = (StringIndex){0};
    f->string_table = init_string_table();

    f->scopes = init_array(sizeof(STScope), 64);
    f->ident_decls = init_array(sizeof(SyntaxIndex), 32);

    f->compilation_units = init_array(sizeof(AstCompilationUnit), 8);
    f->identifiers = init_array(sizeof(AstIdent), 128);
    f->types = init_array(sizeof(AstType), 16);
    f->param_list_decls = init_array(sizeof(AstParameterListDecl), 8);
    f->param_decls = init_array(sizeof(AstParameterDecl), 16);
    f->attr_lists = init_array(sizeof(AstAttrList), 4);
    f->attr_lists_elems = init_array(sizeof(AstAttrListElem), 16);
    f->param_lists = init_array(sizeof(AstParamList), 16);
    f->bind_anno_map = init_array(sizeof(AstBindAnnoMap), 32);
    f->bind_anno_ordinals = init_array(sizeof(AstBindAnnoOrdinals), 32);

    f->statements = init_array(sizeof(_TypedIndex), 32);
    f->blocks = init_array(sizeof(AstBlock), 8);
    f->use_statements = init_array(sizeof(AstUseStatement), 8);
    f->attribute_declarations = init_array(sizeof(AstAttrDecl), 1);
    f->func_declarations = init_array(sizeof(AstFuncDecl), 8);
    f->return_statements = init_array(sizeof(AstReturnStatement), 8);
    f->local_decl_statements = init_array(sizeof(AstLocalDecl), 8);
    f->if_statements = init_array(sizeof(AstIfStatement), 8);
    f->annotate_statements = init_array(sizeof(AstAnnotateStatement), 8);

    f->bind_op_ordinal_statements = init_array(sizeof(AstBindAnnoStatement), 4);
    f->bind_op_ordinal_return_statements = init_array(sizeof(AstBindAnnoStatement), 4);
    f->bind_op_wild_statements = init_array(sizeof(AstBindAnnoStatement), 4);
    f->bind_op_wild_return_statements = init_array(sizeof(AstBindAnnoStatement), 4);
    f->bind_func_ordinal_statements = init_array(sizeof(AstBindAnnoStatement), 4);
    f->bind_func_ordinal_return_statements = init_array(sizeof(AstBindAnnoStatement), 4);
    f->bind_func_wild_statements = init_array(sizeof(AstBindAnnoStatement), 4);
    f->bind_func_wild_return_statements = init_array(sizeof(AstBindAnnoStatement), 4);
    f->anno_op_ordinal_return_statements = init_array(sizeof(AstBindAnnoStatement), 4);
    f->anno_op_wild_return_statements = init_array(sizeof(AstBindAnnoStatement), 4);
    f->anno_func_ordinal_return_statements = init_array(sizeof(AstBindAnnoStatement), 4);
    f->anno_func_wild_return_statements = init_array(sizeof(AstBindAnnoStatement), 4);
    f->bind_wild_return_statements = init_array(sizeof(AstBindAnnoStatement), 4);
    f->bind_wild_statements = init_array(sizeof(AstBindAnnoStatement), 4);
    f->anno_wild_return_statements = init_array(sizeof(AstBindAnnoStatement), 4);

    f->expressions = init_array(sizeof(_TypedIndex), 32);
    f->binary_expressions = init_array(sizeof(AstBinaryOperationExpression), 8);
    f->literal_expressions = init_array(sizeof(AstLiteralExpression), 16);
    f->function_calls = init_array(sizeof(AstFunctionCall), 8);

    return f;
}

void destroy_astfactory(SyntaxFactory *f) {
    debugf("destroy_astfactory\n");
    destroy_string_table(&f->string_table);

    for (int i = 0; i < f->param_list_decls.size; i++) {
        free(((AstParameterListDecl *)f->param_list_decls.array)[i].param_decls.array);
    }
    for (int i = 0; i < f->attr_lists.size; i++) {
        free(((AstAttrList *)f->attr_lists.array)[i].attributes.array);
    }
    for (int i = 0; i < f->blocks.size; i++) {
        free(((AstBlock *)f->blocks.array)[i].statements.array);
    }
    for (int i = 0; i < f->param_lists.size; i++) {
        free(((AstParamList *)f->param_lists.array)[i].param_expressions.array);
    }
    for (int i = 0; i < f->bind_anno_ordinals.size; i++) {
        free(((AstBindAnnoOrdinals *)f->bind_anno_ordinals.array)[i].ordinals.array);
    }
    for (int i = 0; i < f->annotate_statements.size; i++) {
        free(((AstAnnotateStatement *)f->annotate_statements.array)[i].ident_list.array);
    }

    free(f->scopes.array);
    free(f->ident_decls.array);

    free(f->compilation_units.array);
    free(f->identifiers.array);
    free(f->types.array);
    free(f->param_list_decls.array);
    free(f->param_decls.array);
    free(f->param_lists.array);
    free(f->attr_lists.array);
    free(f->attr_lists_elems.array);
    free(f->bind_anno_map.array);
    free(f->bind_anno_ordinals.array);

    free(f->statements.array);
    free(f->blocks.array);
    free(f->use_statements.array);
    free(f->attribute_declarations.array);
    free(f->func_declarations.array);
    free(f->return_statements.array);
    free(f->local_decl_statements.array);
    free(f->if_statements.array);
    free(f->annotate_statements.array);

    free(f->bind_op_ordinal_statements.array);
    free(f->bind_op_ordinal_return_statements.array);
    free(f->bind_op_wild_statements.array);
    free(f->bind_op_wild_return_statements.array);
    free(f->bind_func_ordinal_statements.array);
    free(f->bind_func_ordinal_return_statements.array);
    free(f->bind_func_wild_statements.array);
    free(f->bind_func_wild_return_statements.array);
    free(f->anno_op_ordinal_return_statements.array);
    free(f->anno_op_wild_return_statements.array);
    free(f->anno_func_ordinal_return_statements.array);
    free(f->anno_func_wild_return_statements.array);
    free(f->bind_wild_return_statements.array);
    free(f->bind_wild_statements.array);
    free(f->anno_wild_return_statements.array);

    free(f->expressions.array);
    free(f->binary_expressions.array);
    free(f->literal_expressions.array);
    free(f->function_calls.array);
}

void push_scope(SyntaxFactory *f, SyntaxIndex block) {
    STScope *s = next_array_elem(&f->scopes);
    s->id_parent = f->current_scope;
    s->block = block;
    s->comp_unit = EMPTY_SYNTAX_INDEX;

    f->current_scope = (ScopeId){f->scopes.size};
}

void pop_scope(SyntaxFactory *f) {
    STScope *current = get_ast_node(f->current_scope, f->scopes);
    f->current_scope = current->id_parent;
}

void add_ident_to_decls(SyntaxFactory *f, SyntaxIndex ident) {
    SyntaxIndex *i = next_array_elem(&f->ident_decls);
    i->i = ident.i;
}

StringIndex copy_token_string(SyntaxFactory *f, Token *tok) {
    return copy_string(&f->string_table, f->current_text + tok->p0.ind, tok->p1.ind - tok->p0.ind);
}

StatementIndex make_statement(SyntaxFactory *f, SyntaxIndex i, SyntaxKind k) {
    debugf("make_statement %i = %i\n", f->statements.size + 1, i.i);
    _TypedIndex *index = next_array_elem(&f->statements);
    index->index.i = i.i;
    index->kind = k;

    return (StatementIndex){f->statements.size};
}

ExpressionIndex make_expression(SyntaxFactory *f, SyntaxIndex i, SyntaxKind k) {
    // debugf("make_expression %i\n", i.i);
    _TypedIndex *index = next_array_elem(&f->expressions);
    index->index.i = i.i;
    index->kind = k;

    return (ExpressionIndex){f->expressions.size};
}

SyntaxIndex make_compilation_unit(SyntaxFactory *f, LexResult *lex, const char *file_path) {
    debugf("make_compilation_unit\n");
    // TODO: Search for referenced comp unit first
    f->current_text = lex->text;
    AstCompilationUnit *unit = next_array_elem(&(f->compilation_units));
    unit->filepath = copy_string(&f->string_table, file_path, strlen(file_path));
    unit->block = make_block(f);
    return (SyntaxIndex){f->compilation_units.size};
}

SyntaxIndex make_block(SyntaxFactory *f) {
    debugf("make_block\n");
    AstBlock *block = next_array_elem(&f->blocks);
    block->statements = init_array(sizeof(StatementIndex), 16);
    return (SyntaxIndex){f->blocks.size};
}

SyntaxIndex add_statement(SyntaxFactory *f, SyntaxIndex block, StatementIndex stmt) {
    if (block.i && stmt.i) {
        AstBlock *b = get_ast_node(block, f->blocks);
        StatementIndex *si = next_array_elem(&b->statements);
        si->i = stmt.i;
        return (SyntaxIndex){b->statements.size};
    }
    return EMPTY_SYNTAX_INDEX;
}

SyntaxIndex make_use_statement(SyntaxFactory *f, Token *use_tok, Token *name) {
    debugf("make_use_statement\n");
    assert(f, use_tok, "Use Keyword");
    assert(f, name, "Use Path");
    AstUseStatement *use = next_array_elem(&(f->use_statements));
    use->referenced_compilation_unit = EMPTY_SYNTAX_INDEX;// TODO: Create future comp unit
    return (SyntaxIndex){f->use_statements.size};
}

SyntaxIndex make_ident(SyntaxFactory *f, Token *name) {
    debugf("make_ident\n");
    assert(f, name, "Ident Name");
    AstIdent *ident = next_array_elem(&f->identifiers);
    ident->name = copy_token_string(f, name);
    ident->scope = f->current_scope;
    return (SyntaxIndex){f->identifiers.size};
}

SyntaxIndex make_attr_decl(SyntaxFactory *f, Token *attr, SyntaxIndex ident, SyntaxIndex block) {
    debugf("make_attr_decl %i\n", ident.i);
    assert(f, attr, "Attr Keyword");
    assert_node(f, ident, "Ident");
    AstAttrDecl *decl = next_array_elem(&f->attribute_declarations);
    decl->ident = ident;
    decl->block_opt = block;
    add_ident_to_decls(f, ident);
    return (SyntaxIndex){f->attribute_declarations.size};
}

SyntaxIndex make_func_decl(SyntaxFactory *f,
    Token *func,
    SyntaxIndex ident,
    Token *left_paran,
    SyntaxIndex param_list_opt,
    Token *right_paran,
    SyntaxIndex return_type_opt,
    SyntaxIndex block)
{
    assert(f, func, "Func Keyword");
    assert_node(f, ident, "Ident");
    assert(f, left_paran, "Left Parenthesis");
    assert(f, right_paran, "Right Parenthesis");
    assert_node(f, block, "Block");

    AstFuncDecl *decl = next_array_elem(&f->func_declarations);
    decl->ident = ident;
    decl->param_list_opt = param_list_opt;
    decl->return_type_opt = return_type_opt;
    decl->block = block;
    add_ident_to_decls(f, ident);

    return (SyntaxIndex){f->func_declarations.size};
}

SyntaxIndex make_param_list_decl(SyntaxFactory *f) {
    debugf("make_param_list_decl\n");
    AstParameterListDecl *list = next_array_elem(&f->param_list_decls);
    list->param_decls = init_array(sizeof(SyntaxIndex), 4);

    return (SyntaxIndex){f->param_list_decls.size};
}

SyntaxIndex add_param_decl(SyntaxFactory *f, SyntaxIndex param_list, SyntaxIndex param_decl) {
    if (param_list.i && param_decl.i) {
        AstParameterListDecl *l = get_ast_node(param_list, f->param_list_decls);
        SyntaxIndex *i = next_array_elem(&l->param_decls);
        i->i = param_decl.i;
        return (SyntaxIndex){l->param_decls.size};
    }
    return EMPTY_SYNTAX_INDEX;
}

SyntaxIndex make_param_decl(SyntaxFactory *f, SyntaxIndex ident, Token *colon, SyntaxIndex type) {
    debugf("make_param_decl\n");
    assert_node(f, ident, "Ident");
    assert(f, colon, ":");
    assert_node(f, type, "Type");

    AstParameterDecl *decl = next_array_elem(&f->param_decls);
    decl->ident = ident;
    decl->type = type;
    add_ident_to_decls(f, ident);
    return (SyntaxIndex){f->param_decls.size};
}

SyntaxIndex make_type(SyntaxFactory *f, SyntaxIndex attr_list_opt, SyntaxIndex ident) {
    debugf("make_type\n");
    assert_node(f, ident, "Type Ident");

    AstType *type = next_array_elem(&f->types);
    type->attr_list_opt = attr_list_opt;
    type->ident = ident;

    return (SyntaxIndex){f->types.size};
}

SyntaxIndex make_attr_list(SyntaxFactory *f) {
    debugf("make_attr_list\n");
    AstAttrList *list = next_array_elem(&f->attr_lists);
    list->attributes = init_array(sizeof(SyntaxIndex), 4);
    return (SyntaxIndex){f->attr_lists.size};
}

SyntaxIndex make_attr_list_elem(SyntaxFactory *f, Token *remove_opt, SyntaxIndex ident) {
    debugf("make_attr_list_elem\n");
    assert_node(f, ident, "attribute ident");
    AstAttrListElem *e = next_array_elem(&f->attr_lists_elems);
    e->ident = ident;
    e->remove = remove_opt != 0;
    return (SyntaxIndex){f->attr_lists_elems.size};
}

SyntaxIndex add_attr(SyntaxFactory *f, SyntaxIndex attr_list, SyntaxIndex elem) {
    if (attr_list.i && elem.i) {
        AstAttrList *l = get_ast_node(attr_list, f->attr_lists);
        SyntaxIndex *i = next_array_elem(&l->attributes);
        i->i = elem.i;
        return (SyntaxIndex){l->attributes.size};
    }
    return EMPTY_SYNTAX_INDEX;
}

SyntaxIndex make_binary_expression(SyntaxFactory *f, ExpressionIndex left, Token *op, ExpressionIndex right) {
    debugf("make_binary_expression\n");
    assert_node(f, left, "Left Binary Expression Value");
    assert(f, op, "Binary Expression Operation");
    assert_node(f, right, "Right Binary Expression Value");

    AstBinaryOperationExpression *exp = next_array_elem(&f->binary_expressions);
    exp->expression_left = left;
    exp->expression_right = right;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch"
    switch (op->type)
    {
    case '+': exp->operation = BIN_ADD; break;
    case '-': exp->operation = BIN_MINUS; break;
    case '*': exp->operation = BIN_MULTIPLY; break;
    case '/': exp->operation = BIN_DIVIDE; break;
    case '%': exp->operation = BIN_MODULUS; break;

    case TOK_EQUALITY:   exp->operation = BIN_EQUALS; break;
    case TOK_NOT_EQUALS: exp->operation = BIN_NOT_EQUALS; break;
    default:
        {
            char err[256];
            snprintf(err, 256, "Unexpected binary operator '%c' (%i)\n", op->type, op->type);
            factory_error(f, err);
        }
        return EMPTY_SYNTAX_INDEX;
    }
#pragma GCC diagnostic pop

    return (SyntaxIndex){f->binary_expressions.size};
}

SyntaxIndex make_return_statement(SyntaxFactory *f, Token *return_token, ExpressionIndex expression) {
    debugf("make_return_statement\n");
    assert(f, return_token, "Return Keyword");
    assert_node(f, expression, "Expression");

    AstReturnStatement *smt = next_array_elem(&f->return_statements);
    smt->expression = expression;
    return (SyntaxIndex){f->return_statements.size};
}

SyntaxIndex make_local_decl(SyntaxFactory *f,
    SyntaxIndex ident, Token *colon, SyntaxIndex type_opt,
    Token *equals_opt, ExpressionIndex expresion_opt,
    Token *semicolon)
{
    assert_node(f, ident, "Ident");
    assert(f, colon, ":");
    if (!type_opt.i) {
        assert(f, equals_opt, "Assignment or Explicit Type");
    }
    if (equals_opt) {
        assert_node(f, expresion_opt, "Expression for Assignment");
    }
    assert(f, semicolon, ";");

    AstLocalDecl *decl = next_array_elem(&f->local_decl_statements);
    decl->ident = ident;
    decl->type_opt = type_opt;
    decl->expression_opt = expresion_opt;
    add_ident_to_decls(f, ident);

    return (SyntaxIndex){f->local_decl_statements.size};
}

SyntaxIndex make_function_call(SyntaxFactory *f,
    ExpressionIndex expression, Token *left, SyntaxIndex param_list_opt, Token *right) {
    debugf("make_function_call_expression\n");
    assert_node(f, expression, "Expression");
    assert(f, left, "(");
    assert(f, right, ")");
    AstFunctionCall *func = next_array_elem(&f->function_calls);
    func->expression_left = expression;
    func->parameters_opt = param_list_opt;
    return (SyntaxIndex){f->function_calls.size};
}

SyntaxIndex make_if_statement(SyntaxFactory *f,
    Token *if_tok, Token *left, ExpressionIndex cond, Token *right,
    StatementIndex stmt,
    Token *else_opt, StatementIndex else_stmt_opt) {
    debugf("make_if_statement\n");
    assert(f, if_tok, "ff keyword");
    assert(f, left, "(");
    assert_node(f, cond, "condition expression");
    assert(f, right, ")");
    assert_node(f, stmt, "statement");
    if (else_opt) {
        assert_node(f, else_stmt_opt, "else statement")
    }

    AstIfStatement *is = next_array_elem(&f->if_statements);
    is->condition = cond;
    is->statement = stmt;
    is->else_statement = else_stmt_opt;
    return (SyntaxIndex){f->if_statements.size};
}

SyntaxIndex make_literal_expression(SyntaxFactory *f, Token *token) {
    debugf("make_literal_expression\n");
    assert(f, token, "Literal Value");
    AstLiteralExpression *lit = next_array_elem(&f->literal_expressions);
    lit->type = token->type;
    switch (token->type) {
        case TOK_NUMERIC_LITERAL:// TODO: float v int
        {
            char buff[21];
            strncpy(buff, f->current_text + token->p0.ind, token->p1.ind - token->p0.ind);
            lit->int_value = atol(buff);
            break;
        }
        default:
            factory_error(f, "Unexpected literal type");
        }
    // lit->string_value = copy_token_string(f, token);
    return (SyntaxIndex){f->literal_expressions.size};
}

SyntaxIndex make_param_list(SyntaxFactory *f) {
    debugf("make_param_list\n");
    AstParamList *list = next_array_elem(&f->param_lists);
    list->param_expressions = init_array(sizeof(ExpressionIndex), 1);
    return (SyntaxIndex){f->param_lists.size};
}

SyntaxIndex add_param(SyntaxFactory *f, SyntaxIndex list, ExpressionIndex exp) {
    debugf("add_param\n");
    if (list.i) {
        assert_node(f, exp, "Expression");
        AstParamList *l = get_ast_node(list, f->param_lists);
        ExpressionIndex *i = next_array_elem(&l->param_expressions);
        i->i = exp.i;
        return (SyntaxIndex){l->param_expressions.size};
    }
    return EMPTY_SYNTAX_INDEX;
}

SyntaxIndex make_annotate_statement(SyntaxFactory *f, SyntaxIndex list) {
    debugf("make_annotate_statement\n");
    assert_node(f, list, "Attr List");
    AstAnnotateStatement *a = next_array_elem(&f->annotate_statements);
    a->attr_list = list;
    a->ident_list = init_array(sizeof(SyntaxIndex), 8);
    return (SyntaxIndex){f->annotate_statements.size};
}

SyntaxIndex add_ident_to_annotate(SyntaxFactory *f, SyntaxIndex annotate, SyntaxIndex ident) {
    if (annotate.i) {
        assert_node(f, ident, "Ident");
        AstAnnotateStatement *a = get_ast_node(annotate, f->annotate_statements);
        SyntaxIndex *i = next_array_elem(&a->ident_list);
        i->i = ident.i;
        return (SyntaxIndex){a->ident_list.size};
    }
    return EMPTY_SYNTAX_INDEX;
}

SyntaxIndex make_bind_anno_statement(SyntaxFactory *f,
    BindAnnoStatementKind kind,
    Token *bind_anno,
    SyntaxIndex func, Token *op, Token *wild_target,
    Token *wildcard_kind, SyntaxIndex wildcard_def, SyntaxIndex ordinals,
    Token *arrow_opt, SyntaxIndex return_def_opt)
{
    debugf("make_bind_anno_statement\n");
    DynamicArray *arr;
    switch (kind)
    {
        case _BAS_BIND_OP_ORDINAL:
            arr = &f->bind_op_ordinal_statements;
            break;
        case _BAS_BIND_OP_ORDINAL_RETURN:
            arr = &f->bind_op_ordinal_return_statements;
            break;
        case _BAS_BIND_OP_WILD:
            arr = &f->bind_op_wild_statements;
            break;
        case _BAS_BIND_OP_WILD_RETURN:
            arr = &f->bind_op_wild_return_statements;
            break;
        case _BAS_BIND_FUNC_ORDINAL:
            arr = &f->bind_func_ordinal_statements;
            break;
        case _BAS_BIND_FUNC_ORDINAL_RETURN:
            arr = &f->bind_func_ordinal_return_statements;
            break;
        case _BAS_BIND_FUNC_WILD:
            arr = &f->bind_func_wild_statements;
            break;
        case _BAS_BIND_FUNC_WILD_RETURN:
            arr = &f->bind_func_wild_return_statements;
            break;
        case _BAS_BIND_WILD:
            arr = &f->bind_wild_statements;
            break;
        case _BAS_BIND_WILD_RETURN:
            arr = &f->bind_wild_return_statements;
            break;
        case _BAS_ANNO_OP_ORDINAL_RETURN:
            arr = &f->anno_op_ordinal_return_statements;
            break;
        case _BAS_ANNO_OP_WILD_RETURN:
            arr = &f->anno_op_wild_return_statements;
            break;
        case _BAS_ANNO_FUNC_ORDINAL_RETURN:
            arr = &f->anno_func_ordinal_return_statements;
            break;
        case _BAS_ANNO_FUNC_WILD_RETURN:
            arr = &f->anno_func_wild_return_statements;
            break;
        case _BAS_ANNO_WILD_RETURN:
            arr = &f->anno_wild_return_statements;
            break;
        default:
            if ((kind & BAS_ANNO) == BAS_ANNO &&
                (kind & BAS_RETURN) != BAS_RETURN)
            {
                factory_error(f, "Annos must define a non-empty return");
            }
            else {
                factory_error(f, "Malformed Bind/Anno Kind");
            }
            return EMPTY_SYNTAX_INDEX;
        }

    AstBindAnnoStatement *s = next_array_elem(arr);

    assert(f, bind_anno, "bind/anno");
    bool mismatch = true;
    switch (kind & 0x11)
    {
        case BAS_BIND:
            mismatch = bind_anno->type != TOK_BIND;
            break;
        case BAS_ANNO:
            mismatch = bind_anno->type != TOK_ANNOTATE;
            break;
        default:
            factory_error(f, "Unexpected Bind/Anno type");
            break;
    }
    if (mismatch) {
        factory_error(f, "Bind/anno token does not match kind");
    }

    switch (kind & (0x111 << 2))
    {
        case BAS_TARGET_FUNC:
            assert_node(f, func, "func ident");
            s->func_target = func;
            s->op_target = TOK_NULL;
            break;
        case BAS_TARGET_OP:
            assert(f, op, "operation");
            s->op_target = op->type;
            s->func_target = EMPTY_SYNTAX_INDEX;
            break;
        case BAS_TARGET_WILD:
            assert(f, wild_target, "wildcard target");
            s->op_target = TOK_NULL;
            s->func_target = EMPTY_SYNTAX_INDEX;
            break;
        default:
            factory_error(f, "Unexpected target type");
            break;
    }

    switch (kind & (0x11 << 5))
    {
        case BAS_DEFS_ORDINAL:
            assert_node(f, ordinals, "ordinal list");
            s->ordinals = ordinals;
            s->wildcard = TOK_NULL;
            s->wildcard_list = EMPTY_SYNTAX_INDEX;
            break;
        case BAS_DEFS_WILD:
            assert(f, wildcard_kind, "wildcard def type");
            if (wildcard_kind->type != TOK_ANY &&
                wildcard_kind->type != TOK_ALL &&
                wildcard_kind->type != TOK_NONE)
            {
                factory_error(f, "Invalid wildcard def token");
            }
            s->wildcard = wildcard_kind->type;
            s->wildcard_list = wildcard_def;
            break;
        default:
            s->ordinals = EMPTY_SYNTAX_INDEX;
            s->wildcard = TOK_NULL;
            s->wildcard_list = EMPTY_SYNTAX_INDEX;
            factory_error(f, "Unexpected definition type");
            break;
    }

    if ((kind & BAS_RETURN) == BAS_RETURN) {
        assert(f, arrow_opt, "=>");
        assert_node(f, return_def_opt, "return attr list");
        s->return_def = return_def_opt;
    }

    AstBindAnnoMap *map = next_array_elem(&f->bind_anno_map);
    map->arr = arr;
    map->kind = kind;
    map->index = (SyntaxIndex){arr->size};

    return (SyntaxIndex){f->bind_anno_map.size};
}

SyntaxIndex make_bind_anno_ordinals(SyntaxFactory *f) {
    debugf("make_bind_anno_ordinals\n");
    AstBindAnnoOrdinals *o = next_array_elem(&f->bind_anno_ordinals);
    o->ordinals = init_array(sizeof(SyntaxIndex), 8);
    return (SyntaxIndex){f->bind_anno_ordinals.size};
}

SyntaxIndex add_bind_anno_ordinal(SyntaxFactory *f, SyntaxIndex ordinal_list, SyntaxIndex attr_list_opt) {
    if (ordinal_list.i) {
        AstBindAnnoOrdinals *o = get_ast_node(ordinal_list, f->bind_anno_ordinals);
        SyntaxIndex *i = next_array_elem(&o->ordinals);
        i->i = attr_list_opt.i;
        return (SyntaxIndex){o->ordinals.size};
    }
    return EMPTY_SYNTAX_INDEX;
}
