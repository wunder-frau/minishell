#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef enum
{
	T_PIPE,
    T_BRACKET,
    T_COMMAND,
    T_REDIR,
    T_AND,
    T_OR,
    T_STRING,
    T_SPACE,
    T_UNKNOWN
} NodeType;

typedef struct {
    NodeType type;
    char *value;
} Token;

typedef struct ASTNode
{
    int type;
    struct ASTNode *left;
    struct ASTNode *right;
} ASTNode;

typedef struct s_node_info
{
	int		type;
	char	*str_left;
	char	*str_right;
}	t_node_info;

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
	ASTNode *right;
}	t_bracket;

typedef struct s_pipe
{
	int		type;
	ASTNode	*left;
	ASTNode	*right;
}	t_pipe;

bool create_node(NodeType type, ASTNode **root);
void free_tree(ASTNode **root);
void print_ast(const ASTNode *root);

#endif
