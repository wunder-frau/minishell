#include "minishell.h"

bool	check_redirection(char *str)
{
	int i = 0;
	int inside_quotes = 0;

	while (str[i] != NULL_TERM)
	{
		if (!is_inside_quotes(str[i], &inside_quotes) &&
			(str[i] == REDIR_L || str[i] == REDIR_R))
			{
				printf("😊👍😊👍😊👍😊👍😊👍😊👍Found redirection symbol '%c' outside of quotes\n", str[i]);
				return (true);
			}
			i++;
	}
	return (false);
}

int	set_node_data_pipe(t_parsed_data **data, char *str, int point, int type)
{
	t_parsed_data *node_data;

	printf("Entering set_node_data_pipe with point=%d, type=%d, str=%s\n", point, type, str);

	if (type == T_PIPE)
			str[point] = NULL_TERM;

	node_data = (t_parsed_data *)ft_calloc(1, sizeof(t_parsed_data));
	if (!node_data)
	{
			perror("calloc");
			return (-1);
	}

	node_data->block_left = str;
	node_data->block_right = str + point + 1;
	node_data->type = type;
	*data = node_data;

	printf("Node data created successfully:\n");
	printf("  block_left: %s\n", node_data->block_left);
	printf("  block_right: %s\n", node_data->block_right);
	printf("  type: %d\n", node_data->type);

	return (1);
}

int	set_node_cmd_redirects(t_parsed_data **data, char *str, int type)
{
	t_parsed_data	*node_data;
	char		*redir;
	int			status;

	node_data = ft_calloc(1, sizeof(t_parsed_data));
	if (!node_data)
		return (-1);
	status = convert_input_to_redirects(str, &redir, 0, 0);
	if (status == -1)
	{
		free(node_data);
		return (-1);
	}
	node_data->block_left = redir;
	node_data->block_right = str;
	node_data->type = type;
	*data = node_data;

	printf("Node data created successfully:\n");
	printf("  block_left: %s\n", node_data->block_left);
	printf("  block_right: %s\n", node_data->block_right);
	printf("  type: %d\n", node_data->type);
	return (1);
}

int	set_node_cmd(t_parsed_data **data, char *str, int type)
{
	t_parsed_data	*node_data;

	node_data = ft_calloc(1, sizeof(t_parsed_data));
	if (!node_data)
		return (-1);
	node_data->block_left = NULL;
	node_data->block_right = str;
	node_data->type = type;
	*data = node_data;


	printf("Node CMD created successfully:\n");
	printf("  block_left: %s\n", node_data->block_left);
	printf("  block_right: %s\n", node_data->block_right);
	printf("  type: %d\n", node_data->type);
	return (1);
}

int check_symbol_pairing(char *str, int point, int symbol)
{
	int last_str_index;
	int pair_0;
	int pair_1;

	last_str_index = (int)ft_strlen(str) - 1;
	pair_0 = 0;
	pair_1 = 0;
	while (last_str_index >= point)
	{
		if (str[last_str_index] == symbol)
				pair_0++;
		last_str_index--;
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

int	block_cmd(t_parsed_data **node, char *str, int type)
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
	int last_str_index;

	last_str_index = ft_strlen(str) - 1;
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
			printf("Calling block_cmd with i=%d\n", i);
			status = block_cmd(node, str, type);
		}
		if (status > 0)
			return (status);
		else if (status < 0)
			return (-1);
		i--;
	}
	printf("Recursive call: lexer(node, str, type + 1, ft_strlen(str) - 1)\n");
	return (lexer(node, str, type + 1,
			last_str_index));
}
