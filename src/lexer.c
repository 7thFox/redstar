#include "headers/lexer.h"

typedef struct {
    char c;

    size_t text_size;
    char *text;

    token_index token_count;
    token_index buffer_max;
    Token *token_buffer;

    Cursor current;
} Lexer;

// character classes
static inline bool is_whitespace(char c);
static inline bool is_inline_whitespace(char c);
static inline bool is_ident_start_char(char c);
static inline bool is_ident_char(char c);
static inline bool is_alpha(char c);
static inline bool is_numeric(char c);
static inline bool is_valid_use_path_char(char c);

// lexer functions
static inline char peek(Lexer *l);
bool read(Lexer *l);
void start_token(Lexer *l);
void emit_token(Lexer *l, TokenType t);

// state helpers
bool try_parse_reserved_word(Lexer *l, TokenType type, const char* lit);
bool try_parse_char_literal(Lexer *l, char lit);
bool eat_inline_whitespace(Lexer *l);
bool eat_whitespace(Lexer *l);
bool try_eat_comment(Lexer *l);
void error(Lexer *l, const char * msg);
static inline bool required(Lexer *l, bool ret_val, const char * desc);

// states
bool parse_use(Lexer *l);
bool parse_statement(Lexer *l);
bool parse_ident(Lexer *l);
bool parse_attr_def(Lexer *l);
bool parse_func_def(Lexer *l);
bool parse_param_def(Lexer *l);
void parse_type_name(Lexer *l);
void parse_attr(Lexer *l);
bool parse_return(Lexer *l);
bool parse_expression(Lexer *l);
bool parse_left_unary_operation(Lexer *l);
bool parse_right_unary_operation(Lexer *l);
bool parse_binary_operation(Lexer *l);
bool parse_ident_leading_statement(Lexer *l);
bool parse_if_statement(Lexer *l);
bool parse_numeric_literal(Lexer *l);

LexResult lex_file(FILE *f) {
    Lexer l;

    fseek(f, 0, SEEK_END);
    l.text_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    l.text = malloc(l.text_size * sizeof(char));

    l.buffer_max = 2048;
    l.token_buffer = malloc(l.buffer_max * sizeof(Token));
    l.token_count = 0;

    l.current.ind = -1;
    l.current.line = 1;
    l.current.col = 0;

    fread(l.text, sizeof(char), l.text_size, f);

    eat_whitespace(&l);
    while (parse_use(&l));// move to statemetn?
    while (l.current.ind < l.text_size - 1) {// TODO: might be off by 1 or 2?
        if (!parse_statement(&l)) {
            error(&l, "Unexpected EOF");
            break;
        }
    };

    LexResult r;
    r.tokens = l.token_buffer;
    r.n_tokens = l.token_count;
    r.text = l.text;

    return r;
}

static inline bool is_inline_whitespace(char c) { return c == ' ' || c == '\t'; }
static inline bool is_whitespace(char c) { return is_inline_whitespace(c) || c == '\r' || c == '\n'; }
static inline bool is_alpha(char c) { return c >= 65 && c <= 90 || c >= 97 && c <= 122; }
static inline bool is_numeric(char c) { return c >= 48 && c <= 57; }
static inline bool is_valid_use_path_char(char c) { return is_alpha(c) || is_numeric(c) || c == '-' || c == '_' || c == '/'; }
static inline bool is_ident_start_char(char c) { return is_alpha(c) || c == '_'; }
static inline bool is_ident_char(char c) { return is_ident_start_char(c) || is_numeric(c); }

bool read(Lexer *l) {
    l->current.ind++;
    if (l->current.ind == l->text_size) return false;

    l->c = l->text[l->current.ind];
    //debugf(VERBOSITY_HIGH, "Read: %c\n", l->c);

    if (l->c == '\n') {
        l->current.col = 1;
        l->current.line++;
    }
    else {
        l->current.col++;
    }

    return true;
}

static inline char peek(Lexer *l) {
    char p = l->current.ind + 1 < l->text_size
        ? l->text[l->current.ind + 1]
        : -1;

    //debugf(VERBOSITY_HIGH, "Peek: %c\n", p);
    return p;
}

void start_token(Lexer *l) {
    // TODO: check buffer size and resize if needed
    l->token_buffer[l->token_count].p0 = l->current;
}

void emit_token(Lexer *l, TokenType type) {
    l->token_buffer[l->token_count].type = type;
    l->token_buffer[l->token_count].p1 = l->current;

    // TODO?
    // switch (type)
    // {
    // case TOK_LITERAL:
    //     l->token_buffer[l->token_count].literal_text = l->current;
    //     break;
    // }
    l->token_count++;
}

// helpers:

bool eat_inline_whitespace(Lexer *l) {
    debugf(VERBOSITY_NORMAL, "eat_inline_whitespace\n");
    bool ws = false;
    while (is_inline_whitespace(peek(l)))
    {
        ws = true;
        read(l);
    }

    return ws;
}

