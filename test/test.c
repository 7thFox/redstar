
#include "stdbool.h"
#include "execinfo.h"
#include "stdio.h"

#include "headers/testcommon.h"
#include "headers/syntaxtests.h"
#include "headers/typetests.h"

int main() {
    TestResult (*tests[])() = {
        &success_string_literal,
        &success_string_literal_escapes,

        &scope_success_redefined_complatable_scopes,
        &scope_error_redefined_same_scope,
        &scope_error_redefined_inner_scope,
        &scope_error_redefined_outer_scope,
    };
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
