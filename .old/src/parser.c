#include "headers/parser.h"
#define TRACE_PARSE 0
#define debugf(...) ;
// #define debugf(...) printf("  "); printf(__VA_ARGS__)

typedef enum
{
    // just_parsed_priority < previously_parse_priority : don't recurse
    // or
    // previously_parse_priority >= just_parsed_priority : recurse

    EXP_EQUALS = 900,
    EXP_NOT_EQUALS = 900,

    EXP_MOD = 600,

    EXP_DIVIDE = 500,
    EXP_MULTIPLY = 500,

    EXP_ADD = 400,
    EXP_SUBTRACT = 400,

    EXP_ANY = -9999,
}
ExpressionPriority;

Parser *make_parser() {
    // debugf(VERBOSITY_NORMAL, "make_parser\n");
    Parser *p = malloc(sizeof(Parser));
    p->factory = make_astfactory();
    p->has_error = false;
    return p;
}

void destroy_parser(Parser *p) {
    destroy_astfactory(p->factory);
    free(p);
}

// Helper Functions
Token *accept_token(Parser *p, TokenType type);
Token *peek_token(Parser *p, TokenType type);
Token *npeek_token(Parser *p, TokenType type, token_index n);
void parse_error(Parser *p, const char *msg);

Token *accept_unary_operator(Parser *p);
Token *accept_binary_operator(Parser *p);

// Parse Functions
SyntaxIndex parse_use_statement(Parser *p);
SyntaxIndex parse_block(Parser *p);
StatementIndex parse_statement(Parser *p);
SyntaxIndex parse_attr_statement(Parser *p);
SyntaxIndex parse_func_decl(Parser *p);
SyntaxIndex parse_parameter_list_decl(Parser *p);
SyntaxIndex parse_ident(Parser *p);
SyntaxIndex parse_type(Parser *p);
SyntaxIndex parse_attr_list(Parser *p, bool allow_remove);
SyntaxIndex parse_return_statement(Parser *p);
ExpressionIndex parse_expression(Parser *p, ExpressionPriority priority);
ExpressionIndex parse_unary_expression(Parser *p);
ExpressionIndex parse_binary_expression(Parser *p, ExpressionIndex left, ExpressionPriority priority);
SyntaxIndex parse_local_decl(Parser *p);
SyntaxIndex parse_func_call_expression(Parser *p, ExpressionIndex left);
SyntaxIndex parse_func_call_statement(Parser *p);
SyntaxIndex parse_if_statement(Parser *p);
SyntaxIndex parse_if_statement(Parser *p);
SyntaxIndex parse_literal_expression(Parser *p);
SyntaxIndex parse_annotate_statement(Parser *p);
SyntaxIndex parse_bind_anno_statement(Parser *p);

void parse_lex(Parser *p, LexResult *lex, const char *file_path) {
    // debugf(VERBOSITY_NORMAL, "parse_lex\n");
    p->tokens = lex->tokens;
    p->n_tokens = lex->n_tokens;
    p->current = -1;

    p->factory->tokens = lex->tokens;
    p->factory->current_token = &p->current;

    SyntaxIndex unit_index = make_compilation_unit(p->factory, lex, file_path);
    AstCompilationUnit *unit = get_ast_node(unit_index, p->factory->compilation_units);
    p->factory->current_filepath = unit->filepath;
    SyntaxIndex block = unit->block;

    STScope *scope = next_array_elem(&p->factory->scopes);
    scope->comp_unit = unit_index;
    scope->block = block;
    scope->id_parent = (ScopeId){0};
    p->factory->current_scope = (ScopeId){p->factory->scopes.size};

    SyntaxIndex u;
    while ((u = parse_use_statement(p)).i) {
        add_statement(p->factory, block, 
            make_statement(p->factory, u, AST_USE));
    }

    StatementIndex s;
    while ((s = parse_statement(p)).i) {
        add_statement(p->factory, block, s);
    }

    if (p->current != p->n_tokens-1) {
        parse_error(p, "Unrecognized statement");
    }
}

// Helper Functions
Token* accept_token(Parser *p, TokenType type) {
    token_index next = p->current + 1;
#if TRACE_PARSE
    Cursor c = p->factory->tokens[next].p0;
    printf("    TRACE %s:%i:%i\n",
        get_string(p->factory->string_table, p->factory->current_filepath), c.line, c.col);
#endif
    debugf("accept_token %i (%c) = %i (%c)?\n", (p->tokens[next].type), (p->tokens[next].type), type, type);
    Token *tok;
    if (next < p->n_tokens &&
        (tok = p->tokens + next)->type == type)
    {
        // printf("%c %i\n", type, type);
        p->current = next;
        return tok;
    }
    return 0;
}

