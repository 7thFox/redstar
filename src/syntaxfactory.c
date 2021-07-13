#include "headers/syntaxfactory.h"
#define debugf(...) ;
// #define debugf(...) printf(__VA_ARGS__)

SyntaxArray init_array(size_t elem_size, uint16_t cap);
void *next_array_elem(SyntaxArray *arr);
StringIndex copy_string(SyntaxFactory *f, const char *str);
StringIndex copy_token_string(SyntaxFactory *f, Token *tok);
// void set_last_location(SyntaxFactory *f, Token *x);
SyntaxIndex add_statement_index(SyntaxFactory *f, SyntaxIndex i, SyntaxKind k);
SyntaxIndex make_expression_index(SyntaxFactory *f, SyntaxIndex i, SyntaxKind k);

#define assert(f, x, kind)                                  \
    if (!x)                                                 \
    {                                                       \
        fprintf(stderr, "%s:%i:%i: Missing %s\n",           \
                f->strings + f->current_unit->filepath,     \
                f->tokens[*(f->current_token) + 1].p0.line, \
                f->tokens[*(f->current_token) + 1].p0.col,  \
                kind);                                      \
        return EMPTY_SYNTAX_INDEX;                          \
    }

#define assert_node(f, x, kind)                             \
    if (!x)                                                 \
    {                                                       \
        fprintf(stderr, "%s:%i:%i: Missing %s\n",           \
                f->strings + f->current_unit->filepath,     \
                f->tokens[*(f->current_token) + 1].p0.line, \
                f->tokens[*(f->current_token) + 1].p0.col,  \
                kind);                                      \
        return EMPTY_SYNTAX_INDEX;                          \
    }

SyntaxFactory *make_astfactory() {
    // debugf(VERBOSITY_NORMAL, "make_astfactory\n");
    SyntaxFactory *f = malloc(sizeof(SyntaxFactory));
    f->current_text = 0;
    f->current_unit = 0;
    f->current_block = 0;
    f->current_block = 0;

    f->compilation_units = init_array(sizeof(AstCompilationUnit), 8);
    f->identifiers = init_array(sizeof(AstIdent), 128);
    f->types = init_array(sizeof(AstType), 16);
    f->param_list_decls = init_array(sizeof(AstParameterListDecl), 8);
    f->param_decls = init_array(sizeof(AstParameterDecl), 16);
    f->attr_lists = init_array(sizeof(AstAttrList), 4);

    f->statements = init_array(sizeof(TypedIndex), 32);
    f->blocks = init_array(sizeof(AstBlock), 8);
    f->use_statements = init_array(sizeof(AstUseStatement), 8);
    f->attribute_declarations = init_array(sizeof(AstAttrDecl), 1);
    f->func_declarations = init_array(sizeof(AstFuncDecl), 8);
    f->return_statements = init_array(sizeof(AstReturnStatement), 8);
    f->local_decl_statements = init_array(sizeof(AstLocalDecl), 8);
    f->function_call_statements = init_array(sizeof(AstFunctionCallStatement), 8);
    f->if_statements = init_array(sizeof(AstIfStatement), 8);

    f->expressions = init_array(sizeof(TypedIndex), 32);
    f->binary_expressions = init_array(sizeof(AstBinaryOperationExpression), 8);
    f->function_call_expressions = init_array(sizeof(AstFunctionCallExpression), 16);
    f->literal_expressions = init_array(sizeof(AstLiteralExpression), 16);
    f->param_lists = init_array(sizeof(AstParamList), 16);

    f->strings_capacity = 2048;
    f->strings_size = 0;
    f->strings = malloc(sizeof(char) * f->strings_capacity);

    return f;
}

void destroy_astfactory(SyntaxFactory *f) {
    for (int i = 0; i < f->blocks.size; i++) {
        free(((AstBlock *)f->blocks.array)[i].statements.array);
    }
    free(f->blocks.array);

    for (int i = 0; i < f->param_lists.size; i++) {
        free(((AstParamList *)f->param_lists.array)[i].param_expressions.array);
    }
    free(f->param_lists.array);

    for (int i = 0; i < f->attr_lists.size; i++) {
        free(((AstAttrList *)f->attr_lists.array)[i].attributes.array);
    }
    free(f->attr_lists.array);

    free(f->strings);
    free(f->compilation_units.array);

    free(f->statements.array);
    free(f->function_call_statements.array);
    free(f->attribute_declarations.array);
    free(f->use_statements.array);
    free(f->func_declarations.array);
    free(f->local_decl_statements.array);
    free(f->if_statements.array);

    free(f->function_call_expressions.array);
    free(f->param_list_decls.array);
    free(f->param_decls.array);
    free(f->types.array);
    free(f->identifiers.array);
    free(f->literal_expressions.array);
}

