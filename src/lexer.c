#include "headers/lexer.h"
#define TRACE_LEX 0
#define debugf(...) ;
// #define debugf(...) printf(__VA_ARGS__)

typedef struct {
    char c;

    size_t text_size;
    char *text;

    token_index token_count;
    token_index buffer_max;
    Token *token_buffer;

    Cursor current;

    bool       has_error;
    const char *filepath;
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
bool try_lex_reserved_word(Lexer *l, TokenType type, const char* lit);
bool try_lex_char_literal(Lexer *l, char lit);
bool eat_inline_whitespace(Lexer *l);
bool eat_whitespace(Lexer *l);
bool try_eat_comment(Lexer *l);
void lex_error(Lexer *l, const char * msg);
static inline bool required(Lexer *l, bool ret_val, const char * desc);

// states
bool lex_use(Lexer *l);
bool lex_statement(Lexer *l);
bool lex_ident(Lexer *l);
bool lex_attr_def(Lexer *l);
bool lex_func_def(Lexer *l);
bool lex_param_def(Lexer *l);
void lex_type_name(Lexer *l);
void lex_attr(Lexer *l);
bool lex_return(Lexer *l);
bool lex_expression(Lexer *l);
bool lex_left_unary_operation(Lexer *l);
bool lex_right_unary_operation(Lexer *l);
bool lex_binary_operation(Lexer *l);
bool lex_ident_leading_statement(Lexer *l);
bool lex_if_statement(Lexer *l);
bool lex_numeric_literal(Lexer *l);
bool lex_annotate_statement(Lexer *l);

LexResult lex_file(FILE *f, const char *filepath)
{
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
    l.has_error = false;
    l.filepath = filepath;

    fread(l.text, sizeof(char), l.text_size, f);

    eat_whitespace(&l);
    while (lex_use(&l));// move to statemetn?
    while (l.current.ind < l.text_size - 1) {// TODO: might be off by 1 or 2?
        if (!lex_statement(&l)) {
            lex_error(&l, "Unexpected EOF");
            break;
        }
    };

    LexResult r;
    r.tokens = l.token_buffer;
    r.n_tokens = l.token_count;
    r.text = l.text;
    r.has_error = l.has_error;

    return r;
}

static inline bool is_inline_whitespace(char c) { return c == ' ' || c == '\t'; }
static inline bool is_whitespace(char c) { return is_inline_whitespace(c) || c == '\r' || c == '\n'; }
static inline bool is_alpha(char c) { return (c >= 65 && c <= 90) || (c >= 97 && c <= 122); }
static inline bool is_numeric(char c) { return c >= 48 && c <= 57; }
static inline bool is_valid_use_path_char(char c) { return is_alpha(c) || is_numeric(c) || c == '-' || c == '_' || c == '/'; }
static inline bool is_ident_start_char(char c) { return is_alpha(c) || c == '_'; }
static inline bool is_ident_char(char c) { return is_ident_start_char(c) || is_numeric(c); }

bool read(Lexer *l) {
    l->current.ind++;
    if (l->current.ind == l->text_size) return false;

    l->c = l->text[l->current.ind];
#if TRACE_LEX
    debugf("Read: %c\n", l->c);
#endif

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
    uint16_t i = l->current.ind + 1;
    char p = i < l->text_size
        ? l->text[i]
        : -1;

    // debugf(VERBOSITY_HIGH, "Peek: %c (%i)\n", p, p);
    return p;
}

void start_token(Lexer *l) {
    // TODO: check buffer size and resize if needed
    l->token_buffer[l->token_count].p0.line = l->current.line;
    l->token_buffer[l->token_count].p0.col = l->current.col;//  +1;
    l->token_buffer[l->token_count].p0.ind = l->current.ind + 1;
}

void emit_token(Lexer *l, TokenType type) {
    l->token_buffer[l->token_count].type = type;
    l->token_buffer[l->token_count].p1.line = l->current.line;
    l->token_buffer[l->token_count].p1.col = l->current.col;//  +1;
    l->token_buffer[l->token_count].p1.ind = l->current.ind + 1;

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
    debugf("eat_inline_whitespace\n");
    bool ws = false;
    while (is_inline_whitespace(peek(l)))
    {
        ws = true;
        read(l);
    }

    return ws;
}

bool eat_whitespace(Lexer *l) {
    debugf("eat_whitespace \n");
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
    debugf("try_eat_comment\n");
    if (l->text_size <= (uint16_t)(l->current.ind + 2))
        return false;

    if (l->text[(uint16_t)(l->current.ind +1)] == '/'){
        if (l->text[(uint16_t)(l->current.ind + 2)] == '/') {
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

bool try_lex_reserved_word(Lexer *l, TokenType type, const char* lit) {
    debugf("try_lex_reserved_word '%s'\n", lit);
    int i = 0;
    for (; lit[i] != '\0'; i++)
    {
        // debugf(VERBOSITY_HIGH, "%c == %c ?\n", l->text[l->current.ind + i + 1], lit[i]);
        if ((uint16_t)(l->current.ind + i) >= l->text_size ||
            l->text[(uint16_t)(l->current.ind + i + 1)] != lit[i])
        {
            return false;
        }
    }

    uint16_t charAfterLitInd = l->current.ind + i + 1;
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

bool try_lex_char_literal(Lexer *l, char lit) {
    debugf("try_lex_char_literal '%c'\n", lit);
    if (peek(l) == lit) {
        start_token(l);
        read(l);
        emit_token(l, lit);
        eat_whitespace(l);
        return true;
    }
    return false;
}

void lex_error(Lexer *l, const char * msg) {
    start_token(l);
    emit_token(l, LEX_ERROR);
    fprintf(stderr, "LEX_ERROR in %s:%i:%i: %s\n", l->filepath, l->current.line, l->current.col, msg);
    l->has_error = true;
}

static inline bool required(Lexer *l, bool ret_val, const char * desc) {
    if (!ret_val) {
        char msg[256] = "Expected ";
        lex_error(l, strcat(msg, desc));
    }
    return ret_val;
}

//////////////////////////////////
//            STATES            //
//////////////////////////////////

bool lex_use(Lexer *l) {
    debugf("lex_use \n");
    if (try_lex_reserved_word(l, TOK_USE, "use"))
    {
        start_token(l);
        while (is_valid_use_path_char(peek(l))) read(l);
        emit_token(l, TOK_USEPATH);
        eat_whitespace(l);
        return true;
    }

    return false;
}

bool lex_statement(Lexer *l) {
    debugf("lex_statement \n");

    return 
        lex_attr_def(l) ||
        lex_func_def(l) ||
        lex_return(l) ||
        lex_if_statement(l) ||
        lex_annotate_statement(l) ||
        lex_ident_leading_statement(l);
}

bool lex_attr_def(Lexer *l) {
    debugf("lex_attr_def\n");
    if (try_lex_reserved_word(l, TOK_ATTR, "attr")) {
        lex_ident(l);
        if (try_lex_char_literal(l, '{')) {
            // TODO
            required(l, try_lex_char_literal(l, '}'), "}");
        }
        return true;
    }
    return false;
}

bool lex_ident(Lexer *l) {
    debugf("lex_ident \n");
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

bool lex_func_def(Lexer *l) {
    debugf("lex_func_def \n");
    if (try_lex_reserved_word(l, TOK_FUNC, "func"))
    {
        lex_ident(l);
        required(l, try_lex_char_literal(l, '('), "(");
        if (lex_param_def(l)){
            while (try_lex_char_literal(l, ',') && required(l, lex_param_def(l), "parameter definition"));
        }
        required(l, try_lex_char_literal(l, ')'), ")");
        lex_type_name(l);// return type
        required(l, try_lex_char_literal(l, '{'), "{");
        while (lex_statement(l));
        required(l, try_lex_char_literal(l, '}'), "}");
        return true;
    }
    return false;
}

bool lex_param_def(Lexer *l) {
    debugf("lex_param_def \n");
    if (lex_ident(l)) {
        try_lex_char_literal(l, ':');
        lex_type_name(l);
        return true;
    }
    return false;
}

void lex_type_name(Lexer *l) {
    debugf("lex_type_name \n");
    lex_attr(l);
    lex_ident(l);
    // TODO: pointers?
    // TODO: arrays?
}

void lex_attr(Lexer *l) {
    debugf("lex_attr \n");
    if (try_lex_char_literal(l, '[')) {
        while (lex_ident(l));
        required(l, try_lex_char_literal(l, ']'), "] or ident");
    }
}

bool lex_return(Lexer *l) {
    debugf("lex_return \n");
    if (try_lex_reserved_word(l, TOK_RETURN, "return")) {
        required(l, lex_expression(l), "expression");
        required(l, try_lex_char_literal(l, ';'), ";");
        return true;
    }
    return false;
}

bool lex_expression(Lexer *l) {
    debugf("lex_expression \n");

    if (try_lex_char_literal(l, '(')) {
        lex_expression(l);
        required(l, try_lex_char_literal(l, ')'), ")");
        return true;
    }

    while (lex_left_unary_operation(l));

    if (lex_numeric_literal(l) || 
        lex_ident(l)) {
        while (lex_right_unary_operation(l));

        if (lex_binary_operation(l)) {
            lex_expression(l);
        }
        return true;
    }

    return false;
}

bool lex_left_unary_operation(Lexer *l) {
    debugf("lex_left_unary_operation\n");
    return
        try_lex_reserved_word(l, TOK_INC, "++") ||
        try_lex_reserved_word(l, TOK_DEC, "--");
}

bool lex_right_unary_operation(Lexer *l) {
    debugf("lex_right_unary_operation\n");
    if (try_lex_char_literal(l, '(')) {
        if (lex_expression(l)) {
            while (try_lex_char_literal(l, ',') && required(l, lex_expression(l), "expression"));
        }
        required(l, try_lex_char_literal(l, ')'), ")");
        return true;
    }
    return
        try_lex_reserved_word(l, TOK_INC, "++") ||
        try_lex_reserved_word(l, TOK_DEC, "--");
}

bool lex_binary_operation(Lexer *l) {
    debugf("lex_binary_operation\n");
    if (try_lex_char_literal(l, '+') ||
        try_lex_char_literal(l, '-') ||
        try_lex_char_literal(l, '*') ||
        try_lex_char_literal(l, '/') ||
        try_lex_char_literal(l, '%') ||
        try_lex_reserved_word(l, TOK_AND, "&&") ||
        try_lex_reserved_word(l, TOK_OR, "||") ||
        try_lex_char_literal(l, '&') ||
        try_lex_char_literal(l, '|') ||
        try_lex_reserved_word(l, TOK_EQUALITY, "==") ||
        try_lex_reserved_word(l, TOK_LESS_EQUALS, "<=") ||
        try_lex_reserved_word(l, TOK_GREATER_EQUALS, ">=") ||
        try_lex_reserved_word(l, TOK_NOT_EQUALS, "!=") ||
        try_lex_char_literal(l, '<') ||
        try_lex_char_literal(l, '>')) 
    {
        return true;
    }
    return false;
}

bool lex_ident_leading_statement(Lexer *l) {
    debugf("lex_ident_leading_statement\n");
    if (lex_ident(l)) {
        if (try_lex_char_literal(l, ':')) {
            lex_type_name(l);
            if (try_lex_char_literal(l, '=')) {
                lex_expression(l);
            }
        }
        else if(try_lex_char_literal(l, '(')) {
            if (lex_expression(l)) {
                while (try_lex_char_literal(l, ',') && required(l, lex_expression(l), "expression"));
            }
            required(l, try_lex_char_literal(l, ')'), ")");
        }
        else {
            lex_error(l, "Lone ident is not a valid statement");
        }
        required(l, try_lex_char_literal(l, ';'), ";");
        return true;
    }
    return false;
}

bool lex_if_statement(Lexer *l) {
    debugf("lex_if_statement\n");
    if (try_lex_reserved_word(l, TOK_IF, "if")) {
        required(l, try_lex_char_literal(l, '('), "(");
        required(l, lex_expression(l), "expression");
        required(l, try_lex_char_literal(l, ')'), ")");
        if (try_lex_char_literal(l, '{')) {
            while (lex_statement(l));
            required(l, try_lex_char_literal(l, '}'), "}");
        }
        else {
            required(l, lex_statement(l), "statement");
        }
        return true;
    }
    return false;
}

bool lex_numeric_literal(Lexer *l) {
    debugf("lex_numeric_literal\n");
    bool negative = false;
    char p = peek(l);
    if (p == '-' && (uint16_t)(l->current.ind + 2) < l->text_size) {
        negative = true;
        p = l->text[(uint16_t)(l->current.ind + 2)];
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
            if (!is_numeric(peek(l))) lex_error(l, "Expected numeric value after exponent");
            while (is_numeric(peek(l))) read(l);
        }
        emit_token(l, TOK_NUMERIC_LITERAL);
        eat_whitespace(l);
        return true;
    }
    return false;
}

bool lex_annotate_statement(Lexer *l) {
    debugf("lex_annotate_statement\n");
    if (try_lex_char_literal(l, '[')) {
        required(l, lex_ident(l), "Attr Name Ident");
        while (try_lex_char_literal(l, ',')) {
            required(l, lex_ident(l), "Attr Name Ident");
        }
        required(l, try_lex_char_literal(l, ']'), "]");
        required(l, lex_ident(l), "Variable Name Ident");
        required(l, try_lex_char_literal(l, ';'), ";");
        return true;
    }
    return false;
}
