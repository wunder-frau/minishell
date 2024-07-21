#include "minishell.h"
#include <stdio.h> // For debug print

#include "minishell.h"
#include <stdio.h> // For debug print

bool is_inside_quotes(char c, int *inside_quotes)
{
    if ((c == S_QUO || c == D_QUO) && *inside_quotes == 0)
    {
        *inside_quotes = c;
        return true;
    }
    else if (c == S_QUO && *inside_quotes == S_QUO)
    {
        *inside_quotes = 0;
        return true;
    }
    else if (c == D_QUO && *inside_quotes == D_QUO)
    {
        *inside_quotes = 0;
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
	str[j] = NULL_TERM;
}

void remove_quotes_arr(char **arr, int i)
{
	while (arr[i])
	{
		remove_quotes(arr[i], 0, 0);
		i++;
	}
}

bool is_blank_string(char *str)
{
	if (ft_strlen(str) == 0)
		return true;
	while (ft_is_space(*str))
		str++;
	return (*str == NULL_TERM);
}

static int skip_quotes(char *str)
{
	int i = 0;

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
	return i;
}

char **split_handle_quotes_and_spaces(char *str)
{
	char **res;
	int i = 0;

	while (str[i] != NULL_TERM)
	{
		if (ft_is_space(str[i]))
			str[i] = SEPARATOR;
		else if (str[i] == D_QUO || str[i] == S_QUO)
			i += skip_quotes(str + i);
		i++;
	}
	res = ft_split(str, SEPARATOR);
	return res;
}

int parse_cmd(char *cmd, char ***res, t_minishell *ms)
{
    char **arr;
    int status = 0;
    int i = 0;

    if (is_blank_string(cmd))
    {
        cmd = "";
    }
    arr = split_handle_quotes_and_spaces(cmd);
    if (!arr)
        return MALLOC_ERR;
    while (arr[i] && status == 0)
    {
        if (arr[i][0] != '\'') // Skip single-quoted strings
        {
            status = dollar_expansion(&arr[i], ms, ms->exit_status);
        }
        i++;
    }

    if (status != 0 && arr != NULL)
        free_arr_2d(arr);
    if (status == 0)
    {
        remove_quotes_arr(arr, 0);
        *res = arr;
    }
    return status;
}
