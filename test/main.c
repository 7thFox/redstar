
#include "stdbool.h"
#include "execinfo.h"
#include "stdio.h"

#include "../src/headers/common.h"
#include "../src/headers/lexer.h"
#include "../src/headers/parser.h"
#include "../src/headers/symboltable.h"

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

TestResult error_redefined_same_scope();
TestResult error_redefined_inner_scope();
TestResult error_redefined_outer_scope();

int main() {
    TestResult (*tests[])() = {
        &error_redefined_same_scope,
        &error_redefined_inner_scope,
        &error_redefined_outer_scope};
    int ntests = sizeof(tests) / sizeof(TestResult (*)());

    char **test_names = backtrace_symbols((void**)tests, ntests);

    program_args args;
    memset(&args, 0, sizeof(program_args));
    args.dir_path = "TEST";
    args.allerr = true;   // Don't exit
    args.is_debug = false;// Don't print stacktrace
    set_args_tests(args);

    for (int i = 0; i < ntests; i++) {
        printf("[%i/%i] Running Test %s...", i, ntests, test_names[i]);
        TestResult result = tests[i]();

        printf("\rCompleted Test %s: ", test_names[i]);
        switch (result.status)
        {
            case STATUS_SUCCESS:
                printf("Success");
                break;
            case STATUS_FAILED:
                printf("Failed");
                break;
            case STATUS_INCONCLUSIVE:
                printf("Inconclusive");
                break;
        }
        if (result.message) {
            printf("\n\t%s", result.message);
            if (result.free_message) free(result.message);
        }
        printf("\n");
    }
    printf("\n");
}

typedef enum {
    COMP_LEX_ERROR,
    COMP_PARSE_ERROR,
    COMP_SYMBOL_ERROR,
    COMP_SUCCESS,
} CompileResult;

CompileResult run_test(const char *test_case) {
    CompileResult result = COMP_SUCCESS;
    Parser *p = 0;
    SymbolTable *st = 0;

    FILE *f = tmpfile();
    fputs(test_case, f);
    rewind(f);
    LexResult r = lex_file(f, "");
    if (r.has_error) {
        result = COMP_LEX_ERROR;
        goto end;
    }

    p = make_parser();
    parse_lex(p, &r, "");
    if (p->has_error) {
        result = COMP_PARSE_ERROR;
        goto end;
    }

    st = create_symbol_table(p);
    if (st->has_error) {
        result = COMP_SYMBOL_ERROR;
        goto end;
    }

end:
    if (st) destroy_symbol_table(st);
    if (p) destroy_parser(p);
    free(r.text);
    free(r.tokens);
    fclose(f);
    return result;
}

TestResult assert_compilation(CompileResult expected, CompileResult actual) {
    if (actual == expected) {
        return (TestResult){STATUS_SUCCESS, 0};
    }

    const char *expected_desc;
    switch (expected) {
        case COMP_LEX_ERROR:    expected_desc = "Lex Error"; break;
        case COMP_PARSE_ERROR:  expected_desc = "Parse Error"; break;
        case COMP_SYMBOL_ERROR: expected_desc = "Symbol Error"; break;
        case COMP_SUCCESS:      expected_desc = "Success"; break;
    }
    const char *actual_desc;
    switch (actual) {
        case COMP_LEX_ERROR:    actual_desc = "Lex Error"; break;
        case COMP_PARSE_ERROR:  actual_desc = "Parse Error"; break;
        case COMP_SYMBOL_ERROR: actual_desc = "Symbol Error"; break;
        case COMP_SUCCESS:      actual_desc = "Success"; break;
    }

    char *buff = malloc(sizeof(char) * 128);
    snprintf(buff, 128, "Expected %s but got %s", expected_desc, actual_desc);
    return (TestResult){STATUS_FAILED, buff, true};
}

TestResult error_redefined_same_scope() {
    const char *test =
        "x := 1;\n"
        "x := 1;";
    return assert_compilation(COMP_SYMBOL_ERROR, run_test(test));
}

TestResult error_redefined_inner_scope() {
    const char *test =
        "x := 1;\n"
        "func bar() {\n"
        "   x := 1;\n"
        "}";
    return assert_compilation(COMP_SYMBOL_ERROR, run_test(test));
}

TestResult error_redefined_outer_scope() {
    const char *test =
        "func bar() {\n"
        "   x := 1;\n"
        "}\n"
        "x := 1;";
    return assert_compilation(COMP_SYMBOL_ERROR, run_test(test));
}
