#include "minishell.h"

void	remove_quotes(char *str, int i, int j)
{
	int	len;
	int	inside_quotes;

	len = ft_strlen(str);
	inside_quotes = 0;
	while (i < len)
	{
		if ((str[i] == S_QUO || str[i] == D_QUO) && !inside_quotes)
			inside_quotes = str[i];
		else if (str[i] == S_QUO && inside_quotes == S_QUO)
			inside_quotes = 0;
		else if (str[i] == D_QUO && inside_quotes == D_QUO)
			inside_quotes = 0;
		else if (str[i] == SPA && !inside_quotes)
			break ;
		else
			str[j++] = str[i];
		i++;
	}
	while (i < len)
		str[j++] = str[i++];
	str[j] = NULL_TERM;
}

void	remove_quotes_arr(char **arr, int i)
{
	while (arr[i])
		remove_quotes(arr[i++], 0, 0);
}

bool	is_blank_string(char *str)
{
	if (ft_strlen(str) == 0)
		return (true);
	while (ft_isspace(*str))
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

char	**wrapper_ft_split_with_quotes(char *str)
{
	char	**res;
	int		i;

	i = 0;
	while (str[i] != NULL_TERM)
	{
		if (str[i] == SPA || str[i] == HT)
			str[i] = SEPARATOR;
		else if (str[i] == D_QUO || str[i] == S_QUO)
			i += skip_quotes(str + i);
		i++;
	}
	res = ft_split(str, SEPARATOR);
	if (!res)
		return (NULL);
	return (res);
}

int	parse_cmd(char *cmd, char ***res, t_minishell *ms)
{
	char	**arr;
	int		i;
	int		status;
  (void)*ms;
	if (is_blank_string(cmd))
		cmd = "";
	arr = wrapper_ft_split_with_quotes(cmd);
	if (!arr)
		return (MALLOC_ERR);
	//i = 0;
	status = 0;
	// while (arr[i] && status == 0)
	// 	status = dollar_sign_expansion(&arr[i++], ms->env, ms->exit_status);
	// if (status == 0)
	// 	status = array_build_before_wc(&arr, -1, -1, -1);
	// if (status == 0)
	// 	status = wildcards(&arr);
	if (status != 0 && arr != NULL)
		ft_free_2d_array(arr);
	if (status == 0)
	{
		remove_quotes_arr(arr, 0);
		*res = arr;
	}
	return (status);
}