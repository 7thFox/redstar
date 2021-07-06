#ifndef AST_H
#define AST_H

#include "lexer.h"
#include "common.h"
#include "stdio.h"
#include <stdlib.h>

typedef uint16_t ast_index;
typedef uint8_t compilation_unit_index;

#define AST_INDEX_NONE (ast_index)-1

// These are each 1 under the real max so -1 can work as a sentinal value
#define AST_ARRAY_SIZE_MAX 0xFFFF
#define COMP_UNIT_SIZE_MAX 0xFF
#define AST_HASH_TABLE_SIZE 0x100

typedef struct {
    uint8_t     has_value;// memset to 0
    ast_index   index;
    void        *next_conflict; // ast_hashtable_value
} ast_hashtable_value;

typedef struct
{
    token_index use_keyword;
    token_index use_path;
} AST_USE_STATEMENT;

typedef struct {
    char *filename;
    ast_index *use_statement_indexes;
    ast_index use_statement_size;
    ast_index use_statement_capacity;
} AST_COMPLIATION_UNIT;

typedef struct {
    AST_COMPLIATION_UNIT    *compilation_units;
    compilation_unit_index  comp_unit_size;
    compilation_unit_index  comp_unit_capacity;

    AST_USE_STATEMENT   *use_statements;
    ast_index           use_statement_size;
    ast_index           use_statement_capacity;
    ast_hashtable_value use_statement_hash_table[AST_HASH_TABLE_SIZE];
} AST_TREE;

AST_TREE create_tree();
compilation_unit_index build_compliation_unit(AST_TREE *tree, LexResult *lex, const char *filePath);

void print_ast(AST_TREE *tree);

void dealloc_ast(AST_TREE *tree);

#endif