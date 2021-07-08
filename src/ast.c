#include "headers/ast.h"

typedef struct {
    token_index pos;
    Token       *tokens;
    token_index cTokens;

    char *text;
    // convience
    AST_TREE *tree;
    AST_COMPLIATION_UNIT *comp_unit;
} lex_cursor;

// helpers
void free_hashtable_conflicts(ast_hashtable_value *table);
Token *get_token(lex_cursor *cursor, TokenType type);
Token *require_token(lex_cursor *cursor, TokenType type);
bool slice_compare(const char *text, Token *t1, Token *t2);

// node builders
ast_index build_find_use_statement(lex_cursor* lex);

// print
#define SPACE_PER_LEVEL 2
#define INDENT_CONST "                                            "
void print_comp_unit(AST_TREE *tree, AST_COMPLIATION_UNIT *unit, int indentLevel);
void print_use_statement(AST_USE_STATEMENT *use, int indentLevel);
void print_statement(AST_COMPLIATION_UNIT *unit, AST_STATEMENT *statement, int indentLevel);
void print_attr_declr(AST_COMPLIATION_UNIT *unit, AST_ATTR_DECLARATION *statement, int indentLevel);

void dealloc_ast(AST_TREE *tree) {
    debugf(VERBOSITY_NORMAL, "dealloc_ast\n");
    for (int i = 0; i < tree->comp_unit_size; i++) {
        free(tree->compilation_units[i].use_statement_indexes);
    }
    free(tree->compilation_units);
    free(tree->use_statements);
    free_hashtable_conflicts(tree->use_statement_hash_table);
}

void free_hashtable_conflicts(ast_hashtable_value *table) {
    debugf(VERBOSITY_NORMAL, "free_hashtable_conflicts\n");
    for (int i = 0; i < AST_HASH_TABLE_SIZE; i++) {
        ast_hashtable_value *hash_entry = table + i;
        if (hash_entry != 0) {
            hash_entry = hash_entry->next_conflict;
        }
        while (hash_entry != 0) {
            ast_hashtable_value *to_free = hash_entry;
            hash_entry = hash_entry->next_conflict;
            free(to_free);
        };
    }
}

Token *get_token(lex_cursor *cursor, TokenType type) {
    debugf(VERBOSITY_NORMAL, "get_token %i (%c)\n", type, type);
    Token *token = cursor->tokens + cursor->pos;
    if (cursor->pos < cursor->cTokens &&
        token->type == type) 
    {
        cursor->pos++;
        return token;
    }
    return 0;
}

Token *require_token(lex_cursor *cursor, TokenType type) {
    debugf(VERBOSITY_NORMAL, "require_token %i (%c)\n", type, type);
    Token *token = cursor->tokens + cursor->pos;
    if (cursor->pos < cursor->cTokens &&
        token->type == type) 
    {
        cursor->pos++;
        return token;
    }

    Token *last_token = cursor->tokens + 
        (cursor->pos < cursor->cTokens ? cursor->pos : cursor->pos - 1);
    fprintf(stderr, "Failed to get token of type '%i' (%c) in %s %i,%i\n",
            type,
            type,
            cursor->comp_unit->filename,
            last_token->p0.line,
            last_token->p0.col);

    return 0;
}

bool slice_compare(const char *text, Token *t1, Token *t2) {
    int diff = t1->p1.ind - t1->p0.ind;
    if (diff != (t2->p1.ind - t2->p0.ind))
    {
        return false;
    }

    for (int i = 0; i < diff; i++) {
        if (text[t1->p0.ind + i + 1] != text[t2->p0.ind + i + 1]) {
            return false;
        }
    }

    return true;
}

AST_TREE create_tree() {
    AST_TREE tree;
    tree.comp_unit_capacity = 8;
    tree.comp_unit_size = 0;
    tree.compilation_units = malloc(sizeof(AST_COMPLIATION_UNIT) * tree.comp_unit_capacity);
    tree.use_statement_capacity = 8;
    tree.use_statement_size = 0;
       printf("alloc\n");
    tree.use_statements = malloc(sizeof(AST_USE_STATEMENT) * tree.use_statement_capacity);
    printf("malloc %x\n", tree.use_statements);
    memset(tree.use_statement_hash_table, 0, sizeof(ast_hashtable_value) * AST_HASH_TABLE_SIZE);
    return tree;
}

