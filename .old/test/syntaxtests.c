#include "headers/syntaxtests.h"

TestResult success_string_literal() {
    return assert_success("x := \"test\";");
}
TestResult success_string_literal_escapes() {
    return assert_success("x := \"test\\n\";");
}