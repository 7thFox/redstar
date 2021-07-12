#include "headers/parser.h"

Parser *make_parser() {
    debugf(VERBOSITY_NORMAL, "make_parser\n");
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

// Parse Functions
SyntaxIndex parse_use_statement(Parser *p);
SyntaxIndex parse_statement(Parser *p);
SyntaxIndex parse_attr_statement(Parser *p);

void parse_lex(Parser *p, LexResult *lex, const char *file_path) {
    debugf(VERBOSITY_NORMAL, "parse_lex\n");
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
    debugf(VERBOSITY_NORMAL, "accept_token %i = %i (%c)?\n", (p->tokens[next].type), type, type);
    Token *tok;
    if (next < p->n_tokens &&
        (tok = p->tokens + next)->type == type)
    {
        p->current = next;
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
        // ... || ...
        EMPTY_SYNTAX_INDEX;
}

SyntaxIndex parse_attr_statement(Parser *p) {
    debugf(VERBOSITY_NORMAL, "parse_attr_statement\n");
    Token *attr;
    if ((attr = accept_token(p, TOK_ATTR))) {
        return make_attr_decl(p->factory,
            attr,
            make_ident(p->factory, accept_token(p, TOK_IDENT)),
            EMPTY_SYNTAX_INDEX);// TODO
    }
    return EMPTY_SYNTAX_INDEX;
}
