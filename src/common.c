#include "headers/common.h"

#include <execinfo.h>

void error_common() {
    program_args args = get_args();

    if (args.is_debug)
    {
#define BT_BUFFER_SIZE 255
        void *bt[BT_BUFFER_SIZE];
        int nbt = backtrace(bt, BT_BUFFER_SIZE);
        char **symbols = backtrace_symbols(bt, BT_BUFFER_SIZE);
        printf("Stack Trace:\n");
        // Skipping first frame (error_common) + 1 (calling error function)
        for (int i = 2; i < nbt; i++)
        {
            printf("\t%s\n", symbols[i]);
        }
        free(symbols);
    }

    if (!args.allerr) {
        exit(EXIT_FAILURE);
    }
}

program_args _args;
program_args get_args() { return _args; }

bool parseArgs(int argc, char ** argv){
    memset(&_args, 0, sizeof(program_args));
    bool dirSet = false;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-debug") == 0) {
            _args.is_debug = true;
        }
        else if (strcmp(argv[i], "-printlex") == 0) {
            _args.printlex = true;
        }
        else if (strcmp(argv[i], "-printtree") == 0) {
            _args.printtree = true;
        }
        else if (strcmp(argv[i], "-allerr") == 0) {
            _args.allerr = true;
        }
        else {
            if (i != argc - 1) return false;

            dirSet = true;
            _args.dir_path = argv[i];
        }
    }

    if (!dirSet) {
        return false;
    }
    return true;
}
