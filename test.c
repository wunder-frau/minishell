#include "test.h"

ASTNode* createVarNode(char varName) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        perror("Failed to allocate memory for variable node");
        exit(EXIT_FAILURE);
    }
    node->type = NODE_VAR;
    node->data.varName = varName;
    return node;
}

ASTNode* createConstNode(int value) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        perror("Failed to allocate memory for constant node");
        exit(EXIT_FAILURE);
    }
    node->type = NODE_CONST;
    node->data.constValue = value;
    return node;
}

ASTNode* createOpNode(OpType op, ASTNode* left, ASTNode* right) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        perror("Failed to allocate memory for operator node");
        exit(EXIT_FAILURE);
    }
    node->type = NODE_OP;
    node->data.op.op = op;
    node->data.op.left = left;
    node->data.op.right = right;
    return node;
}

void printAST(ASTNode* node) {
    if (node == NULL) return;

    switch (node->type) {
        case NODE_VAR:
            printf("%c", node->data.varName);
            break;
        case NODE_CONST:
            printf("%d", node->data.constValue);
            break;
        case NODE_OP:
            switch (node->data.op.op) {
                case OP_ASSIGN:
                    printAST(node->data.op.left);
                    printf(" = ");
                    printAST(node->data.op.right);
                    break;
                case OP_ADD:
                    printf("(");
                    printAST(node->data.op.left);
                    printf(" + ");
                    printAST(node->data.op.right);
                    printf(")");
                    break;
                case OP_MUL:
                    printf("(");
                    printAST(node->data.op.left);
                    printf(" * ");
                    printAST(node->data.op.right);
                    printf(")");
                    break;
            }
            break;
    }
}

void freeAST(ASTNode* node) {
    if (node == NULL) return;

    if (node->type == NODE_OP) {
        freeAST(node->data.op.left);
        freeAST(node->data.op.right);
    }

    free(node);
}

int countNodes(ASTNode* node) {
    if (node == NULL) return 0;

    if (node->type == NODE_OP) {
        return 1 + countNodes(node->data.op.left) + countNodes(node->data.op.right);
    }

    return 1;
}

int evaluateAST(ASTNode* node, int* variables) {
    if (node == NULL) {
        fprintf(stderr, "Error: NULL node encountered during evaluation\n");
        exit(EXIT_FAILURE);
    }

    switch (node->type) {
        case NODE_VAR:
            return variables[node->data.varName - 'a']; // Assuming 'a' to 'z'
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
    ASTNode* varX = createVarNode('x');
    ASTNode* varA = createVarNode('a');
    ASTNode* varB = createVarNode('b');
    ASTNode* varC = createVarNode('c');

    ASTNode* mulNode = createOpNode(OP_MUL, varB, varC);

    ASTNode* addNode = createOpNode(OP_ADD, varA, mulNode);

    ASTNode* assignNode = createOpNode(OP_ASSIGN, varX, addNode);

    printf("AST: ");
    printAST(assignNode);
    printf("\n");

    int variables[26] = {0};

    variables['a' - 'a'] = 2;  // a = 2
    variables['b' - 'a'] = 5;  // b = 3
    variables['c' - 'a'] = 6;  // c = 4

    int result = evaluateAST(assignNode, variables);

    printf("Result: %d\n", result);
    printf("Value of x: %d\n", variables['x' - 'a']);

    freeAST(assignNode);

    return 0;
}

