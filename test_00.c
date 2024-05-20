#include "test_00.h"

ASTNode* createVarNode(char varName) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_VAR;
    node->data.varName = varName;
    return node;
}

ASTNode* createConstNode(int constValue) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_CONST;
    node->data.constValue = constValue;
    return node;
}

ASTNode* createOpNode(OpType op, ASTNode* left, ASTNode* right) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_OP;
    node->data.op.op = op;
    node->data.op.left = left;
    node->data.op.right = right;
    return node;
}

int evaluateAST(ASTNode* node, int* variables) {
    if (node == NULL) {
        fprintf(stderr, "Error: NULL node encountered during evaluation\n");
        exit(EXIT_FAILURE);
    }

    switch (node->type) {
        case NODE_VAR:
            return variables[node->data.varName - 'a'];
        case NODE_CONST:
            return node->data.constValue;
        case NODE_OP:
            switch (node->data.op.op) {
                case OP_ASSIGN:
                    variables[node->data.op.left->data.varName - 'a'] = evaluateAST(node->data.op.right, variables);
                    return variables[node->data.op.left->data.varName - 'a'];
                case OP_ADD:
                    return evaluateAST(node->data.op.left, variables) + evaluateAST(node->data.op.right, variables);
                case OP_MUL:
                    return evaluateAST(node->data.op.left, variables) * evaluateAST(node->data.op.right, variables);
                default:
                    fprintf(stderr, "Error: Unknown operation type\n");
                    exit(EXIT_FAILURE);
            }
        default:
            fprintf(stderr, "Error: Unknown node type\n");
            exit(EXIT_FAILURE);
    }
}

int main() {
    // Array to store variable values (assuming variables 'a' to 'z')
    int variables[26] = {0};

    // Construct the AST for the expression: a = 2 + 3 * 4
    ASTNode* expr = createOpNode(OP_ASSIGN,
                                 createVarNode('a'),
                                 createOpNode(OP_ADD,
                                              createConstNode(2),
                                              createOpNode(OP_MUL,
                                                           createConstNode(3),
                                                           createConstNode(4))));

    int result = evaluateAST(expr, variables);

    printf("Result: %d\n", result);
    printf("Value of a: %d\n", variables['a' - 'a']);

    free(expr->data.op.right->data.op.right);
    free(expr->data.op.right->data.op.left);
    free(expr->data.op.right);
    free(expr->data.op.left);
    free(expr);

    return 0;
}
