
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
} program_args;

bool parseArgs(int argc, char ** argv);
program_args get_args();
#endif