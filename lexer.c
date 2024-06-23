#include "minishell.h"

bool is_odd(int num)
{
	return (num % 2 != 0);
}


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
	if (str[*i] == S_QUO && *quote_type == S_QUO)
		*quote_type = 0;
	else if (str[*i] == S_QUO && *quote_type == 0)
		*quote_type = S_QUO;
	else if (str[*i] == D_QUO && *quote_type == D_QUO)
		*quote_type = 0;
	else if (str[*i] == D_QUO && *quote_type == 0)
		*quote_type = D_QUO;
}

int	redir_search(char *str)
{
	int	i;
	int	quote_type;
	int	key;

	i = 0;
	quote_type = 0;
	key = 0;
	while (str[i] != NULL_TERM)
	{
		check_if_inside_quotes(str, &i, &quote_type);
		if (!quote_type)
		{
			if (str[i] == REDIR_L || str[i] == REDIR_R)
				key++;
			i++;
		}
		else
			i++;
	}
	return (key > 0);
}

// int	round_brackets_check(char *str, int point)
// {
// 	int	i;
// 	int	unmatched_count;
// 	int	quote_type;

// 	i = (int)ft_strlen(str) - 1;
// 	unmatched_count = 0;
// 	quote_type = 0;
// 	while (i >= point)
// 	{
// 		check_if_inside_quotes(str, &i, &quote_type);
// 		if (!quote_type)
// 		{
// 			if (str[i] == O_ROU)
// 				unmatched_count++;
// 			if (str[i] == C_ROU)
// 				unmatched_count--;
// 			i--;
// 		}
// 		else
// 			i--;
// 	}
// 	return (!unmatched_count);
// }
int set_node_info(t_node_info **info, char *str, int point, int type)
{
    t_node_info *node_info;

    printf("Entering set_node_info with point=%d, type=%d, str=%s\n", point, type, str);

    if (type == T_PIPE)
        str[point] = '\0'; // Properly null-terminate the string

    node_info = (t_node_info *)calloc(1, sizeof(t_node_info));
    if (!node_info)
    {
        perror("calloc");
        return (-1);
    }

    node_info->str_left = str;
    node_info->str_right = str + point + 1;
    node_info->type = type;
    *info = node_info;

    printf("Node info created successfully:\n");
    printf("  str_left: %s\n", node_info->str_left);
    printf("  str_right: %s\n", node_info->str_right);
    printf("  type: %d\n", node_info->type);

    return (1);
}


// int set_node_info(t_node_info **info, char *str, int point, int type)
// {
//     t_node_info *node_info;

//     if (type == T_PIPE)
//         str[point] = NULL_TERM;
//     node_info = ft_calloc(1, sizeof(t_node_info));
//     if (!node_info)
//         return (-1);
//     node_info->str_left = str;
//     node_info->str_right = str + point + 1;
//     node_info->type = type;
//     *info = node_info;
    
//     printf("point: %d\n", point);
//     printf("str[point]: %c\n", str[point]);
//     printf("Node info created successfully:\n");
//     printf("  str_left: %s\n", node_info->str_left);
//     printf("  str_right: %s\n", node_info->str_right);
//     printf("  type: %d\n", node_info->type);

//     return (1);
// }

int	set_node_info_command(t_node_info **info, char *str, int type)
{
	t_node_info	*node_info;
	char		*redir;
	int			status;

	node_info = ft_calloc(1, sizeof(t_node_info));
	if (!node_info)
		return (-1);
	status = modificate_str_command_without_br(str, &redir, 0, 0);
	if (status == -1)
	{
		free(node_info);
		return (-1);
	}
	node_info->str_left = redir;
	node_info->str_right = str;
	node_info->type = type;
	*info = node_info;

	printf("Node info created successfully:\n");
	printf("  str_left: %s\n", node_info->str_left);
	printf("  str_right: %s\n", node_info->str_right);
	printf("  type: %d\n", node_info->type);
	return (1);
}

int	set_node_cmd_simple(t_node_info **info, char *str, int type)
{
	t_node_info	*node_info;

	node_info = ft_calloc(1, sizeof(t_node_info));
	if (!node_info)
		return (-1);
	node_info->str_left = NULL;
	node_info->str_right = str;
	node_info->type = type;
	*info = node_info;
	return (1);
}

