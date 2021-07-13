#ifndef SYNTAXKIND_H
#define SYNTAXKIND_H

typedef enum {
    AST_COMPILATION_UNIT,
    AST_USE,
    AST_ATTR_DECL,
    AST_FUNC_DECL,
    AST_IDENT,
    AST_RETURN_STATEMENT,//5
    AST_BINARY_EXPRESSION,
    AST_LOCAL_DECL,
    AST_FUNC_CALL,
} SyntaxKind;

#endif