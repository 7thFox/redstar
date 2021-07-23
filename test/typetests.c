#include "headers/typetests.h"

TestResult scope_success_redefined_complatable_scopes() {
    return assert_success(
        "func foo() {\n"
        "   x := 1;\n"
        "}\n"
        "func bar() {\n"
        "   x := 1;\n"
        "}");
}

TestResult scope_error_redefined_same_scope() {
    const char *test =
        "x := 1;\n"
        "x := 1;";
    return assert_compilation(COMP_SYMBOL_ERROR, run_test(test));
}

TestResult scope_error_redefined_inner_scope() {
    const char *test =
        "x := 1;\n"
        "func bar() {\n"
        "   x := 1;\n"
        "}";
    return assert_compilation(COMP_SYMBOL_ERROR, run_test(test));
}

TestResult scope_error_redefined_outer_scope() {
    const char *test =
        "func bar() {\n"
        "   x := 1;\n"
        "}\n"
        "x := 1;";
    return assert_compilation(COMP_SYMBOL_ERROR, run_test(test));
}
