#include "minishell.h"

// ! remove later `ft_isspace` and `ft_strndup`

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

int	pipe_block(t_node_info **node, char *str, int type, int i)
{
    (void)**node;
    (void)type;
    if (str[i] == PIPE)
    {
        printf("PIPE_pipe_block");
        printf("\n");
    }
    else
    {
        printf("NOT_PIPE");
        printf("\n");
    }
    return (0);
}

int lexer(t_node_info **node, char *str, int type,
	int i)
{
	int	status;

	while (i >= 0)
	{
		if (type == T_PIPE)
			status = pipe_block(node, str, type, i);
		i--;
	}
	// return (lexer(node, str, type + 1,
	// 		ft_strlen(str) - 1));
    return (0);
}
