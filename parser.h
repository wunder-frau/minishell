#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum
{
	T_AND,
	T_OR,
	T_PIPE,
	T_BRACKET,
	T_COMMAND_BR,
} NodeType;

typedef struct ASTNode
{
    int type;
    struct ASTNode *left;
    struct ASTNode *right;
} ASTNode;

typedef struct s_and
{
	int		type;
	ASTNode	*left;
	ASTNode	*right;
}	t_and;

typedef struct s_or
{
	int		type;
	ASTNode	*left;
	ASTNode	*right;
}	t_or;

typedef struct s_bracket
{
	int		type;
	ASTNode	*left;
}	t_bracket;


bool create_node(NodeType type, ASTNode **root);
void free_tree(ASTNode **root);
void print_ast(const ASTNode *root);

#endif
