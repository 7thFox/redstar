#include "headers/parser.h"

typedef enum {
    
    // just_parsed_priority < previously_parse_priority : don't recurse
    // or
    // previously_parse_priority >= just_parsed_priority : recurse
    
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
SyntaxIndex parse_statement(Parser *p);
SyntaxIndex parse_attr_statement(Parser *p);
SyntaxIndex parse_func_decl(Parser *p);
SyntaxIndex parse_parameter_list_decl(Parser *p);
SyntaxIndex parse_ident(Parser *p);
SyntaxIndex parse_type(Parser *p);
SyntaxIndex parse_block(Parser *p);
SyntaxIndex parse_return_statement(Parser *p);
SyntaxIndex parse_expression(Parser *p, ExpressionPriority priority);
SyntaxIndex parse_unary_expression(Parser *p);
SyntaxIndex parse_binary_expression(Parser *p, SyntaxIndex left, ExpressionPriority priority);
SyntaxIndex parse_local_decl(Parser *p);
SyntaxIndex parse_func_call_expression(Parser *p, SyntaxIndex left);
SyntaxIndex parse_func_call_statement(Parser *p);

void parse_lex(Parser *p, LexResult *lex, const char *file_path) {
    // debugf(VERBOSITY_NORMAL, "parse_lex\n");
    p->tokens = lex->tokens;
    p->n_tokens = lex->n_tokens;
    p->current = -1;

    p->factory->tokens = lex->tokens;
    p->factory->current_token = &p->current;

    begin_compilation_unit(p->factory, lex, file_path);
    while (parse_use_statement(p));
    while (parse_statement(p));

    if (p->current < p->n_tokens) {
        fprintf(stderr, "%s:%i:%i: Unrecongnized statement\n",
                p->factory->strings + p->factory->current_unit->filepath,
                p->tokens[p->current + 1].p0.line,
                p->tokens[p->current + 1].p0.col);
    }
    end_compilation_unit(p->factory);
}

// Helper Functions
Token* accept_token(Parser *p, TokenType type) {
    token_index next = p->current + 1;
    debugf(VERBOSITY_NORMAL, "accept_token %i (%c) = %i (%c)?\n", (p->tokens[next].type), (p->tokens[next].type), type, type);
    Token *tok;
    if (next < p->n_tokens &&
        (tok = p->tokens + next)->type == type)
    {
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
    debugf(VERBOSITY_NORMAL, "parse_use_statement\n");
    Token *use;
    if ((use = accept_token(p, TOK_USE))) {
        return make_use_statement(p->factory, use, accept_token(p, TOK_USEPATH));
    }
    return EMPTY_SYNTAX_INDEX;
}

SyntaxIndex parse_statement(Parser *p) {
    debugf(VERBOSITY_NORMAL, "parse_statement\n");
    return parse_attr_statement(p) ||
        parse_func_decl(p) ||
        parse_return_statement(p) ||
        parse_block(p) ||
        parse_local_decl(p) ||
        parse_func_call_statement(p)// do this last because it's pretty awful
        // ... || ...
        ;
}

SyntaxIndex parse_attr_statement(Parser *p) {
    debugf(VERBOSITY_NORMAL, "parse_attr_statement\n");
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
    debugf(VERBOSITY_NORMAL, "parse_func_decl\n");
    Token *func;
    if ((func = accept_token(p, TOK_FUNC))) {
        SyntaxIndex ident = parse_ident(p);
        Token *left = accept_token(p, '(');
        SyntaxIndex param_list = parse_parameter_list_decl(p);
        Token *right = accept_token(p, ')');
        SyntaxIndex return_type = parse_type(p);
        SyntaxIndex block = parse_block(p);

        debugf(VERBOSITY_NORMAL, "parse_func_decl (END)\n");
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
    debugf(VERBOSITY_NORMAL, "parse_parameter_list_decl\n");
    if (!peek_token(p, ')')) {
        SyntaxIndex list = begin_param_list(p->factory);

        do {
            SyntaxIndex ident = parse_ident(p);
            Token *colon = accept_token(p, ':');
            SyntaxIndex type = parse_type(p);
            make_param(p->factory, ident, colon, type);
        } while (accept_token(p, ','));

        debugf(VERBOSITY_NORMAL, "parse_parameter_list_decl (END)\n");
        end_param_list(p->factory);
        return list;
    }
    return EMPTY_SYNTAX_INDEX;
}

SyntaxIndex parse_type(Parser *p) {
    // debugf(VERBOSITY_NORMAL, "parse_type\n");
    SyntaxIndex attr_list = EMPTY_SYNTAX_INDEX;
    Token *left;
    if ((left = accept_token(p, '['))) {
        attr_list = begin_attr_list(p->factory, left);
        
        do { parse_ident(p); } while (accept_token(p, ','));

        accept_token(p, ']');
        end_attr_list(p->factory);
    }

    SyntaxIndex ident = parse_ident(p);
    return make_type(p->factory, attr_list, ident);
}

SyntaxIndex parse_block(Parser *p) {
    // debugf(VERBOSITY_NORMAL, "parse_block\n");
    if (accept_token(p, '{')) {
        SyntaxIndex block = begin_block(p->factory);
        
        while (parse_statement(p));

        end_block(p->factory);

        if (!accept_token(p, '}')) {
            fprintf(stderr, "Expected }\n");
        }

        return block;
    }
    return EMPTY_SYNTAX_INDEX;
}

SyntaxIndex parse_return_statement(Parser *p) {
    debugf(VERBOSITY_NORMAL, "parse_return_statement\n");
    Token *tok_return;
    if ((tok_return = accept_token(p, TOK_RETURN))) {
        SyntaxIndex exp = parse_expression(p, EXP_ANY);
        SyntaxIndex smt = make_return_statement(p->factory, tok_return, exp);

        if (!accept_token(p, ';')) {
            fprintf(stderr, "Expected ;\n");
        }

        return smt;
    }
    return EMPTY_SYNTAX_INDEX;
}

SyntaxIndex parse_expression(Parser *p, ExpressionPriority priority) {
    debugf(VERBOSITY_NORMAL, "parse_expression\n");
    SyntaxIndex left = parse_unary_expression(p);
    
    SyntaxIndex func;
    while ((func = parse_func_call_expression(p, left)) != left) {
        left = func;
    }

    SyntaxIndex bin;
    while ((bin = parse_binary_expression(p, left, priority)))
    {
        left = bin;
    }

    return left;
}

SyntaxIndex parse_binary_expression(Parser *p, SyntaxIndex left, ExpressionPriority priority) {
    debugf(VERBOSITY_NORMAL, "parse_binary_expression\n");
    if (p->current + 1 >= p->n_tokens) {
        return EMPTY_SYNTAX_INDEX;
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
    // case '%': new_priority = EXP_ADD; break;
    // case TOK_OR: new_priority = EXP_ADD; break;
    // case TOK_AND: new_priority = EXP_ADD; break;
    // case '|': new_priority = EXP_ADD; break;
    // case '&': new_priority = EXP_ADD; break;
    default:
        return EMPTY_SYNTAX_INDEX;
    }
#pragma GCC diagnostic pop

    op = accept_token(p, op->type);

    SyntaxIndex right;
    if (new_priority < priority) {
        right = parse_unary_expression(p);
    }
    else {
        right = parse_expression(p, new_priority);
    }

    SyntaxIndex func;
    while ((func = parse_func_call_expression(p, right)) != right) {
        right = func;
    }

    return make_binary_expression(p->factory, left, op, right);
}

SyntaxIndex parse_unary_expression(Parser *p) {
    debugf(VERBOSITY_NORMAL, "parse_unary_expression\n");
    SyntaxIndex exp;
    SyntaxIndex exp_index;
    if ((exp = parse_ident(p))) {
        exp_index = make_unary_expression(p->factory, exp, AST_IDENT);
    }
    // else if ...
    else {
        return EMPTY_SYNTAX_INDEX;
    }
    
    return exp_index;
}

SyntaxIndex parse_local_decl(Parser *p) {
    debugf(VERBOSITY_NORMAL, "parse_local_decl\n");
    if (peek_token(p, TOK_IDENT) && npeek_token(p, ':', 2)) {
        SyntaxIndex ident = parse_ident(p);
        Token *colon = accept_token(p, ':');
        SyntaxIndex type = EMPTY_SYNTAX_INDEX;
        Token *equals = 0;
        SyntaxIndex exp = EMPTY_SYNTAX_INDEX;
        Token *semicolon = 0;
        if ((equals = accept_token(p, '='))) {// Implicit type with assignment
            debugf(VERBOSITY_NORMAL, "imp assign\n");
            exp = parse_expression(p, EXP_ANY);
        }
        else {
            debugf(VERBOSITY_NORMAL, "imp extype\n");
            type = parse_type(p);
            if ((equals = accept_token(p, '='))) {// Explicit type with assignment
                debugf(VERBOSITY_NORMAL, "with assign\n");
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

SyntaxIndex parse_func_call_expression(Parser *p, SyntaxIndex left_exp) {
    debugf(VERBOSITY_NORMAL, "parse_func_call_expression\n");
    Token *left;
    if ((left = accept_token(p, '('))) {
        SyntaxIndex param_list = 0;// TODO
        Token *right = accept_token(p, ')');
        SyntaxIndex x = make_function_call_expression(p->factory, left_exp, left, param_list, right);
        return x;
    }
    return left_exp;
}

SyntaxIndex parse_func_call_statement(Parser *p) {
    debugf(VERBOSITY_NORMAL, "parse_func_call_statement\n");
    Token *prev = 0;
    for (token_index i = p->current + 1; i < p->n_tokens; i++) {
        Token *tok = p->tokens + i;
        if (tok->type == ';' && prev && prev->type == ')') {
            // TODO: say a short prayer

            // ... also could probably do some setup to undo the
            // damage we're about to do, which, if done, we can skip
            // this lookahead to ;

            bool parsed = false;
            SyntaxIndex exp = parse_unary_expression(p);
            SyntaxIndex func;
            while ((func = parse_func_call_expression(p, exp)) != exp) {
                exp = func;
                parsed = true;
            }

            if (!parsed) {
                fprintf(stderr, "Failed to parse func call\n");
                // you may now cry
            }

            Token *semi = accept_token(p, ';');

            return make_function_call_statement(p->factory, exp, semi);
        }
    }
    return EMPTY_SYNTAX_INDEX;
}
