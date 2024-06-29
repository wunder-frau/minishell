#include "minishell.h"

bool	check_redirection(char *str)
{
	int	i;
	int	quote_type;

	i = 0;
	quote_type = 0;
	while (str[i] != NULL_TERM)
	{
		quote_tracker(str, &i, &quote_type);
			if (!quote_type && (str[i] == REDIR_L
				|| str[i] == REDIR_R))
				return (true);
			i++;
	}
	return (false);
}

int	set_node_data_pipe(t_parsed_data **info, char *str, int point, int type)
{
	t_parsed_data *node_info;

	printf("Entering set_node_data_pipe with point=%d, type=%d, str=%s\n", point, type, str);

	if (type == T_PIPE)
			str[point] = NULL_TERM;

	node_info = (t_parsed_data *)calloc(1, sizeof(t_parsed_data));
	if (!node_info)
	{
			perror("calloc");
			return (-1);
	}

	node_info->block_left = str;
	node_info->block_right = str + point + 1;
	node_info->type = type;
	*info = node_info;

	printf("Node info created successfully:\n");
	printf("  block_left: %s\n", node_info->block_left);
	printf("  block_right: %s\n", node_info->block_right);
	printf("  type: %d\n", node_info->type);

	return (1);
}

int	set_node_cmd_redirects(t_parsed_data **info, char *str, int type)
{
	t_parsed_data	*node_info;
	char		*redir;
	int			status;

	node_info = ft_calloc(1, sizeof(t_parsed_data));
	if (!node_info)
		return (-1);
	status = modificate_str_command_without_br(str, &redir, 0, 0);
	if (status == -1)
	{
		free(node_info);
		return (-1);
	}
	node_info->block_left = redir;
	node_info->block_right = str;
	node_info->type = type;
	*info = node_info;

	printf("Node info created successfully:\n");
	printf("  block_left: %s\n", node_info->block_left);
	printf("  block_right: %s\n", node_info->block_right);
	printf("  type: %d\n", node_info->type);
	return (1);
}

int	set_node_cmd(t_parsed_data **info, char *str, int type)
{
	t_parsed_data	*node_info;

	node_info = ft_calloc(1, sizeof(t_parsed_data));
	if (!node_info)
		return (-1);
	node_info->block_left = NULL;
	node_info->block_right = str;
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
	return (ft_is_odd(pair_0) == false && ft_is_odd(pair_1) == false);
}

int block_pipe(t_parsed_data **node, char *str, int type, int i)
{
	if (str[i] == PI)
	{
			printf("Found pipe character at index %d\n", i);
			if (check_symbol_pairing(str, i, S_QUO) && check_symbol_pairing(str, i, D_QUO))
			{
					printf("Valid symbol pairing at index %d\n", i);
					return set_node_data_pipe(node, str, i, type);
			}
			else
			{
					printf("Invalid symbol pairing at index %d, recursing lexer\n", i);
					return lexer(node, str, type, i - 1);
			}
	}
	return 0;
}

int	block_command(t_parsed_data **node, char *str, int type)
{
	if (check_redirection(str))
		return (set_node_cmd_redirects(node, str, type));
	else
		return (set_node_cmd(node, str, T_COMMAND));
	return (0);
}

int lexer(t_parsed_data **node, char *str, int type, int i)
{
	int status;

	printf("Entering lexer with type=%d, i=%d, str=%s\n", type, i, str);

	while (i >= 0)
	{
		if (type == T_PIPE)
		{
			printf("Calling block_pipe with i=%d\n", i);
			status = block_pipe(node, str, type, i);
		}
		else if (type == T_COMMAND)
		{
			printf("Calling block_command with i=%d\n", i);
			status = block_command(node, str, type);
		}
		if (status > 0)
			return (status);
		else if (status < 0)
			return (-1);
		i--;
	}
	printf("Recursive call: lexer(node, str, type + 1, ft_strlen(str) - 1)\n");
	return (lexer(node, str, type + 1,
			ft_strlen(str) - 1));
}
