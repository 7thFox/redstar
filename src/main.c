#include "headers/common.h"
#include "headers/lexer.h"
#include "headers/parser.h"
#include "headers/printast.h"
#include "headers/symboltable.h"

#include "dirent.h"
#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#define debugf(...) ;
// #define debugf(...) printf(__VA_ARGS__)
void post_parse(Parser *p, program_args args);
int parse_file(program_args args, const char *filePath, Parser *parser);

int main(int argc, char ** argv) {
    if (!parseArgs(argc, argv)) {
        fprintf(stderr, "failed to parse args\n");
        return 1;

    }

    program_args args = get_args();

    DIR *dr;
    debugf("root dir: %s\n", args.dir_path);

    if (!(dr = opendir(args.dir_path)))
    {
        fprintf(stderr, "Could not open directory '%s'\n", args.dir_path);
        return 1;
    }

    Parser *parser = make_parser();

    char buff[512];
    struct dirent *de;
    while ((de = readdir(dr))) {
        if (de->d_type == DT_REG) {

            sprintf(buff, "%s/%s", args.dir_path, de->d_name);
            if (parse_file(args, buff, parser)) break;
        }
        else if (de->d_type == DT_DIR) {
            // TODO: recursive
        }
    }
    closedir(dr);

    if (!parser->has_error) {
        post_parse(parser, args);
    }
    destroy_parser(parser);
}

int parse_file(program_args args, const char* filePath, Parser *parser) {
    debugf("parsing file '%s'...\n", filePath);

    FILE *f = fopen(filePath, "r");
    if (!f) {
        fprintf(stderr, "Failed to open file '%s'\n", filePath);
        return 1;
    }

    LexResult r = lex_file(f, filePath);
    fclose(f);

    if (r.has_error) return 1;

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

    return parser->has_error;
}

void post_parse(Parser *p, program_args args) {
    if (args.printstrings) {
#define PRINT_STRING_TABLE_ROW_SIZE 0x40
        for (int row = 0; row < p->factory->string_table.size; row += PRINT_STRING_TABLE_ROW_SIZE) {
            printf("0x%04x: ", row);
            for (int col = 0; row + col < p->factory->string_table.size && col < PRINT_STRING_TABLE_ROW_SIZE; col++)
            {
                char c = p->factory->string_table.strings[row + col];
                if (c == 0) {
                    printf("\u25A1 ");
                }
                else {
                    printf("%c ", c);
                }
            }
            printf("\n");
        }
    }
    if (args.printtree) {
        print_ast(p);
    }

    debugf("Creating Symbol Table...");
    SymbolTable *st = create_symbol_table(p);

    for (int i = 0; i < p->factory->identifiers.size; i++) {
        AstIdent *ident = ((AstIdent *)p->factory->identifiers.array) + i;
        if (!ident->symbol.i) continue;
        printf("IDENT %i => Symbol %i ('%s')\n",
            i + 1,
            ident->symbol.i,
            get_string(p->factory->string_table, ident->name));
    }
        // for (int i = 0; i < st->nsymbols; i++) {
        //     printf("%i: Scope %i StringIndex %i\n", i, st->symbols[i].decl_scope.i, st->symbols[i].name.i);
        // }

        destroy_symbol_table(st);
}