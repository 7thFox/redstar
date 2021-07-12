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
// SyntaxIndex parse_ident_expression(Parser *p);

void parse_lex(Parser *p, LexResult *lex, const char *file_path) {
    // debugf(VERBOSITY_NORMAL, "parse_lex\n");
    p->tokens = lex->tokens;
    p->n_tokens = lex->n_tokens;
    p->current = -1;

    begin_compilation_unit(p->factory, lex, file_path);
    while (parse_use_statement(p));
    while (parse_statement(p));
    // TODO
    end_compilation_unit(p->factory);
}

// Helper Functions
Token* accept_token(Parser *p, TokenType type) {
    token_index next = p->current + 1;
    // debugf(VERBOSITY_NORMAL, "accept_token %i = %i (%c)?\n", (p->tokens[next].type), type, type);
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
        parse_block(p)
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
        // TODO:
        accept_token(p, TOK_RETURN);
        accept_token(p, TOK_IDENT);
        accept_token(p, '/');
        accept_token(p, TOK_IDENT);
        accept_token(p, ';');

        // while (!accept_token(p, '}')) {
        //     parse_statement(p);
        // }
        end_block(p->factory);
        accept_token(p, '}');
        return block;
    }
    return EMPTY_SYNTAX_INDEX;
}

SyntaxIndex parse_return_statement(Parser *p) {
    Token *tok_return;
    if ((tok_return = accept_token(p, TOK_RETURN))) {


    }
    return EMPTY_SYNTAX_INDEX;
}

SyntaxIndex parse_expression(Parser *p, ExpressionPriority priority) {
    SyntaxIndex left = parse_unary_expression(p);
    SyntaxIndex bin;

    while ((bin = parse_binary_expression(p, left, priority)))
    {
        left = bin;
    }

    return left;
}

SyntaxIndex parse_binary_expression(Parser *p, SyntaxIndex left, ExpressionPriority priority) {
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

    return make_binary_expression(p->factory, left, op, right);
}

SyntaxIndex parse_unary_expression(Parser *p) {
    SyntaxIndex exp;
    if ((exp = parse_ident(p))) {
        return make_unary_expression(p->factory, exp, AST_IDENT);
    }
    return EMPTY_SYNTAX_INDEX;
}

