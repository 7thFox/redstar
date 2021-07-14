#include "headers/parser.h"
#define debugf(...) ;
// #define debugf(...) printf(__VA_ARGS__)

typedef enum {
    
    // just_parsed_priority < previously_parse_priority : don't recurse
    // or
    // previously_parse_priority >= just_parsed_priority : recurse
    
    EXP_EQUALS = 500,
    EXP_NOT_EQUALS = 500,

    EXP_DIVIDE = 500,
    EXP_MULTIPLY = 500,

    EXP_ADD = 400,
    EXP_SUBTRACT = 400,

    EXP_ANY = -9999,

} ExpressionPriority;

Parser *make_parser() {
    // debugf(VERBOSITY_NORMAL, "make_parser\n");
    Parser *p = malloc(sizeof(Parser));
    p->factory = make_astfactory();
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

// Parse Functions
SyntaxIndex parse_use_statement(Parser *p);
SyntaxIndex parse_block(Parser *p);
StatementIndex parse_statement(Parser *p);
SyntaxIndex parse_attr_statement(Parser *p);
SyntaxIndex parse_func_decl(Parser *p);
SyntaxIndex parse_parameter_list_decl(Parser *p);
SyntaxIndex parse_ident(Parser *p);
SyntaxIndex parse_type(Parser *p);
SyntaxIndex parse_block(Parser *p);
SyntaxIndex parse_return_statement(Parser *p);
ExpressionIndex parse_expression(Parser *p, ExpressionPriority priority);
ExpressionIndex parse_unary_expression(Parser *p);
ExpressionIndex parse_binary_expression(Parser *p, ExpressionIndex left, ExpressionPriority priority);
SyntaxIndex parse_local_decl(Parser *p);
SyntaxIndex parse_func_call_expression(Parser *p, ExpressionIndex left);
SyntaxIndex parse_func_call_statement(Parser *p);
SyntaxIndex parse_if_statement(Parser *p);
SyntaxIndex parse_literal_expression(Parser *p);

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
        fprintf(stderr, "%s:%i:%i: Unrecongnized statement\n",
                p->factory->strings + p->factory->current_filepath.i,
                p->tokens[p->current + 1].p0.line,
                p->tokens[p->current + 1].p0.col);
    }
}

// Helper Functions
Token* accept_token(Parser *p, TokenType type) {
    token_index next = p->current + 1;
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
            EMPTY_SYNTAX_INDEX);// TODO
    }
    return EMPTY_SYNTAX_INDEX;
}

SyntaxIndex parse_func_decl(Parser *p) {
    debugf("parse_func_decl\n");
    Token *func;
    if ((func = accept_token(p, TOK_FUNC))) {
        SyntaxIndex ident = parse_ident(p);
        Token *left = accept_token(p, '(');
        SyntaxIndex param_list = parse_parameter_list_decl(p);
        Token *right = accept_token(p, ')');
        SyntaxIndex return_type = parse_type(p);
        SyntaxIndex block = parse_block(p);

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
    // debugf(VERBOSITY_NORMAL, "parse_type\n");
    SyntaxIndex attr_list = EMPTY_SYNTAX_INDEX;
    if ((accept_token(p, '['))) {
        attr_list = make_attr_list(p->factory);
        
        do { 
            SyntaxIndex attr = parse_ident(p);
            add_attr(p->factory, attr_list, attr);
        } while (accept_token(p, ','));

        accept_token(p, ']');
    }

    SyntaxIndex ident = parse_ident(p);
    return make_type(p->factory, attr_list, ident);
}

SyntaxIndex parse_block(Parser *p) {
    debugf("parse_block\n");
    if (accept_token(p, '{')) {
        SyntaxIndex block = make_block(p->factory);
        
        StatementIndex s;
        while ((s = parse_statement(p)).i) {
            add_statement(p->factory, block, s);
        }
        
        if (!accept_token(p, '}')) {
            fprintf(stderr, "Expected }\n");
        }

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
            fprintf(stderr, "Expected ;\n");
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

    case TOK_NOT_EQUALS: new_priority = EXP_EQUALS; break;
    case TOK_EQUALITY:   new_priority = EXP_NOT_EQUALS; break;
    // case '%': new_priority = EXP_ADD; break;
    // case TOK_OR: new_priority = EXP_ADD; break;
    // case TOK_AND: new_priority = EXP_ADD; break;
    // case '|': new_priority = EXP_ADD; break;
    // case '&': new_priority = EXP_ADD; break;
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
                fprintf(stderr, "Expected ) or ,\n");
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
                fprintf(stderr, "Failed to parse func call\n");
                // you may now cry
            }

            if (!accept_token(p, ';')) {
                fprintf(stderr, "Expected ;\n");
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
        return make_if_statement(p->factory,
            if_tok, left, cond, right, smt);
    }
    return EMPTY_SYNTAX_INDEX;
}

SyntaxIndex parse_literal_expression(Parser *p) {
    debugf("parse_literal_expression\n");
    Token *tok;
    if ((tok = accept_token(p, TOK_NUMERIC_LITERAL))) {
        return make_literal_expression(p->factory, tok);
    }
    return EMPTY_SYNTAX_INDEX;
}