Token *peek_token(Parser *p, TokenType type) {
    return npeek_token(p, type, 1);
}

Token *npeek_token(Parser *p, TokenType type, token_index n) {
    token_index next = p->current + n;
    Token *tok;
    if (next < p->n_tokens &&
        (tok = p->tokens + next)->type == type)
    {
        return tok;
    }
    return 0;
}

void parse_error(Parser *p, const char *msg) {
    Cursor c = p->factory->tokens[p->current].p0;
    fprintf(stderr, "PARSE_ERROR in %s:%i:%i: %s\n",
        get_string(p->factory->string_table, p->factory->current_filepath),
        c.line, c.col, msg);
    p->has_error = true;
    error_common();
}

// Parse Functions
SyntaxIndex parse_use_statement(Parser *p) {
    debugf("parse_use_statement\n");
    Token *use;
    if ((use = accept_token(p, TOK_USE))) {
        return make_use_statement(p->factory, use, accept_token(p, TOK_USEPATH));
    }
    return EMPTY_SYNTAX_INDEX;
}

StatementIndex parse_statement(Parser *p) {
    debugf("parse_statement\n");

#define try_parse_stmt(fn, k)                      \
    if ((ind = fn(p)).i)                           \
    {                                              \
        return make_statement(p->factory, ind, k); \
    }

    SyntaxIndex ind;
    // Leads with keyword
    try_parse_stmt(parse_attr_statement, AST_ATTR_DECL);
    try_parse_stmt(parse_func_decl, AST_FUNC_DECL);
    try_parse_stmt(parse_return_statement, AST_RETURN_STATEMENT);
    try_parse_stmt(parse_block, AST_BLOCK);
    try_parse_stmt(parse_if_statement, AST_IF_STATEMENT);
    try_parse_stmt(parse_bind_anno_statement, AST_BIND_ANNO);
    try_parse_stmt(parse_annotate_statement, AST_ANNOTATE_LOCAL);
    // Others
    try_parse_stmt(parse_local_decl, AST_LOCAL_DECL);
    try_parse_stmt(parse_func_call_statement, AST_FUNC_CALL); // do this last because it's pretty awful

    return (StatementIndex){0};
}

SyntaxIndex parse_attr_statement(Parser *p) {
    debugf("parse_attr_statement\n");
    Token *attr;
    if ((attr = accept_token(p, TOK_ATTR))) {
        SyntaxIndex ident = parse_ident(p);
        return make_attr_decl(p->factory,
            attr,
            ident,
            EMPTY_SYNTAX_INDEX);// TODO: attr body?
    }
    return EMPTY_SYNTAX_INDEX;
}

SyntaxIndex parse_func_decl(Parser *p) {
    debugf("parse_func_decl\n");
    Token *func;
    if ((func = accept_token(p, TOK_FUNC))) {
        SyntaxIndex block = make_block(p->factory);// because of param scoping, we custom parse the block

        SyntaxIndex ident = parse_ident(p);
        Token *left = accept_token(p, '(');
        push_scope(p->factory, block);
        SyntaxIndex param_list = parse_parameter_list_decl(p);
        Token *right = accept_token(p, ')');
        SyntaxIndex return_type = EMPTY_SYNTAX_INDEX;
        if (!peek_token(p, '{')) {
            return_type = parse_type(p);
        }

        // PARSE_BLOCK START
        if (!accept_token(p, '{')) {
            parse_error(p, "Expected block");
        }
        StatementIndex s;
        while ((s = parse_statement(p)).i) {
            add_statement(p->factory, block, s);
        }

        if (!accept_token(p, '}')) {
            parse_error(p, "Expected }");
        }
        pop_scope(p->factory);
        // PARSE_BLOCK END

        debugf("parse_func_decl (END)\n");
        return make_func_decl(p->factory, func, ident, left, param_list, right, return_type, block);
    }
    return EMPTY_SYNTAX_INDEX;
}

SyntaxIndex parse_ident(Parser *p) {
    // debugf(VERBOSITY_NORMAL, "parse_ident\n");
    Token *ident;
    if ((ident = accept_token(p, TOK_IDENT))) {
        return make_ident(p->factory, ident);
    }
    return EMPTY_SYNTAX_INDEX;
}

