#include "headers/testcommon.h"

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

TestResult assert_success(const char *test_case) {
    return assert_compilation(COMP_SUCCESS, run_test(test_case));
}