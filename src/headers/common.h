
#ifndef COMMON_H
#define COMMON_H

#include "stdbool.h"
#include "string.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"

void error_common();

typedef struct {
    char *dir_path;
    bool is_debug;
    bool allerr;

    bool printlex;
    bool printtree;
    bool printsymbols;
    bool printstrings;
} program_args;

bool parseArgs(int argc, char ** argv);
void set_args_tests(program_args args);
program_args get_args();
#endif