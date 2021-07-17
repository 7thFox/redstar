#include "headers/syntaxfactory.h"
#define debugf(...) ;
// #define debugf(...) printf(__VA_ARGS__)

SyntaxArray init_array(size_t elem_size, uint16_t cap);
void *next_array_elem(SyntaxArray *arr);
StringIndex copy_string(SyntaxFactory *f, const char *str);
StringIndex copy_token_string(SyntaxFactory *f, Token *tok);

void factory_error(SyntaxFactory *f, const char *missing) {
    fprintf(stderr, "SYNTAX_FACTORY_ERROR in %s:%i:%i: Missing %s\n",
            get_string(f, f->current_filepath),
            f->tokens[*(f->current_token) + 1].p0.line,
            f->tokens[*(f->current_token) + 1].p0.col,
            missing);
    error_common();
}

#define assert(f, x, kind)         \
    if (!x)                        \
    {                              \
        factory_error(f, kind);    \
        return EMPTY_SYNTAX_INDEX; \
    }

#define assert_node(f, x, kind)    \
    if (!x.i)                      \
    {                              \
        factory_error(f, kind);    \
        return EMPTY_SYNTAX_INDEX; \
    }

SyntaxFactory *make_astfactory() {
    debugf("make_astfactory\n");
    SyntaxFactory *f = malloc(sizeof(SyntaxFactory));
    f->current_text = 0;
    f->current_filepath = (StringIndex){0};

    f->compilation_units = init_array(sizeof(AstCompilationUnit), 8);
    f->identifiers = init_array(sizeof(AstIdent), 128);
    f->types = init_array(sizeof(AstType), 16);
    f->param_list_decls = init_array(sizeof(AstParameterListDecl), 8);
    f->param_decls = init_array(sizeof(AstParameterDecl), 16);
    f->attr_lists = init_array(sizeof(AstAttrList), 4);
    f->param_lists = init_array(sizeof(AstParamList), 16);
    f->bind_anno_map = init_array(sizeof(AstBindAnnoMap), 32);

    f->statements = init_array(sizeof(_TypedIndex), 32);
    f->blocks = init_array(sizeof(AstBlock), 8);
    f->use_statements = init_array(sizeof(AstUseStatement), 8);
    f->attribute_declarations = init_array(sizeof(AstAttrDecl), 1);
    f->func_declarations = init_array(sizeof(AstFuncDecl), 8);
    f->return_statements = init_array(sizeof(AstReturnStatement), 8);
    f->local_decl_statements = init_array(sizeof(AstLocalDecl), 8);
    f->if_statements = init_array(sizeof(AstIfStatement), 8);
    f->annotate_statements = init_array(sizeof(AstAnnotateStatement), 8);
    f->bind_op_statements = init_array(sizeof(AstBindAnnoStatement), 8);
    f->bind_func_statements = init_array(sizeof(AstBindAnnoStatement), 8);
    f->anno_op_statements = init_array(sizeof(AstBindAnnoStatement), 8);
    f->anno_func_statements = init_array(sizeof(AstBindAnnoStatement), 8);

    f->expressions = init_array(sizeof(_TypedIndex), 32);
    f->binary_expressions = init_array(sizeof(AstBinaryOperationExpression), 8);
    f->literal_expressions = init_array(sizeof(AstLiteralExpression), 16);
    f->function_calls = init_array(sizeof(AstFunctionCall), 8);

    f->strings_capacity.i = 2048;
    f->strings_size.i = 0;
    f->strings = malloc(sizeof(char) * f->strings_capacity.i);

    return f;
}

void destroy_astfactory(SyntaxFactory *f) {
    debugf("destroy_astfactory\n");
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

    free(f->compilation_units.array);
    free(f->identifiers.array);
    free(f->types.array);
    free(f->param_list_decls.array);
    free(f->param_decls.array);
    free(f->param_lists.array);
    free(f->attr_lists.array);
    free(f->bind_anno_map.array);

    free(f->statements.array);
    free(f->blocks.array);
    free(f->use_statements.array);
    free(f->attribute_declarations.array);
    free(f->func_declarations.array);
    free(f->return_statements.array);
    free(f->local_decl_statements.array);
    free(f->if_statements.array);
    free(f->annotate_statements.array);
    free(f->bind_op_statements.array);
    free(f->bind_func_statements.array);
    free(f->anno_op_statements.array);
    free(f->anno_func_statements.array);

    free(f->expressions.array);
    free(f->binary_expressions.array);
    free(f->literal_expressions.array);
    free(f->function_calls.array);
}