SyntaxIndex parse_parameter_list_decl(Parser *p) {
    debugf("parse_parameter_list_decl\n");
    if (!peek_token(p, ')')) {
        SyntaxIndex list = make_param_list_decl(p->factory);

        do {
            SyntaxIndex ident = parse_ident(p);
            Token *colon = accept_token(p, ':');
            SyntaxIndex type = parse_type(p);
            SyntaxIndex param = make_param_decl(p->factory, ident, colon, type);
            add_param_decl(p->factory, list, param);
        } while (accept_token(p, ','));

        return list;
    }
    return EMPTY_SYNTAX_INDEX;
}

SyntaxIndex parse_type(Parser *p) {
    debugf("parse_type\n");
    SyntaxIndex attr_list = parse_attr_list(p, false);
    SyntaxIndex ident = parse_ident(p);
    return make_type(p->factory, attr_list, ident);
}

SyntaxIndex parse_attr_list(Parser *p, bool allow_remove) {
    debugf("parse_attr_list\n");
    if ((accept_token(p, '['))) {
        SyntaxIndex list = make_attr_list(p->factory);

        do {
            Token *remove = 0;
            if (allow_remove) {
                remove = accept_token(p, '-');
            }
            SyntaxIndex attr = parse_ident(p);
            SyntaxIndex elem = make_attr_list_elem(p->factory, remove, attr);
            add_attr(p->factory, list, elem);
        } while (accept_token(p, ','));

        if (!accept_token(p, ']')) {
            parse_error(p, "Expected ]");
        }
        return list;
    }
    return EMPTY_SYNTAX_INDEX;
}

SyntaxIndex parse_block(Parser *p) {
    debugf("parse_block\n");
    if (accept_token(p, '{')) {
        SyntaxIndex block = make_block(p->factory);
        push_scope(p->factory, block);
        StatementIndex s;
        while ((s = parse_statement(p)).i) {
            add_statement(p->factory, block, s);
        }

        if (!accept_token(p, '}')) {
            parse_error(p, "Expected }");
        }
        pop_scope(p->factory);
        return block;
    }
    return EMPTY_SYNTAX_INDEX;
}

SyntaxIndex parse_return_statement(Parser *p) {
    debugf("parse_return_statement\n");
    Token *tok_return;
    if ((tok_return = accept_token(p, TOK_RETURN))) {
        ExpressionIndex exp = parse_expression(p, EXP_ANY);
        SyntaxIndex smt = make_return_statement(p->factory, tok_return, exp);

        if (!accept_token(p, ';')) {
            parse_error(p, "Expected ;");
        }

        return smt;
    }
    return EMPTY_SYNTAX_INDEX;
}

ExpressionIndex parse_expression(Parser *p, ExpressionPriority priority) {
    debugf("parse_expression\n");
    ExpressionIndex left = parse_unary_expression(p);

    ExpressionIndex bin;
    while ((bin = parse_binary_expression(p, left, priority)).i)
    {
        left = bin;
    }

    return left;
}

ExpressionIndex parse_binary_expression(Parser *p, ExpressionIndex left, ExpressionPriority priority) {
    debugf("parse_binary_expression\n");
    if (p->current + 1 >= p->n_tokens) {
        return (ExpressionIndex){0};
    }
    Token *op = p->tokens + p->current + 1;
    ExpressionPriority new_priority;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch"
    switch (op->type)
    {
    case '+': new_priority = EXP_ADD; break;
    case '-': new_priority = EXP_SUBTRACT; break;
    case '*': new_priority = EXP_MULTIPLY; break;
    case '/': new_priority = EXP_DIVIDE; break;
    case '%': new_priority = EXP_MOD; break;

    case TOK_NOT_EQUALS: new_priority = EXP_EQUALS; break;
    case TOK_EQUALITY:   new_priority = EXP_NOT_EQUALS; break;
    default:
        return (ExpressionIndex){0};
    }
#pragma GCC diagnostic pop

    op = accept_token(p, op->type);

    ExpressionIndex right;
    if (new_priority < priority) {
        right = parse_unary_expression(p);
    }
    else {
        right = parse_expression(p, new_priority);
    }

    return make_expression(
        p->factory, 
        make_binary_expression(p->factory, left, op, right),
        AST_BINARY_EXPRESSION);
}