bool eat_whitespace(Lexer *l) {
    debugf(VERBOSITY_NORMAL, "eat_whitespace \n");
    bool ws = false;
    while (1)
    {
        char p = peek(l);
        if (is_whitespace(p)) {
            ws = true;
            read(l);
        }
        else if (!try_eat_comment(l)) break;// comments don't count
    }

    return ws;
}

bool try_eat_comment(Lexer *l) {
    if (l->text_size <= l->current.ind + 2) return false;

    if (l->text[l->current.ind +1] == '/'){
        if (l->text[l->current.ind + 2] == '/') {
            read(l);
            read(l);
            do
            {
                read(l);
            } while (l->c != '\n');
            return true;
        }
        // TODO: support /**/ comments
    }
    return false;
}

bool try_parse_reserved_word(Lexer *l, TokenType type, const char* lit) {
    debugf(VERBOSITY_NORMAL, "try_parse_reserved_word '%s'\n", lit);
    int i = 0;
    for (; lit[i] != '\0'; i++)
    {
        //debugf(VERBOSITY_HIGH, "%c == %c ?\n", l->text[l->current.ind + i + 1], lit[i]);
        if (l->current.ind + i >= l->text_size ||
            l->text[l->current.ind + i + 1] != lit[i])
        {
            return false;
        }
    }

    int charAfterLitInd = l->current.ind + i + 1;
    if (l->text_size <= charAfterLitInd || is_whitespace(l->text[charAfterLitInd])) {
        start_token(l);
        l->current.col += i;// no newlines in literals
        l->current.ind += i;
        emit_token(l, type);
        eat_whitespace(l);
        return true;
    }
    return false;
}

bool try_parse_char_literal(Lexer *l, char lit) {
    debugf(VERBOSITY_NORMAL, "try_parse_char_literal '%c'\n", lit);
    if (peek(l) == lit) {
        start_token(l);
        read(l);
        emit_token(l, lit);
        eat_whitespace(l);
        return true;
    }
    return false;
}

void error(Lexer *l, const char * msg) {
    start_token(l);
    emit_token(l, LEX_ERROR);
    fprintf(stderr, "ERROR on %i,%i: %s\n", l->current.line, l->current.col, msg);
}

static inline bool required(Lexer *l, bool ret_val, const char * desc) {
    if (!ret_val) {
        char msg[256] = "Expected ";
        error(l, strcat(msg, desc));
    }
    return ret_val;
}

//////////////////////////////////
//            STATES            //
//////////////////////////////////

bool parse_use(Lexer *l) {
    debugf(VERBOSITY_NORMAL, "parse_use \n");
    if (try_parse_reserved_word(l, TOK_USE, "use"))
    {
        start_token(l);
        while (is_valid_use_path_char(peek(l))) read(l);
        emit_token(l, TOK_USEPATH);
        eat_whitespace(l);
        return true;
    }

    return false;
}

bool parse_statement(Lexer *l) {
    debugf(VERBOSITY_NORMAL, "parse_statement \n");

    return 
        parse_attr_def(l) ||
        parse_func_def(l) ||
        parse_return(l) ||
        parse_if_statement(l) ||
        parse_ident_leading_statement(l);
}

bool parse_attr_def(Lexer *l) {
    if (try_parse_reserved_word(l, TOK_ATTR, "attr")) {
        parse_ident(l);
        if (try_parse_char_literal(l, '{')) {
            // TODO
            required(l, try_parse_char_literal(l, '}'), "}");
        }
        return true;
    }
    return false;
}

bool parse_ident(Lexer *l) {
    debugf(VERBOSITY_NORMAL, "parse_ident \n");
    if (is_ident_start_char(peek(l))) {
        start_token(l);
        read(l);
        while (is_ident_char(peek(l))) read(l);
        emit_token(l, TOK_IDENT);
        eat_whitespace(l);
        return true;
    }

    return false;
}

bool parse_func_def(Lexer *l) {
    debugf(VERBOSITY_NORMAL, "parse_func_def \n");
    if (try_parse_reserved_word(l, TOK_FUNC, "func"))
    {
        parse_ident(l);
        required(l, try_parse_char_literal(l, '('), "(");
        if (parse_param_def(l)){
            while (try_parse_char_literal(l, ',') && required(l, parse_param_def(l), "parameter definition"));
        }
        required(l, try_parse_char_literal(l, ')'), ")");
        parse_type_name(l);// return type
        required(l, try_parse_char_literal(l, '{'), "{");
        while (parse_statement(l));
        required(l, try_parse_char_literal(l, '}'), "}");
        return true;
    }
    return false;
}

bool parse_param_def(Lexer *l) {
    debugf(VERBOSITY_NORMAL, "parse_param_def \n");
    if (parse_ident(l)) {
        try_parse_char_literal(l, ':');
        parse_type_name(l);
        return true;
    }
    return false;
}

void parse_type_name(Lexer *l) {
    debugf(VERBOSITY_NORMAL, "parse_type_name \n");
    parse_attr(l);
    parse_ident(l);
    // TODO: pointers?
    // TODO: arrays?
}

