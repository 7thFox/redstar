#ifndef TEST_COMMON_H
#define TEST_COMMON_H

#include "../../src/headers/common.h"
#include "../../src/headers/lexer.h"
#include "../../src/headers/parser.h"
#include "../../src/headers/symboltable.h"

typedef enum {
    STATUS_SUCCESS,
    STATUS_FAILED,
    STATUS_INCONCLUSIVE
} TestStatus;

typedef struct {
    TestStatus status;
    char *message;
    bool free_message;
} TestResult;

typedef enum {
    COMP_LEX_ERROR,
    COMP_PARSE_ERROR,
    COMP_SYMBOL_ERROR,
    COMP_SUCCESS,
} CompileResult;

CompileResult run_test(const char *test_case);
TestResult assert_compilation(CompileResult expected, CompileResult actual);
TestResult assert_success(const char *test_case);

#endif