ExpressionIndex parse_unary_expression(Parser *p) {
    debugf("parse_unary_expression\n");
    bool parse_func = false;
    SyntaxIndex exp;
    ExpressionIndex exp_index;
    if ((exp = parse_ident(p)).i) {
        parse_func = true;
        exp_index = make_expression(p->factory, exp, AST_IDENT);
    }
    else if ((exp = parse_literal_expression(p)).i) {
        exp_index = make_expression(p->factory, exp, AST_LITERAL);
    }
    else {
        return (ExpressionIndex){0};
    }
    
    if (parse_func) {
        SyntaxIndex func;
        while ((func = parse_func_call_expression(p, exp_index)).i) {
            exp_index = make_expression(p->factory, func, AST_FUNC_CALL);
        }
    }
    
    return exp_index;
}

SyntaxIndex parse_local_decl(Parser *p) {
    debugf("parse_local_decl\n");
    if (peek_token(p, TOK_IDENT) && npeek_token(p, ':', 2)) {
        SyntaxIndex ident = parse_ident(p);
        Token *colon = accept_token(p, ':');
        SyntaxIndex type = EMPTY_SYNTAX_INDEX;
        Token *equals = 0;
        ExpressionIndex exp = (ExpressionIndex){0};
        Token *semicolon = 0;
        if ((equals = accept_token(p, '='))) {// Implicit type with assignment
            exp = parse_expression(p, EXP_ANY);
        }
        else {
            type = parse_type(p);
            if ((equals = accept_token(p, '='))) {// Explicit type with assignment
                exp = parse_expression(p, EXP_ANY);
            }
        }

        semicolon = accept_token(p, ';');
        return make_local_decl(p->factory, 
            ident, 
            colon, type,
            equals, exp,
            semicolon);

    }
    return EMPTY_SYNTAX_INDEX;
}

SyntaxIndex parse_func_call_expression(Parser *p, ExpressionIndex left_exp) {
    debugf("parse_func_call_expression\n");
    Token *left;
    if ((left = accept_token(p, '('))) {
        SyntaxIndex param_list = EMPTY_SYNTAX_INDEX;
        Token *right;
        if (!(right = accept_token(p, ')'))) {
            param_list = make_param_list(p->factory);
                        
            do {
                add_param(p->factory, param_list,
                    parse_expression(p, EXP_ANY));
            } while (accept_token(p, ','));

            if (!(right = accept_token(p, ')'))) {
                parse_error(p, "Expected ) or ,");
            }
        }
        return make_function_call(p->factory, left_exp, left, param_list, right);
    }
    return EMPTY_SYNTAX_INDEX;
}

SyntaxIndex parse_func_call_statement(Parser *p) {
    debugf("parse_func_call_statement\n");
    Token *prev = 0;
    for (token_index i = p->current + 1; i < p->n_tokens; i++) {
        Token *tok = p->tokens + i;
        if (tok->type == ';' && prev && prev->type == ')') {
            // TODO: say a short prayer

            // ... also could probably do some setup to undo the
            // damage we're about to do, which, if done, we can skip
            // this lookahead to ;

            ExpressionIndex exp = parse_unary_expression(p);
            _TypedIndex i = *((_TypedIndex *)get_ast_node(exp, p->factory->expressions));
            if (i.kind != AST_FUNC_CALL) {
                parse_error(p, "Failed to parse func call");
                // you may now cry
            }

            if (!accept_token(p, ';')) {
                parse_error(p, "Expected ;");
                return EMPTY_SYNTAX_INDEX;
                // TODO: I think if you did something like
                // get_func()() this will get hit...
            }

            return (SyntaxIndex){i.index.i};
        }
    }
    return EMPTY_SYNTAX_INDEX;
}

SyntaxIndex parse_if_statement(Parser *p) {
    debugf("parse_if_statement\n");
    Token *if_tok;
    if ((if_tok = accept_token(p, TOK_IF))) {
        Token *left = accept_token(p, '(');
        ExpressionIndex cond = parse_expression(p, EXP_ANY);
        Token *right = accept_token(p, ')');
        StatementIndex smt = parse_statement(p);
        Token *else_tok;
        StatementIndex else_stmt = (StatementIndex){0};
        if ((else_tok = accept_token(p, TOK_ELSE))) {
            else_stmt = parse_statement(p);
        }
        return make_if_statement(p->factory,
            if_tok, left, cond, right, smt,
            else_tok, else_stmt);
    }
    return EMPTY_SYNTAX_INDEX;
}

