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
SyntaxIndex parse_if_statement(Parser *p);
SyntaxIndex parse_literal_expression(Parser *p);

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
    debugf("accept_token %i (%c) = %i (%c)?\n", (p->tokens[next].type), (p->tokens[next].type), type, type);
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
    debugf("parse_use_statement\n");
    Token *use;
    if ((use = accept_token(p, TOK_USE))) {
        return make_use_statement(p->factory, use, accept_token(p, TOK_USEPATH));
    }
    return EMPTY_SYNTAX_INDEX;
}

SyntaxIndex parse_statement(Parser *p) {
    debugf("parse_statement\n");
    return 
        // Leads with keyword
        parse_attr_statement(p) ||
        parse_func_decl(p) ||
        parse_return_statement(p) ||
        block_as_statement(p->factory, parse_block(p)) ||
        parse_if_statement(p) ||
        // Others
        parse_local_decl(p) ||
        parse_func_call_statement(p)// do this last because it's pretty awful
        ;
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
        SyntaxIndex list = begin_param_list_decl(p->factory);

        do {
            SyntaxIndex ident = parse_ident(p);
            Token *colon = accept_token(p, ':');
            SyntaxIndex type = parse_type(p);
            make_param(p->factory, ident, colon, type);
        } while (accept_token(p, ','));

        debugf("parse_parameter_list_decl (END)\n");
        end_param_list_decl(p->factory);
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
    debugf("parse_block\n");
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
    debugf("parse_return_statement\n");
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
    debugf("parse_expression\n");
    SyntaxIndex left = parse_unary_expression(p);

    SyntaxIndex bin;
    while ((bin = parse_binary_expression(p, left, priority)))
    {
        left = bin;
    }

    return left;
}

SyntaxIndex parse_binary_expression(Parser *p, SyntaxIndex left, ExpressionPriority priority) {
    debugf("parse_binary_expression\n");
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

    case TOK_NOT_EQUALS: new_priority = EXP_EQUALS; break;
    case TOK_EQUALITY:   new_priority = EXP_NOT_EQUALS; break;
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

    return make_binary_expression(p->factory, left, op, right);
}

SyntaxIndex parse_unary_expression(Parser *p) {
    debugf("parse_unary_expression\n");
    bool parse_func = false;
    SyntaxIndex exp;
    SyntaxIndex exp_index;
    if ((exp = parse_ident(p))) {
        parse_func = true;
        exp_index = make_unary_expression(p->factory, exp, AST_IDENT);
    }
    else if ((exp = parse_literal_expression(p))) {
        exp_index = exp;
    }
    else {
        return EMPTY_SYNTAX_INDEX;
    }
    
    if (parse_func) {
        SyntaxIndex func;
        while ((func = parse_func_call_expression(p, exp_index)) != exp_index) {
            exp_index = func;
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
        SyntaxIndex exp = EMPTY_SYNTAX_INDEX;
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

SyntaxIndex parse_func_call_expression(Parser *p, SyntaxIndex left_exp) {
    debugf("parse_func_call_expression\n");
    Token *left;
    if ((left = accept_token(p, '('))) {
        SyntaxIndex param_list = 0;
        Token *right;
        if (!(right = accept_token(p, ')'))) {
            param_list = make_param_list(p->factory);
                        
            do {
                add_param(p->factory, param_list,
                    parse_expression(p, EXP_ANY));
            } while (accept_token(p, ','));
        }
        SyntaxIndex x = make_function_call_expression(p->factory, left_exp, left, param_list, right);
        return x;
    }
    return left_exp;
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

            SyntaxIndex exp = parse_unary_expression(p);
            bool parsed = ((TypedIndex*)p->factory->expressions.array)[exp-1].kind == AST_FUNC_CALL;
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

SyntaxIndex parse_if_statement(Parser *p) {
    debugf("parse_if_statement\n");
    Token *if_tok;
    if ((if_tok = accept_token(p, TOK_IF))) {
        Token *left = accept_token(p, '(');
        SyntaxIndex cond = parse_expression(p, EXP_ANY);
        Token *right = accept_token(p, ')');
        SyntaxIndex smt = parse_statement(p);
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

