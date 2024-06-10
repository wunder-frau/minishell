#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "parser.h"
#include "minishell.h"

#include "libft_/libft.h"

enum	e_characters
{
	NULL_TERM,
	HT = 9,
	NL = 10,
	L_REDIR_SEPARATOR = 29,
	R_REDIR_SEPARATOR = 30,
	SEPARATOR = 31,
	SPACE = 32,
	D_QUOTE = 34,
	D_SIGN = 36,
	AND = 38,
	S_QUOTE = 39,
	O_ROUND = 40,
	C_ROUND = 41,
	ASTERISK = 42,
	DASH = 45,
	DOT = 46,
	SLASH = 47,
	COLON = 58,
	SEMICOLON = 59,
	REDIR_L = 60,
	EQUAL = 61,
	REDIR_R = 62,
	Q_MARK = 63,
	BACKSLASH = 92,
	UNSCORE = 95,
	N_LOWER = 110,
	PIPE = 124,
};

typedef enum {
    TOKEN_PIPE,
    TOKEN_BRACKET,
    TOKEN_COMMAND,
    TOKEN_REDIR,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_STRING,
    TOKEN_SPACE,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char *value;
} Token;

Token *create_token(TokenType type, const char *value);
void free_token(Token *token);
// Token **lexer(const char *input, int *num_tokens);
int	lexer(t_node_info **node, char *str, int type, int i);
void free_tokens(Token **tokens, int num_tokens);

#endif