SyntaxIndex parse_literal_expression(Parser *p) {
    debugf("parse_literal_expression\n");
    Token *tok;
    if ((tok = accept_token(p, TOK_NUMERIC_LITERAL)) ||
        (tok = accept_token(p, TOK_STRING_LITERAL))) {
        return make_literal_expression(p->factory, tok);
    }
    return EMPTY_SYNTAX_INDEX;
}

SyntaxIndex parse_annotate_statement(Parser *p) {
    debugf("parse_annotate_statement\n");
    SyntaxIndex list;
    if ((list = parse_attr_list(p, true)).i) {
        SyntaxIndex anno = make_annotate_statement(p->factory, list);
        SyntaxIndex ident = parse_ident(p);
        add_ident_to_annotate(p->factory, anno, ident);
        while (accept_token(p, ',')) {
            ident = parse_ident(p);
            add_ident_to_annotate(p->factory, anno, ident);
        }
        if (!accept_token(p, ';')) {
            parse_error(p, "Expected ;");
        }
        return anno;
    }
    return EMPTY_SYNTAX_INDEX;
}

SyntaxIndex parse_bind_anno_statement(Parser *p) {
    debugf("parse_bind_anno_statement\n");

    BindAnnoStatementKind kind = 0;

    Token *bind_anno = 0;
    if ((bind_anno = accept_token(p, TOK_BIND))) kind |= BAS_BIND;
    else if ((bind_anno = accept_token(p, TOK_ANNOTATE))) kind |= BAS_ANNO;
    else return EMPTY_SYNTAX_INDEX;

    SyntaxIndex func = EMPTY_SYNTAX_INDEX;
    Token *wildcard_target = 0;
    Token *op = 0;
    if ((wildcard_target = accept_token(p, '_'))) kind |= BAS_TARGET_WILD;
    else if ((func = parse_ident(p)).i) kind |= BAS_TARGET_FUNC;
    else if ((op = accept_token(p, '*')) ||
             (op = accept_token(p, '/')) ||
             (op = accept_token(p, '+')) ||
             (op = accept_token(p, '-')) ||
             (op = accept_token(p, TOK_INC)) ||
             (op = accept_token(p, TOK_DEC)) ||
             (op = accept_token(p, TOK_EQUALITY)) ||
             (op = accept_token(p, TOK_GREATER_EQUALS)) ||
             (op = accept_token(p, TOK_NOT_EQUALS)) ||
             (op = accept_token(p, TOK_LESS_EQUALS)) ||
             (op = accept_token(p, '<')) ||
             (op = accept_token(p, '>')))
    {
        kind |= BAS_TARGET_OP;
    }
    else parse_error(p, "Expected func name, op, or _");

    Token *wildcard_kind = 0;
    SyntaxIndex wildcard_list = EMPTY_SYNTAX_INDEX;
    SyntaxIndex ordinal_list = EMPTY_SYNTAX_INDEX;
    if ((wildcard_kind = accept_token(p, TOK_ANY)) ||
        (wildcard_kind = accept_token(p, TOK_ALL)) ||
        (wildcard_kind = accept_token(p, TOK_NONE)))
    {
        kind |= BAS_DEFS_WILD;
        wildcard_list = parse_attr_list(p, true);
    }
    else
    {
        if ((kind & BAS_TARGET_WILD) == BAS_TARGET_WILD) {
            parse_error(p, "Cannot define ordinal attributes for wildcard bind/anno target");
        }
        kind |= BAS_DEFS_ORDINAL;
        ordinal_list = make_bind_anno_ordinals(p->factory);
        SyntaxIndex list;
        do {
            if (accept_token(p, '_')) {
                add_bind_anno_ordinal(p->factory, ordinal_list, EMPTY_SYNTAX_INDEX);
            }
            else if ((list = parse_attr_list(p, true)).i) {
                add_bind_anno_ordinal(p->factory, ordinal_list, list);
            }
        } while (accept_token(p, ','));
    }

    Token *arrow = 0;
    SyntaxIndex arrow_list = EMPTY_SYNTAX_INDEX;
    if ((arrow = accept_token(p, TOK_DOUBLE_ARROW)))
    {
        if ((arrow_list = parse_attr_list(p, false)).i) {
            kind |= BAS_RETURN;
        }
        else if (!accept_token(p, '_')) {
            parse_error(p, "Expected attr list or _");
        }
    }

    return make_bind_anno_statement(p->factory, kind, bind_anno,
        func, op, wildcard_target,
        wildcard_kind, wildcard_list, ordinal_list,
        arrow, arrow_list);
}