#ifndef TYPETESTS_H
#define TYPETESTS_H

#include "testcommon.h"

TestResult scope_success_redefined_complatable_scopes();
TestResult scope_error_redefined_same_scope();
TestResult scope_error_redefined_inner_scope();
TestResult scope_error_redefined_outer_scope();

#endif