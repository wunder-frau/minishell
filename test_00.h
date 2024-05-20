#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>

typedef enum {
    NODE_VAR,
    NODE_CONST,
    NODE_OP
} NodeType;

typedef enum {
    OP_ASSIGN,
    OP_ADD,
    OP_MUL
} OpType;

typedef struct {
    OpType op;
    struct ASTNode* left;
    struct ASTNode* right;
} OpNode;

typedef union {
    char varName;
    int constValue;
    OpNode op;
} NodeData;

typedef struct ASTNode {
    NodeType type;
    NodeData data;
} ASTNode;

#endif