SyntaxArray init_array(size_t elem_size, uint16_t cap) {
    SyntaxArray a;
    a.elem_size = elem_size;
    a.capacity = cap;
    a.size = 0;
    a.array = malloc(elem_size * cap);
    return a;
}

void *next_array_elem(SyntaxArray *arr) {
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
    size_t len = strlen(str) + 1;
    if (f->strings_size.i + len > f->strings_capacity.i) {
        StringIndex newcap = (StringIndex){f->strings_capacity.i < STRINGS_MAX_GROW
            ? f->strings_capacity.i * 2
            : f->strings_capacity.i + STRINGS_MAX_GROW};
        f->strings_capacity = newcap;
        f->strings = realloc(f->strings, sizeof(char) * f->strings_capacity.i);
    }
    StringIndex index = f->strings_size;
    strcpy(f->strings + index.i, str);
    f->strings[len - 1] = '\0';
    f->strings_size.i += len;
    
    return index;
}

StringIndex copy_token_string(SyntaxFactory *f, Token *tok) {
    size_t len = tok->p1.ind - tok->p0.ind + 1;
    if (f->strings_size.i + len > f->strings_capacity.i) {
        StringIndex newcap = (StringIndex){f->strings_capacity.i < STRINGS_MAX_GROW
            ? f->strings_capacity.i * 2
            : f->strings_capacity.i + STRINGS_MAX_GROW};
        f->strings_capacity = newcap;
        f->strings = realloc(f->strings, sizeof(char) * f->strings_capacity.i);
    }
    StringIndex index = f->strings_size;
    strncpy(f->strings + index.i, f->current_text + tok->p0.ind, len - 1);
    f->strings[index.i + tok->p1.ind] = '\0';
    f->strings_size.i += len;

    return index;
}

StatementIndex make_statement(SyntaxFactory *f, SyntaxIndex i, SyntaxKind k) {
    debugf("FFFFFFmake_statement %i = %i\n", f->statements.size + 1, i.i);
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
    unit->filepath = copy_string(f, file_path);
    unit->block = make_block(f);
    return (SyntaxIndex){f->compilation_units.size};
}

SyntaxIndex make_block(SyntaxFactory *f) {
    AstBlock *block = next_array_elem(&f->blocks);
    block->statements = init_array(sizeof(StatementIndex), 16);
    
    debugf("make_block\n");
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
    assert(f, name, "Ident Name");
    AstIdent *ident = next_array_elem(&f->identifiers);
    ident->name = copy_token_string(f, name);
    debugf("make_ident %i\n", f->identifiers.size);
    return (SyntaxIndex){f->identifiers.size};
}

SyntaxIndex make_attr_decl(SyntaxFactory *f, Token *attr, SyntaxIndex ident, SyntaxIndex block) {
    debugf("make_attr_decl %i\n", ident.i);
    assert(f, attr, "Attr Keyword");
    assert_node(f, ident, "Ident");
    AstAttrDecl *decl = next_array_elem(&f->attribute_declarations);
    decl->ident = ident;
    decl->block_opt = block;
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

    return (SyntaxIndex){f->func_declarations.size};
}

SyntaxIndex make_param_list_decl(SyntaxFactory *f) {
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
    assert_node(f, ident, "Ident");
    assert(f, colon, ":");
    assert_node(f, type, "Type");

    AstParameterDecl *decl = next_array_elem(&f->param_decls);
    decl->ident = ident;
    decl->type = type;

    return (SyntaxIndex){f->param_decls.size};
}

SyntaxIndex make_type(SyntaxFactory *f, SyntaxIndex attr_list_opt, SyntaxIndex ident) {
    assert_node(f, ident, "Type Ident");

    AstType *type = next_array_elem(&f->types);
    type->attr_list_opt = attr_list_opt;
    type->ident = ident;

    return (SyntaxIndex){f->types.size};
}

