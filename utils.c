#include "minishell.h"

bool is_inside_quotes(char c, int *inside_quotes)
{
	if ((c == S_QUO || c == D_QUO) && *inside_quotes == 0)
	{
		*inside_quotes = c;
		// printf("🎉 🌟 🍕Entering quotes '%c'\n", c);
		return true;
	}
	else if (c == S_QUO && *inside_quotes == S_QUO)
	{
		*inside_quotes = 0;
		// printf("🎉 🌟 🍕Leaving single quotes\n");
		return true;
	}
	else if (c == D_QUO && *inside_quotes == D_QUO)
	{
		*inside_quotes = 0;
		// printf("🎉 🌟 🍕Leaving double quotes\n");
		return true;
	}
	return false;
}

void remove_quotes(char *str, int i, int j)
{
	int len = ft_strlen(str);
	int inside_quotes = 0;

	while (i < len)
	{
		if (!is_inside_quotes(str[i], &inside_quotes))
		{
			str[j++] = str[i];
		}
		i++;
	}
	while (i < len)
		str[j++] = str[i++];
	str[j] = NULL_TERM;
	// printf("🔚 Resulting string after removing quotes: [::%s]\n", str);
}

void	remove_quotes_arr(char **arr, int i)
{
	while (arr[i])
	{
		remove_quotes(arr[i], 0, 0);
		// printf("\n");
		// printf("🤪🤪%s\n", arr[i]);
		i++;
	}
}

bool	is_blank_string(char *str)
{
	if (ft_strlen(str) == 0)
		return (true);
	while (ft_is_space(*str))
		str++;
	return (*str == NULL_TERM);
}

static int	skip_quotes(char *str)
{
	int	i;

	i = 0;
	if (*str == D_QUO)
	{
		i++;
		while (str[i] != D_QUO)
			i++;
	}
	else if (*str == S_QUO)
	{
		i++;
		while (str[i] != S_QUO)
			i++;
	}
	return (i);
}

char	**split_handle_quotes_and_spaces(char *str)
{
	char	**res;
	int		i;

	i = 0;
	while (str[i] != NULL_TERM)
	{
		if (ft_is_space(str[i]))
			str[i] = SEPARATOR;
		else if (str[i] == D_QUO || str[i] == S_QUO)
			i += skip_quotes(str + i);
		i++;
	}
	res = ft_split(str, SEPARATOR);
	if (!res)
		return (NULL);
		 i = 0;
	while (res[i])
	{
		// printf("\n");
		// printf("🎉%s\n", res[i]);
		i++;
	}
	return (res);
}

int	parse_cmd(char *cmd, char ***res, t_minishell *ms)
{
	char	**arr;
	int		status;
	int		i;
  (void)*ms;
	if (is_blank_string(cmd))
	{
		cmd = "";
	}
	arr = split_handle_quotes_and_spaces(cmd);
	if (!arr)
		return (MALLOC_ERR);
	status = 0;
	i = 0;
	while (arr[i] && status == 0)
		status = dollar_expansion(&arr[i++], ms, status);
	if (status != 0 && arr != NULL)
		free_arr_2d(arr);
	if (status == 0)
	{
		remove_quotes_arr(arr, 0);
		*res = arr;
	}
	return (status);
}
