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

typedef struct ASTNode {
    NodeType type;
    union {
        char varName;
        int constValue;
        struct {
            OpType op;
            struct ASTNode *left;
            struct ASTNode *right;
        } op;
    } data;
} ASTNode;

ASTNode* createVarNode(char varName);
ASTNode* createConstNode(int value);
ASTNode* createOpNode(OpType op, ASTNode* left, ASTNode* right);
void printAST(ASTNode* node);
void freeAST(ASTNode* node);
int countNodes(ASTNode* node);
int evaluateAST(ASTNode* node, int* variables);

#endif