SyntaxArray init_array(size_t elem_size, uint16_t cap) {
    SyntaxArray a;
    a.elem_size = elem_size;
    a.capacity = cap;
    a.size = 0;
    a.array = malloc(elem_size * cap);
    // debugf(VERBOSITY_HIGH, "init_array: %x\n", a.array);
    return a;
}

// void set_last_location(SyntaxFactory *f, Token *x) {
//     f->last_location.line = x->p1.line;
//     f->last_location.col = x->p1.col;
// }

void *next_array_elem(SyntaxArray *arr) {
    // debugf(VERBOSITY_HIGH, "next_array_elem");
    // debugf(VERBOSITY_HIGH, ": %x (%i/%i)x%i\n", arr->array, arr->size, arr->capacity, arr->elem_size);
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
    if (f->strings_size + len > f->strings_capacity) {
        StringIndex newcap = f->strings_capacity < STRINGS_MAX_GROW
            ? f->strings_capacity * 2
            : f->strings_capacity + STRINGS_MAX_GROW;
        f->strings_capacity = newcap;
        f->strings = realloc(f->strings, sizeof(char) * f->strings_capacity);
    }
    StringIndex index = f->strings_size;
    strcpy(f->strings + index, str);
    f->strings[len - 1] = '\0';
    f->strings_size += len;
    return index;
}

StringIndex copy_token_string(SyntaxFactory *f, Token *tok) {
    // debugf(VERBOSITY_HIGH, "copy_token_string\n");
    size_t len = tok->p1.ind - tok->p0.ind + 1;
    if (f->strings_size + len > f->strings_capacity) {
        StringIndex newcap = f->strings_capacity < STRINGS_MAX_GROW
            ? f->strings_capacity * 2
            : f->strings_capacity + STRINGS_MAX_GROW;
        f->strings_capacity = newcap;
        f->strings = realloc(f->strings, sizeof(char) * f->strings_capacity);
    }
    StringIndex index = f->strings_size;
    strncpy(f->strings + index, f->current_text + tok->p0.ind, len - 1);
    f->strings[index + tok->p1.ind] = '\0';
    f->strings_size += len;

    return index;
}

SyntaxIndex add_statement_index(SyntaxFactory *f, SyntaxIndex i, SyntaxKind k) {
    // debugf(VERBOSITY_HIGH, "add_statement_index %x + %i\n", f->statements.array, i);
    TypedIndex *index = next_array_elem(&f->statements);
    index->index = i;
    index->kind = k;

    if (f->current_block) {
        debugf("add_statement_index current_block %p (%i)\n", (void*)f->current_block, i);
        TypedIndex *block_index = next_array_elem(& f->current_block->statements);
        *block_index = *index;
    }

    return i;
}

SyntaxIndex make_expression_index(SyntaxFactory *f, SyntaxIndex i, SyntaxKind k) {
    TypedIndex *index = next_array_elem(&f->expressions);
    index->index = i;
    index->kind = k;
    debugf("make_expression_index %i (%i)\n", f->expressions.size, k);

    return f->expressions.size;
}

SyntaxIndex begin_compilation_unit(SyntaxFactory *f, LexResult *lex, const char *file_path) {
    debugf("begin_compilation_unit\n");
    // TODO: Search for referenced comp unit first
    f->current_text = lex->text;
    f->current_unit = next_array_elem(&(f->compilation_units));
    f->current_unit->filepath = copy_string(f, file_path);
    f->current_unit->block = begin_block(f);
    return f->compilation_units.size;
}

void end_compilation_unit(SyntaxFactory *f) {
    // debugf(VERBOSITY_NORMAL, "end_compilation_unit\n");
    end_block(f);
    f->current_unit = 0;
}