void parse_attr(Lexer *l) {
    debugf(VERBOSITY_NORMAL, "parse_attr \n");
    if (try_parse_char_literal(l, '[')) {
        while (parse_ident(l));
        required(l, try_parse_char_literal(l, ']'), "] or ident");
    }
}

bool parse_return(Lexer *l) {
    debugf(VERBOSITY_NORMAL, "parse_return \n");
    if (try_parse_reserved_word(l, TOK_RETURN, "return")) {
        required(l, parse_expression(l), "expression");
        required(l, try_parse_char_literal(l, ';'), ";");
        return true;
    }
    return false;
}

bool parse_expression(Lexer *l) {
    debugf(VERBOSITY_NORMAL, "parse_expression \n");

    if (try_parse_char_literal(l, '(')) {
        parse_expression(l);
        required(l, try_parse_char_literal(l, ')'), ")");
        return true;
    }

    while (parse_left_unary_operation(l));

    if (parse_numeric_literal(l) || 
        parse_ident(l)) {
        while (parse_right_unary_operation(l));

        if (parse_binary_operation(l)) {
            parse_expression(l);
        }
        return true;
    }

    return false;
}

bool parse_left_unary_operation(Lexer *l) {
    return
        try_parse_reserved_word(l, TOK_INC, "++") ||
        try_parse_reserved_word(l, TOK_DEC, "--");
}

bool parse_right_unary_operation(Lexer *l) {
    if (try_parse_char_literal(l, '(')) {
        if (parse_expression(l)) {
            while (try_parse_char_literal(l, ',') && required(l, parse_expression(l), "expression"));
        }
        required(l, try_parse_char_literal(l, ')'), ")");
        return true;
    }
    return
        try_parse_reserved_word(l, TOK_INC, "++") ||
        try_parse_reserved_word(l, TOK_DEC, "--");
}

bool parse_binary_operation(Lexer *l) {
    if (try_parse_char_literal(l, '+') ||
        try_parse_char_literal(l, '-') ||
        try_parse_char_literal(l, '*') ||
        try_parse_char_literal(l, '/') ||
        try_parse_char_literal(l, '%') ||
        try_parse_reserved_word(l, TOK_AND, "&&") ||
        try_parse_reserved_word(l, TOK_OR, "||") ||
        try_parse_char_literal(l, '&') ||
        try_parse_char_literal(l, '|') ||
        try_parse_reserved_word(l, TOK_EQUALITY, "==") ||
        try_parse_reserved_word(l, TOK_LESS_EQUALS, "<=") ||
        try_parse_reserved_word(l, TOK_GREATER_EQUALS, ">=") ||
        try_parse_reserved_word(l, TOK_NOT_EQUALS, "!=") ||
        try_parse_char_literal(l, '<') ||
        try_parse_char_literal(l, '>')) 
    {
        return true;
    }
    return false;
}

bool parse_ident_leading_statement(Lexer *l) {
    if (parse_ident(l)) {
        if (try_parse_char_literal(l, ':')) {
            parse_type_name(l);
            if (try_parse_char_literal(l, '=')) {
                parse_expression(l);
            }
        }
        else if(try_parse_char_literal(l, '(')) {
            if (parse_expression(l)) {
                while (try_parse_char_literal(l, ',') && required(l, parse_expression(l), "expression"));
            }
            required(l, try_parse_char_literal(l, ')'), ")");
        }
        else {
            error(l, "Lone ident is not a valid statement");
        }
        required(l, try_parse_char_literal(l, ';'), ";");
        return true;
    }
    return false;
}

bool parse_if_statement(Lexer *l) {
    if (try_parse_reserved_word(l, TOK_IF, "if")) {
        required(l, try_parse_char_literal(l, '('), "(");
        required(l, parse_expression(l), "expression");
        required(l, try_parse_char_literal(l, ')'), ")");
        if (try_parse_char_literal(l, '{')) {
            while (parse_statement(l));
            required(l, try_parse_char_literal(l, '}'), "}");
        }
        else {
            required(l, parse_statement(l), "statement");
        }
        return true;
    }
    return false;
}

bool parse_numeric_literal(Lexer *l) {
    bool negative = false;
    char p = peek(l);
    if (p == '-' && l->current.ind + 2 < l->text_size) {
        negative = true;
        p = l->text[l->current.ind + 2];
    }

    if (is_numeric(peek(l))) {
        start_token(l);
        if (negative) read(l);
        while (is_numeric(peek(l))) read(l);
        if (peek(l) == '.') {
            read(l);
            while (is_numeric(peek(l))) read(l);
        }
        if (peek(l) == 'e') {
            read(l);
            if (peek(l) == '-') read(l);
            if (!is_numeric(peek(l))) error(l, "Expected numeric value after exponant");
            while (is_numeric(peek(l))) read(l);
        }
        emit_token(l, TOK_NUMERIC_LITERAL);
        eat_whitespace(l);
        return true;
    }
}


