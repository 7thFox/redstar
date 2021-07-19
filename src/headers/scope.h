#ifndef SCOPE_H
#define SCOPE_H

#include "common.h"
#include "syntaxindex.h"

typedef struct{uint16_t i;} ScopeId;

typedef struct {
    ScopeId     id_parent;
    SyntaxIndex comp_unit;// only when top-level (no parent id)
    SyntaxIndex block;
} STScope;

#endif