SyntaxIndex begin_block(SyntaxFactory *f) {
    AstBlock *block = next_array_elem(&f->blocks);
    block->statements = init_array(sizeof(TypedIndex), 16);
    block->parent_block = f->current_block_index;
    
    f->current_block = block;
    f->current_block_index = f->blocks.size;
    debugf("begin_block %p (%i)\n", (void*)f->current_block, f->current_block_index);
    return f->blocks.size;
}

void end_block(SyntaxFactory *f) {
    debugf("end_block %p (%i)\n", (void*)f->current_block, f->current_block_index);
    if (f->current_block) {
        if (f->current_block->parent_block) {
            f->current_block_index = f->current_block->parent_block;
            f->current_block = ((AstBlock*)f->blocks.array) + f->current_block_index - 1;
        }
        else {
            f->current_block_index = EMPTY_SYNTAX_INDEX;
            f->current_block = 0;
        }
    }
}

SyntaxIndex make_use_statement(SyntaxFactory *f, Token *use_tok, Token *name) {
    debugf("make_use_statement\n");
    assert(f, use_tok, "Use Keyword");
    assert(f, name, "Use Path");
    AstUseStatement *use = next_array_elem(&(f->use_statements));
    use->referenced_compilation_unit = EMPTY_SYNTAX_INDEX;// TODO: Create future comp unit
    return add_statement_index(f, f->use_statements.size, AST_USE);
}

SyntaxIndex make_ident(SyntaxFactory *f, Token *name) {
    assert(f, name, "Ident Name");
    AstIdent *ident = next_array_elem(&f->identifiers);
    ident->name = copy_token_string(f, name);
    debugf("make_ident %i\n", f->identifiers.size);
    return f->identifiers.size;
}

SyntaxIndex make_attr_decl(SyntaxFactory *f, Token *attr, SyntaxIndex ident, SyntaxIndex block) {
    debugf("make_attr_decl %i\n", ident);
    assert(f, attr, "Attr Keyword");
    assert_node(f, ident, "Ident");
    AstAttrDecl *decl = next_array_elem(&f->attribute_declarations);
    decl->ident = ident;
    decl->block_opt = block;
    return add_statement_index(f, f->attribute_declarations.size, AST_ATTR_DECL);
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

    return add_statement_index(f, f->func_declarations.size, AST_FUNC_DECL);
}

SyntaxIndex begin_param_list_decl(SyntaxFactory *f) {
    f->current_param_list_decl = next_array_elem(&f->param_list_decls);
    f->current_param_list_decl->param_start = f->param_decls.size + 1;

    return f->param_list_decls.size;
}

void end_param_list_decl(SyntaxFactory *f) {
    f->current_param_list_decl->param_end_noninclusive = f->param_decls.size + 1;
    f->current_param_list_decl = 0;
}

SyntaxIndex make_param(SyntaxFactory *f, SyntaxIndex ident, Token *colon, SyntaxIndex type) {
    assert_node(f, ident, "Ident");
    assert(f, colon, ":");
    assert_node(f, type, "Type");

    AstParameterDecl *decl = next_array_elem(&f->param_decls);
    decl->ident = ident;
    decl->type = type;

    return f->param_decls.size;
}

SyntaxIndex make_type(SyntaxFactory *f, SyntaxIndex attr_list_opt, SyntaxIndex ident) {
    assert_node(f, ident, "Type Ident");

    AstType *type = next_array_elem(&f->types);
    type->attr_list_opt = attr_list_opt;
    type->ident = ident;

    return f->types.size;
}

SyntaxIndex begin_attr_list(SyntaxFactory *f, Token *left) {
    assert(f, left, "[");
    f->current_attr_list = next_array_elem(&f->attr_lists);
    f->current_attr_list->attributes = init_array(sizeof(SyntaxIndex), 1);
    return f->attr_lists.size;
}

void end_attr_list(SyntaxFactory *f) {
    f->current_attr_list = 0;
}

SyntaxIndex make_unary_expression(SyntaxFactory *f, SyntaxIndex value, SyntaxKind kind) {
    assert_node(f, value, "Unary Expression Value");
    return make_expression_index(f, value, kind);
}