SyntaxIndex make_attr_list(SyntaxFactory *f) {
    AstAttrList *list = next_array_elem(&f->attr_lists);
    list->attributes = init_array(sizeof(SyntaxIndex), 1);
    return (SyntaxIndex){f->attr_lists.size};
}

SyntaxIndex add_attr(SyntaxFactory *f, SyntaxIndex attr_list, SyntaxIndex attr_ident) {
    if (attr_list.i && attr_ident.i) {
        AstAttrList *l = get_ast_node(attr_list, f->attr_lists);
        SyntaxIndex *i = next_array_elem(&l->attributes);
        i->i = attr_ident.i;
        return (SyntaxIndex){l->attributes.size};
    }
    return EMPTY_SYNTAX_INDEX;
}

SyntaxIndex make_binary_expression(SyntaxFactory *f, ExpressionIndex left, Token *op, ExpressionIndex right) {
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

    case TOK_EQUALITY:   exp->operation = BIN_EQUALS; break;
    case TOK_NOT_EQUALS: exp->operation = BIN_NOT_EQUALS; break;
    // case '%': new_priority = EXP_ADD; break;
    // case TOK_OR: new_priority = EXP_ADD; break;
    // case TOK_AND: new_priority = EXP_ADD; break;
    // case '|': new_priority = EXP_ADD; break;
    // case '&': new_priority = EXP_ADD; break;
    default:
        fprintf(stderr, "Unexpected binary operator '%c' (%i)\n", op->type, op->type);
        return EMPTY_SYNTAX_INDEX;
    }
#pragma GCC diagnostic pop

    return (SyntaxIndex){f->binary_expressions.size};
}