int check_symbol_pairing(char *str, int point, int symbol)
{
    int i;
    int pair_0;
    int pair_1;

    i = (int)ft_strlen(str) - 1;
    pair_0 = 0;
    pair_1 = 0;
    while (i >= point)
    {
        if (str[i] == symbol)
            pair_0++;
        i--;
    }
    while (--point > 0)
    {
        if (str[point] == symbol)
            pair_1++;
    }
    return (is_odd(pair_0) == false && is_odd(pair_1) == false);
}

int pipe_block(t_node_info **node, char *str, int type, int i)
{
    if (str[i] == PI)
    {
        printf("Found pipe character at index %d\n", i);
        if (check_symbol_pairing(str, i, S_QUO) && check_symbol_pairing(str, i, D_QUO))
        {
            printf("Valid symbol pairing at index %d\n", i);
            return set_node_info(node, str, i, type);
        }
        else
        {
            printf("Invalid symbol pairing at index %d, recursing lexer\n", i);
            return lexer(node, str, type, i - 1);
        }
    }
    return 0;
}

// int pipe_block(t_node_info **node, char *str, int type, int i)
// {
//     if (str[i] == PI)
//     {
//         if (check_symbol_pairing(str, i, S_QUO) && check_symbol_pairing(str, i, D_QUO))
//         {
//             printf("set_node\n");
//             return set_node_info(node, str, i, type);
//         }
//         else
//         {
//             printf("Invalid symbol pairing at index %d\n", i);
//             return (lexer(node, str, type, i - 1));
//         }
//     }
//     return 0;
// }

// int	pipe_block(t_node_info **node, char *str, int type, int i)
// {
//     if (str[i] == PI)
//     {
//         if (check_symbol_pairing(str, i, S_QUO) && check_symbol_pairing(str, i, D_QUO))
//         {
//             printf("set_node");
//             printf("\n");
//             return(set_node_info(node, str, i, type));
//         }
//         else
//         {
//             printf("str[i]_block: %d\n", str[i]);
//             printf("[i]_block: %d\n", i);
//             printf("type: %d", str[i]);
//             printf("lexer recursion");
//             printf("\n");
//             return(lexer(node, str, type, i - 1));
//         }
//     }
//     return (0);
// }

int	command_without_bracket_block(t_node_info **node, char *str, int type)
{
	(void)type;
	if (redir_search(str))
		return (set_node_info_command(node, str, type));
	else
		return (set_node_cmd_simple(node, str, T_COMMAND));
	return (0);
}

int lexer(t_node_info **node, char *str, int type, int i)
{
	int status;

	printf("Entering lexer with type=%d, i=%d, str=%s\n", type, i, str);

	while (i >= 0)
	{
		if (type == T_PIPE)
		{
			printf("Calling pipe_block with i=%d\n", i);
			status = pipe_block(node, str, type, i);
		}
		else if (type == T_COMMAND)
		{
			printf("Calling command_without_bracket_block with i=%d\n", i);
			status = command_without_bracket_block(node, str, type);
		}
		else 
		{
			printf("Unexpected type: %d\n", type);
			status = -1;
		}

		if (status > 0)
		{
			printf("lexer returning with status=%d\n", status);
			return (status);
		}
		else if (status < 0)
		{
			printf("lexer encountered an error with status=%d\n", status);
			return (-1);
		}

		i--;
	}

    printf("Recursive call: lexer(node, str, type + 1, ft_strlen(str) - 1)\n");
    return (lexer(node, str, type + 1, ft_strlen(str) - 1));
}

// int lexer(t_node_info **node, char *str, int type, int i)
// {
//     int status;

//     while (i >= 0)
//     {
//         if (type == T_PIPE)
//         {
//             printf("Calling pipe_block with i=%d\n", i);
//             status = pipe_block(node, str, type, i);
//         }
//         else 
//         {
//             printf("Unexpected type: %d\n", type);
//             status = -1;
//         }

//         if (status > 0)
//             return (status);
//         else if (status < 0)
//             return (-1);

//         i--;
//     }

//     printf("Recursive call: lexer(node, str, type + 1, ft_strlen(str) - 1)\n");
//     return (lexer(node, str, type + 1, ft_strlen(str) - 1));
// }

// int lexer(t_node_info **node, char *str, int type,
// 	int i)
// {
// 	int	status;

// 	while (i >= 0)
// 	{
// 		if (type == T_PIPE)
// 			status = pipe_block(node, str, type, i);
//         else 
//         {
//             printf("__lol_err___");
//             status = -1;
//         }
//         if (status > 0)
// 			return (status);
// 		else if (status < 0)
// 			return (-1);
// 		i--;
// 	}
// 	return (lexer(node, str, type + 1,
// 			ft_strlen(str) - 1));
// }
