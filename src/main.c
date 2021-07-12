#include "headers/common.h"
#include "headers/lexer.h"
#include "headers/parser.h"
#include "headers/printast.h"

#include "dirent.h"
#include "stdio.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *dir_path;
    bool is_debug;
    bool printlex;
} program_args;

int parse_file(const char *filePath, Parser *parser);
int parseArgs(int argc, char ** argv);

program_args args;

int main(int argc, char ** argv) {
    if (parseArgs(argc, argv) != 0) {
        printf("failed to parse args\n");
        return 1;
    }

    DIR *dr;
    debugf(VERBOSITY_NORMAL, "root dir: %s\n", args.dir_path);

    if (!(dr = opendir(args.dir_path)))
    {
        printf("Could not open directory '%s'\n", args.dir_path);
        return 1;
    }

    Parser *parser = make_parser();

    char buff[512];
    struct dirent *de;
    while ((de = readdir(dr))) {
        if (de->d_type == DT_REG) {

            sprintf(buff, "%s/%s", args.dir_path, de->d_name);
            parse_file(buff, parser);
        }
        else if (de->d_type == DT_DIR) {
            // TODO: recursive
        }
    }
    closedir(dr);

    print_ast(parser);

    destroy_parser(parser);
}

int parseArgs(int argc, char ** argv){
    bool dirSet = false;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-debug") == 0) {
            debugf(VERBOSITY_HIGH, "DEBUG\n");
            args.is_debug = 1;
        }
        else if (strcmp(argv[i], "-printlex") == 0) {
            args.printlex = 1;
        }
        else {
            if (i != argc - 1) return 1;
            dirSet = true;
            args.dir_path = argv[i];
        }
    }

    if (!dirSet) {
        return 1;
    }
    return 0;
}

int parse_file(const char* filePath, Parser *parser) {
    printf("parsing file '%s'...\n", filePath);

    FILE *f = fopen(filePath, "r");
    if (!f) {
        printf("Failed to open file '%s'\n", filePath);
        return 1;
    }

    LexResult r = lex_file(f);
    fclose(f);

    if (args.printlex) {
        printf("Lexed Tokens:\n");
        for (int i = 0; i < r.n_tokens; i++) {
            if (r.tokens[i].type == LEX_ERROR)
                printf("ERR ");
            else if (r.tokens[i].type <= 255)
                printf("%c ", r.tokens[i].type);
            else
                printf("%i ", r.tokens[i].type);
        }
        printf("\n");
    }

    parse_lex(parser, &r, filePath);

    free(r.tokens);
    free(r.text);
    return 0;
}
