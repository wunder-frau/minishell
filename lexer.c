#include "parser.h"
#include "minishell.h"
#include "lexer.h"

int ft_isspace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

char *ft_strndup(const char *s, size_t n) {
    size_t len = 0;
    while (len < n && s[len] != '\0')
        len++;
    char *new_str = (char *)malloc(len + 1);
    if (new_str == NULL)
        return NULL;
    size_t i = 0;
    while (i < len)
    {
       new_str[i] = s[i];
       i++;
    }
    new_str[len] = '\0';

    return new_str;
}

Token *create_token(TokenType type, const char *value) {
    Token *token = malloc(sizeof(Token));
    if (!token) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    token->type = type;
    token->value = ft_strdup(value);
    if (!token->value) {
        perror("strdup");
        free(token);
        exit(EXIT_FAILURE);
    }
    return token;
}

void free_token(Token *token) {
    if (token) {
        free(token->value);
        free(token);
    }
}

int	lexer(t_node_info **node, char *str, int type,
	int i)
{
	int	status;

	while (i >= 0)
	{
		// if (type == T_AND || type == T_OR)
		// 	status = and_if_condition_block(node, str, type, i);
		if (type == T_PIPE)
			status = pipe_block(node, str, type, i);
		// else if (type == T_BRACKET)
		// 	status = brackets_block(node, str, type);
		// else if (type == T_COMMAND)
		// 	status = command_without_bracket_block(node, str, type);
		if (status > 0)
			return (status);
		else if (status < 0)
			return (-1);
		i--;
	}
	return (lexer(node, str, type + 1,
			ft_strlen(str) - 1));
}
// Token **lexer(const char *input, int *num_tokens) {
//     int length = ft_strlen(input);
//     int capacity = length + 1;
//     Token **tokens = malloc(capacity * sizeof(Token *));
//     if (!tokens) {
//         perror("malloc");
//         exit(EXIT_FAILURE);
//     }

//     int i = 0, token_count = 0;
//     while (i < length)
//     {
//         if (ft_isspace(input[i])) {
//             tokens[token_count++] = create_token(TOKEN_SPACE, " ");
//             while (ft_isspace(input[i])) i++;
//         } else if (input[i] == '|') {
//             tokens[token_count++] = create_token(TOKEN_PIPE, "|");
//             i++;
//         } else if (input[i] == '(' || input[i] == ')') {
//             char bracket[2] = {input[i], '\0'};
//             tokens[token_count++] = create_token(TOKEN_BRACKET, bracket);
//             i++;
//         } else if (input[i] == '&' && input[i+1] == '&') {
//             tokens[token_count++] = create_token(TOKEN_AND, "&&");
//             i += 2;
//         } else if (input[i] == '|' && input[i+1] == '|') {
//             tokens[token_count++] = create_token(TOKEN_OR, "||");
//             i += 2;
//         } else {
//             int start = i;
//             while (i < length && !ft_isspace(input[i]) && input[i] != '|' &&
//                    input[i] != '(' && input[i] != ')' && input[i] != '&') {
//                 i++;
//             }
//             char *substring = ft_strndup(&input[start], i - start);
//             if (!substring) {
//                 perror("strndup");
//                 exit(EXIT_FAILURE);
//             }
//             tokens[token_count++] = create_token(TOKEN_STRING, substring);
//             free(substring);
//         }
//     }

//     *num_tokens = token_count;
//     return tokens;
// }

void free_tokens(Token **tokens, int num_tokens) {
    for (int i = 0; i < num_tokens; i++) {
        free_token(tokens[i]);
    }
    free(tokens); 
}
