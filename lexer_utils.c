#include "minishell.h"

void	quote_tracker(char *str, int *i, int *quote_type)
{
	char cur_char = str[*i];

	if ((cur_char == S_QUO && *quote_type == S_QUO)
	|| (cur_char == D_QUO && *quote_type == D_QUO))
	{
		printf("inside____________quotes");
		printf("\n");
		*quote_type = 0;
	}
	else if (cur_char == S_QUO || cur_char == D_QUO)
	{
		printf("out_____of______quotes");
		printf("\n");
		*quote_type = cur_char;
	}
}

static void	modificate_str_utils(char *str, char *r, int *i, int *j)
{
	r[*j] = str[*i];
	str[*i] = SPA;
	(*j)++;
	(*i)++;
}

static int	is_special_char(char c)
{
	return (c == REDIR_L || c == REDIR_R || c == SPA || c == HT);
}

static void	rep_redir_with_sep(char *str, char **redir, int *i, int *j)
{
	int q_flag = 0;

	while (is_special_char(str[*i]) && str[*i] != NULL_TERM)
		modificate_str_utils(str, *redir, i, j);
	while (str[*i] != NULL_TERM)
	{
		quote_tracker(str, i, &q_flag);
		if (q_flag == 0 && (is_special_char(str[*i]) || str[*i] == NULL_TERM))
				break;
		else
				modificate_str_utils(str, *redir, i, j);
	}
	(*redir)[(*j)++] = SEPARATOR;
	(*i)--;
}

int	modificate_str_command_without_br(char *str, char **redir, int i,
	int j)
{
	int	q_flag;

	q_flag = 0;
	if (!str)
		return (-1);
	*redir = (char *)ft_calloc(ft_strlen(str) * 2, sizeof(char));
	if (!*redir)
		return (-1);
	while (str[i] != NULL_TERM)
	{
		quote_tracker(str, &i, &q_flag);
		if (q_flag == 0 && (str[i] == REDIR_L || str[i] == REDIR_R))
			rep_redir_with_sep(str, redir, &i, &j);
		if (str[i] == NULL_TERM)
			break ;
		i++;
	}
	return (1);
}