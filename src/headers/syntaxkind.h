#ifndef SYNTAXKIND_H
#define SYNTAXKIND_H

typedef enum {
    AST_COMPILATION_UNIT,
    AST_USE,
    AST_ATTR_DECL,
    AST_FUNC_DECL,
    AST_IDENT,
    AST_BINARY_EXPRESSION,
} SyntaxKind;

#endif