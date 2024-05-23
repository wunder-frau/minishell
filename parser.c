#include "parser.h"

t_and   *create_and_node(void) {
    t_and *node = (t_and *)calloc(1, sizeof(t_and));
    if (!node)
        return NULL;
    node->type = T_AND;
    return node;
}

t_or    *create_or_node(void) {
    t_or *node = (t_or *)calloc(1, sizeof(t_or));
    if (!node)
        return NULL;
    node->type = T_OR;
    return node;
}

t_pipe  *create_pipe_node(void)
{
	t_pipe	*node;

	node = (t_pipe *)calloc(1, sizeof(t_pipe));
	if (!node)
		return (NULL);
	node->type = T_PIPE;
	return (node);
}

t_bracket *create_bracket_node(void) {
    t_bracket *node = (t_bracket *)calloc(1, sizeof(t_bracket));
    if (!node)
        return NULL;
    node->type = T_BRACKET;
    return node;
}

bool create_node(NodeType type, ASTNode **root) {
    ASTNode *node = NULL;
    switch (type) {
        case T_AND:
            node = (ASTNode *)create_and_node();
            break;
        case T_OR:
            node = (ASTNode *)create_or_node();
            break;
        case T_BRACKET:
            node = (ASTNode *)create_bracket_node();
            break;
        case T_PIPE:
            node = (ASTNode *)create_pipe_node();
            break;
        default:
            return false;
    }

    if (node == NULL)
        return false;

    *root = node;
    return true;
}

void free_tree(ASTNode **root) {
    if (*root == NULL)
        return;

    free_tree(&((*root)->left));
    free_tree(&((*root)->right));
    free(*root);
    *root = NULL;
}

void print_ast(const ASTNode *root) {
    if (root == NULL)
        return;

    switch (root->type) {
        case T_AND:
            printf("T_AND\n");
            break;
        case T_OR:
            printf("T_OR\n");
            break;
        case T_BRACKET:
            printf("T_BRACKET\n");
            break;
        case T_PIPE:
            printf("T_PIPE\n");
            break;
        default:
            printf("Unknown type\n");
            break;
    }

    print_ast(root->left);
    print_ast(root->right);
}

int main() {
    ASTNode *root = NULL;

    if (!create_node(T_AND, &root)) {
        fprintf(stderr, "Failed to create root node\n");
        return EXIT_FAILURE;
    }

    if (!create_node(T_OR, &(root->left))) {
        fprintf(stderr, "Failed to create left child node\n");
        free_tree(&root);
        return EXIT_FAILURE;
    }

    if (!create_node(T_PIPE, &(root->right))) {
        fprintf(stderr, "Failed to create right child node\n");
        free_tree(&root);
        return EXIT_FAILURE;
    }

    if (!create_node(T_BRACKET, &(root->left->left))) {
        fprintf(stderr, "Failed to create right child node\n");
        free_tree(&root);
        return EXIT_FAILURE;
    }

    printf("AST Structure:\n");
    print_ast(root);

    free_tree(&root);

    return EXIT_SUCCESS;
}