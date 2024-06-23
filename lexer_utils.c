#include "minishell.h"

// char	*command_part(char *str, int *i, int quote_type)
// {
// 	while (str[*i] != NULL_TERM)
// 	{
// 		check_if_inside_quotes(str, i, &quote_type);
// 		if (!quote_type)
// 		{
// 			if (str[*i] == O_ROUND)
// 			{
// 				while (str[*i] != C_ROUND)
// 					(*i)++;
// 				if (str[*i] == C_ROUND)
// 				{
// 					str[(*i)++] = NULL_TERM;
// 					return (str);
// 				}
// 			}
// 			(*i)++;
// 		}
// 		else
// 			(*i)++;
// 	}
// 	return (NULL);
// }

static void	modificate_str_utils(char *str, char *r, int *i, int *j)
{
	r[*j] = str[*i];
	str[*i] = SPA;
	(*j)++;
	(*i)++;
}

static void	fill_redir(char *str, char **redir, int *i, int *j)
{
	int	q_flag;

	q_flag = 0;
	while ((str[*i] == REDIR_L || str[*i] == REDIR_R
			|| str[*i] == SPA || str[*i] == HT) && str[*i] != NULL_TERM)
		modificate_str_utils(str, *redir, i, j);
	while (str[*i] != NULL_TERM)
	{
		if ((str[*i] == S_QUO || str[*i] == D_QUO) && q_flag == 0)
			q_flag = str[*i];
		else if (str[*i] == S_QUO && q_flag == S_QUO)
			q_flag = 0;
		else if (str[*i] == D_QUO && q_flag == D_QUO)
			q_flag = 0;
		if (q_flag == 0 && (str[*i] == SPA || str[*i] == REDIR_L
				|| str[*i] == REDIR_R || str[*i] == HT || str[*i] == NULL_TERM))
			break ;
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
		if ((str[i] == S_QUO || str[i] == D_QUO) && q_flag == 0)
			q_flag = str[i];
		else if (str[i] == S_QUO && q_flag == S_QUO)
			q_flag = 0;
		else if (str[i] == D_QUO && q_flag == D_QUO)
			q_flag = 0;
		if (q_flag == 0 && (str[i] == REDIR_L || str[i] == REDIR_R))
			fill_redir(str, redir, &i, &j);
		if (str[i] == NULL_TERM)
			break ;
		i++;
	}
	return (1);
}