SyntaxIndex make_binary_expression(SyntaxFactory *f, SyntaxIndex left, Token *op, SyntaxIndex right) {
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

    return make_expression_index(f, f->binary_expressions.size, AST_BINARY_EXPRESSION);
}

SyntaxIndex make_return_statement(SyntaxFactory *f, Token *return_token, SyntaxIndex expression) {
    assert(f, return_token, "Return Keyword");
    assert_node(f, expression, "Expression");

    AstReturnStatement *smt = next_array_elem(&f->return_statements);
    smt->expression = expression;
    return add_statement_index(f, f->return_statements.size, AST_RETURN_STATEMENT);
}

SyntaxIndex make_local_decl(SyntaxFactory *f, 
    SyntaxIndex ident, Token *colon, SyntaxIndex type_opt, 
    Token *equals_opt, SyntaxIndex expresion_opt, 
    Token *semicolon) 
{
    assert(f, ident, "Ident");
    assert(f, colon, ":");
    if (!type_opt) {
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

    return add_statement_index(f, f->local_decl_statements.size, AST_LOCAL_DECL);
}

SyntaxIndex make_function_call_statement(SyntaxFactory *f, SyntaxIndex func_call_exp, Token *semicolon) {
    debugf("make_function_call_statement\n");
    assert_node(f, func_call_exp, "Expression");
    assert(f, semicolon, ";");

    AstFunctionCallStatement *call = next_array_elem(&f->function_call_statements);
    call->func_call_exp = func_call_exp;
    return add_statement_index(f, f->function_call_statements.size, AST_FUNC_CALL);
}

SyntaxIndex make_function_call_expression(SyntaxFactory *f, 
    SyntaxIndex expression, Token *left, SyntaxIndex param_list_opt, Token *right) {
    debugf("make_function_call_expression\n");
    assert_node(f, expression, "Expression");
    assert(f, left, "(");
    assert(f, right, ")");
    AstFunctionCallExpression *exp = next_array_elem(&f->function_call_expressions);
    exp->expression_left = expression;
    exp->parameters_opt = param_list_opt;
    SyntaxIndex x = make_expression_index(f, f->function_call_expressions.size, AST_FUNC_CALL);

    return x;
}

SyntaxIndex make_if_statement(SyntaxFactory *f, Token *if_tok, Token *left, SyntaxIndex cond, Token *right, SyntaxIndex stmt) {
    debugf("make_if_statement\n");
    assert(f, if_tok, "If Keyword");
    assert(f, left, "(");
    assert_node(f, cond, "Condition Expression");
    assert(f, right, ")");
    assert_node(f, stmt, "Statement");

    AstIfStatement *is = next_array_elem(&f->if_statements);
    is->condition = cond;
    is->statement = stmt;
    return add_statement_index(f, f->if_statements.size, AST_IF_STATEMENT);
}

SyntaxIndex make_literal_expression(SyntaxFactory *f, Token *token) {
    debugf("make_literal_expression\n");
    assert(f, token, "Literal Value");
    AstLiteralExpression *lit = next_array_elem(&f->literal_expressions);
    lit->string_value = copy_token_string(f, token);
    lit->type = token->type;
    return make_expression_index(f, f->literal_expressions.size, AST_LITERAL);
}

SyntaxIndex make_param_list(SyntaxFactory *f) {
    debugf("make_param_list\n");
    AstParamList *list = next_array_elem(&f->param_lists);
    list->param_expressions = init_array(sizeof(SyntaxIndex), 1);
    return f->param_lists.size;
}

SyntaxIndex add_param(SyntaxFactory *f, SyntaxIndex list, SyntaxIndex exp) {
    debugf("add_param\n");
    if (list) {
        assert_node(f, exp, "Expression");
        AstParamList *l = ((AstParamList *)f->param_lists.array) + list - 1;
        SyntaxIndex *i = next_array_elem(&l->param_expressions);
        *i = exp;
        return l->param_expressions.size;
    }
    return EMPTY_SYNTAX_INDEX;
}

SyntaxIndex block_as_statement(SyntaxFactory *f, SyntaxIndex block) {
    if (block) {
        add_statement_index(f, block, AST_BLOCK);
        return f->statements.size;
    }
    return EMPTY_SYNTAX_INDEX;
}