SyntaxIndex make_return_statement(SyntaxFactory *f, Token *return_token, ExpressionIndex expression) {
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

SyntaxIndex make_if_statement(SyntaxFactory *f, Token *if_tok, Token *left, ExpressionIndex cond, Token *right, StatementIndex stmt) {
    debugf("make_if_statement\n");
    assert(f, if_tok, "If Keyword");
    assert(f, left, "(");
    assert_node(f, cond, "Condition Expression");
    assert(f, right, ")");
    assert_node(f, stmt, "Statement");

    AstIfStatement *is = next_array_elem(&f->if_statements);
    is->condition = cond;
    is->statement = stmt;
    return (SyntaxIndex){f->if_statements.size};
}

SyntaxIndex make_literal_expression(SyntaxFactory *f, Token *token) {
    debugf("make_literal_expression\n");
    assert(f, token, "Literal Value");
    AstLiteralExpression *lit = next_array_elem(&f->literal_expressions);
    lit->string_value = copy_token_string(f, token);
    lit->type = token->type;
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

SyntaxIndex make_annotate_statement(SyntaxFactory *f, SyntaxIndex list, SyntaxIndex ident, Token *semi) {
    debugf("make_annotate_statement\n");
    assert_node(f, list, "Attr List");
    assert_node(f, ident, "Ident");
    assert(f, semi, ";");
    AstAnnotateStatement *a = next_array_elem(&f->annotate_statements);
    a->attr_list = list;
    a->ident = ident;
    return (SyntaxIndex){f->annotate_statements.size};
}

SyntaxIndex make_bind_operation_statement(SyntaxFactory *f, Token* bind, Token *op) {
    debugf("make_bind_operation_statement\n");
    assert(f, bind, "bind");
    assert(f, op, "Operation");

    AstBindAnnoStatement *s = next_array_elem(&f->bind_op_statements);
    s->parameters = init_array(sizeof(SyntaxIndex), 4);
    s->op_target = op->type;
    s->return_def = EMPTY_SYNTAX_INDEX;
    s->func_target = EMPTY_SYNTAX_INDEX;

    AstBindAnnoMap *m = next_array_elem(&f->bind_anno_map);
    m->arr = &f->bind_op_statements;
    m->index = (SyntaxIndex){f->bind_op_statements.size};
    return (SyntaxIndex){f->bind_anno_map.size};
}

SyntaxIndex make_bind_function_statement(SyntaxFactory *f, Token* bind, SyntaxIndex ident) {
    debugf("make_bind_function_statement\n");
    assert(f, bind, "bind");
    assert_node(f, ident, "function ident");

    AstBindAnnoStatement *s = next_array_elem(&f->bind_func_statements);
    s->parameters = init_array(sizeof(SyntaxIndex), 4);
    s->func_target = ident;
    s->return_def = EMPTY_SYNTAX_INDEX;
    s->op_target = 0;

    AstBindAnnoMap *m = next_array_elem(&f->bind_anno_map);
    m->arr = &f->bind_func_statements;
    m->index = (SyntaxIndex){f->bind_func_statements.size};
    return (SyntaxIndex){f->bind_anno_map.size};
}

SyntaxIndex make_anno_operation_statement(SyntaxFactory *f, Token* anno, Token *op) {
    debugf("make_bind_operation_statement\n");
    assert(f, anno, "anno");
    assert(f, op, "Operation");

    AstBindAnnoStatement *s = next_array_elem(&f->anno_op_statements);
    s->parameters = init_array(sizeof(SyntaxIndex), 4);
    s->op_target = op->type;
    s->return_def = EMPTY_SYNTAX_INDEX;
    s->func_target = EMPTY_SYNTAX_INDEX;

    AstBindAnnoMap *m = next_array_elem(&f->bind_anno_map);
    m->arr = &f->anno_op_statements;
    m->index = (SyntaxIndex){f->anno_op_statements.size};
    return (SyntaxIndex){f->bind_anno_map.size};
}

SyntaxIndex make_anno_function_statement(SyntaxFactory *f, Token* anno, SyntaxIndex ident) {
    debugf("make_bind_operation_statement\n");
    assert(f, anno, "anno");
    assert_node(f, ident, "function ident");

    AstBindAnnoStatement *s = next_array_elem(&f->anno_func_statements);
    s->parameters = init_array(sizeof(SyntaxIndex), 4);
    s->func_target = ident;
    s->return_def = EMPTY_SYNTAX_INDEX;
    s->op_target = 0;

    AstBindAnnoMap *m = next_array_elem(&f->bind_anno_map);
    m->arr = &f->anno_func_statements;
    m->index = (SyntaxIndex){f->anno_func_statements.size};
    return (SyntaxIndex){f->bind_anno_map.size};
}

SyntaxIndex add_bind_anno_definition(SyntaxFactory *f, SyntaxIndex stmt, SyntaxIndex def) {
    debugf("add_bind_anno_definition\n");
    if (stmt.i) {
        assert_node(f, def, "attribute list");

        AstBindAnnoMap *m = get_ast_node(stmt, f->bind_anno_map);
        AstBindAnnoStatement *s = get_ast_node(m->index, (*m->arr));
        SyntaxIndex *i = next_array_elem(&s->parameters);
        *i = def;
        return (SyntaxIndex){s->parameters.size};
    }
    return EMPTY_SYNTAX_INDEX;
}

SyntaxIndex add_bind_anno_definition_ignore(SyntaxFactory *f, SyntaxIndex stmt, Token *ignore) {
    debugf("add_bind_anno_definition_ignore\n");
    if (stmt.i) {
        assert(f, ignore, "_");

        AstBindAnnoMap *m = get_ast_node(stmt, f->bind_anno_map);
        AstBindAnnoStatement *s = get_ast_node(m->index, (*m->arr));
        SyntaxIndex *i = next_array_elem(&s->parameters);
        *i = EMPTY_SYNTAX_INDEX;
        return (SyntaxIndex){s->parameters.size};
    }
    return EMPTY_SYNTAX_INDEX;
}

void add_bind_anno_return(SyntaxFactory *f, SyntaxIndex stmt, Token *arrow, SyntaxIndex return_def){
    debugf("add_bind_anno_definition_ignore\n");
    if (stmt.i) {
        if (!arrow) {
            factory_error(f, "=>");
        }
        if (!return_def.i) {
            factory_error(f, "attribute list");
        }

        AstBindAnnoMap *m = get_ast_node(stmt, f->bind_anno_map);
        AstBindAnnoStatement *s = get_ast_node(m->index, (*m->arr));
        s->return_def = stmt;
    }
}