compilation_unit_index build_compliation_unit(AST_TREE *tree, LexResult *lex, const char *filePath) {
    debugf(VERBOSITY_NORMAL, "build_compliation_unit\n");
    if (tree->comp_unit_size == tree->comp_unit_capacity) {
        if (tree->comp_unit_capacity >= COMP_UNIT_SIZE_MAX) {
            fprintf(stderr, "Error building AST for '%s': Max number of compliation units.\n", filePath);
            return -1;
        }
        tree->comp_unit_capacity *= 2;
        if (tree->comp_unit_capacity > COMP_UNIT_SIZE_MAX) tree->comp_unit_capacity = COMP_UNIT_SIZE_MAX;

        tree->compilation_units = realloc(tree->compilation_units, sizeof(AST_COMPLIATION_UNIT) * tree->comp_unit_capacity);
    }

    lex_cursor cursor;
    cursor.pos = 0;
    cursor.tokens = lex->tokens;
    cursor.cTokens = lex->n_tokens;
    cursor.text = lex->text;
    cursor.tree = tree;
    cursor.comp_unit = tree->compilation_units + tree->comp_unit_size;

    cursor.comp_unit->filename = malloc(sizeof(char) * 255);
    cursor.comp_unit->filename = strcpy(cursor.comp_unit->filename, filePath);
    cursor.comp_unit->use_statement_capacity = 5;
    cursor.comp_unit->use_statement_size = 0;
    cursor.comp_unit->use_statement_indexes = malloc(sizeof(ast_index) * cursor.comp_unit->use_statement_capacity);

    ast_index use_index;
    while (1) {
        if ((use_index = build_find_use_statement(&cursor)) == AST_INDEX_NONE) {
            break;
        }

        // TODO: realloc indexes if needed

        cursor.comp_unit->use_statement_indexes[cursor.comp_unit->use_statement_size++] = use_index;
    }

    return tree->comp_unit_size++;
}

ast_index build_find_use_statement(lex_cursor* lex) {
    debugf(VERBOSITY_NORMAL, "build_find_use_statement\n");
    Token *use_keyword;
    Token *use_path;
    if ((use_keyword = get_token(lex, TOK_USE)) == 0 ||
        (use_path = require_token(lex, TOK_USEPATH)) == 0)// TODO
    {
        return -1;
    }

    if (lex->tree->use_statement_size == lex->tree->use_statement_capacity) {
        if (lex->tree->use_statement_size == AST_ARRAY_SIZE_MAX) {
            fprintf(stderr, "Reached max global use statements.\n");
            return -1;
        }
        lex->tree->use_statement_capacity *= 2;
        if (lex->tree->use_statement_capacity > AST_ARRAY_SIZE_MAX) lex->tree->use_statement_capacity = AST_ARRAY_SIZE_MAX;

        lex->tree->use_statements = realloc(lex->tree->use_statements, sizeof(AST_USE_STATEMENT) * lex->tree->use_statement_capacity);
    }

    char hash = 0;
    for (int i = use_path->p0.ind; i < use_path->p1.ind; i++) {
        hash ^= lex->text[i + 1];
    }

    ast_hashtable_value *entry = lex->tree->use_statement_hash_table + hash;

    if (entry->has_value == 0) {
        entry->has_value = 1;
        entry->index = lex->tree->use_statement_size;
    }
    else {
        ast_hashtable_value *prev_entry = 0;
        do
        {
            Token *entryPath = lex->tokens + lex->tree->use_statements[entry->index].use_path;
            if (slice_compare(lex->text, use_path, entryPath))
            {
                return entry->index;
            }
            prev_entry = entry;
            entry = entry->next_conflict;
        } while (entry != 0);

        entry = malloc(sizeof(ast_hashtable_value));
        prev_entry->next_conflict = entry;
    }

    AST_USE_STATEMENT *use = lex->tree->use_statements + entry->index;
    use->use_keyword = lex->pos - 2;
    use->use_path = lex->pos - 1;
    return lex->tree->use_statement_size++;
}

void print_ast(AST_TREE *tree) {
    for (int i = 0; i < tree->comp_unit_size; i++) {
        print_comp_unit(tree, tree->compilation_units + i, 0);
    }
}

void print_comp_unit(AST_TREE *tree, AST_COMPLIATION_UNIT *unit, int indentLevel) {
    printf("%.*s", indentLevel * SPACE_PER_LEVEL, INDENT_CONST);
    printf("AST_COMPLIATION_UNIT '%s':\n", unit->filename);
    for (int i = 0; i < unit->use_statement_size; i++) {
        print_use_statement(tree->use_statements + unit->use_statement_indexes[i], indentLevel + 1);
    }
    // for (int i = 0; i < unit->statements_size; i++) {
    //     print_statement(unit->statements + i, indentLevel + 1);
    // }
}

void print_use_statement(AST_USE_STATEMENT *use, int indentLevel) {
    printf("%.*s", indentLevel * SPACE_PER_LEVEL, INDENT_CONST);
    printf("AST_USE_STATEMENT\n");
}



// void print_statement(AST_COMPLIATION_UNIT *unit, AST_STATEMENT *statement, int indentLevel) {
//     switch (statement->type)
//     {
//         case AttrDeclr:
//             print_attr_declr(unit, unit->attr + statement->index, indentLevel);
//             break;
//         }
// }
// void print_attr_declr(AST_COMPLIATION_UNIT *unit, AST_ATTR_DECLARATION *statement, int indentLevel) {
//     printf("%.*s", indentLevel * SPACE_PER_LEVEL, INDENT_CONST);
//     printf("AST_ATTR_DECLARATION\n");
// }