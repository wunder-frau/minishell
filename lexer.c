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

void	check_if_inside_quotes(char *str, int *i, int *quote_type)
{
   	if (str[*i] == S_QUOTE && *quote_type == S_QUOTE)
		*quote_type = 0;
	else if (str[*i] == S_QUOTE && *quote_type == 0)
		*quote_type = S_QUOTE;
	else if (str[*i] == D_QUOTE && *quote_type == D_QUOTE)
		*quote_type = 0;
	else if (str[*i] == D_QUOTE && *quote_type == 0)
		*quote_type = D_QUOTE;
}

int	round_brackets_check(char *str, int point)
{
	int	i;
	int	unmatched_count;
	int	quote_type;

	i = (int)ft_strlen(str) - 1;
	unmatched_count = 0;
	quote_type = 0;
	while (i >= point)
	{
		check_if_inside_quotes(str, &i, &quote_type);
		if (!quote_type)
		{
			if (str[i] == O_ROUND)
				unmatched_count++;
			if (str[i] == C_ROUND)
				unmatched_count--;
			i--;
		}
		else
			i--;
	}
	return (!unmatched_count);
}

int set_node_info(t_node_info **info, char *str, int point, int type)
{
    t_node_info *node_info;

    if (type == T_PIPE)
        str[point] = NULL_TERM;
    node_info = ft_calloc(1, sizeof(t_node_info));
    if (!node_info)
        return (-1);
    node_info->str_left = str;
    node_info->str_right = str + point + 1;
    node_info->type = type;
    *info = node_info;

    printf("point: %d\n", point);
    printf("str[point]: %c\n", str[point]);
    printf("Node info created successfully:\n");
    printf("  str_left: %s\n", node_info->str_left);
    printf("  str_right: %s\n", node_info->str_right);
    printf("  type: %d\n", node_info->type);

    return (-1);
}

int	pipe_block(t_node_info **node, char *str, int type, int i)
{
    //(void)**node;
    //(void)type;
    if (str[i] == PIPE)
    {
        // to remove
        if (round_brackets_check(str, i))
        {
            return(set_node_info(node, str, i, type));
            printf("set_node");
            printf("\n");
        }
        else
        {
        //return(lexer(node, str, type, i - 1));
        printf("str[i]_block: %d\n", str[i]);
        printf("[i]_block: %d\n", i);
        printf("type: %d", str[i]);
        printf("lexer recursion");
        printf("\n");
        }
    }
    // else
    // {
        printf("NOT_PIPE");
        printf("\n");
    // }
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
        if (status > 0)
			return (status);
		else if (status < 0)
			return (-1);
		i--;
	}
	return (lexer(node, str, type + 1,
			ft_strlen(str) - 1));
}
