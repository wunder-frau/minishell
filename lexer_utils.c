#include "minishell.h"

void	quote_tracker(char *str, int *i, int *quote_status)
{
	char cur_char = str[*i];

	if ((cur_char == S_QUO && *quote_status == S_QUO)
	|| (cur_char == D_QUO && *quote_status == D_QUO))
	{
		printf("kkkkk___%c\n", cur_char);
		printf("inside____________quotes");
		printf("\n");
		*quote_status = 0;
		printf("type : [%c]", *quote_status);
	}
	else if (cur_char == S_QUO || cur_char == D_QUO)
	{
		printf("jojo___%c\n", cur_char);
		printf("\n");
		*quote_status = cur_char;
		printf("TYPE: [%c]", *quote_status);
	}
}

static void	copy_and_replace_with_space(char *str, char *r, int *i, int *j)
{
	r[*j] = str[*i];
	str[*i] = SPA;
	(*j)++;
	(*i)++;
	printf("🔚 Resulting string after removing quotes: [%s]\n", str);
}

static int	is_special_char(char c)
{
	return (c == REDIR_L || c == REDIR_R || c == SPA || c == HT);
}

static void	extract_redirect(char *str, char **redir, int *i, int *j)
{
	int quote_status = 0;

	while (is_special_char(str[*i]) && str[*i] != NULL_TERM)
	{
		printf("TTTTT%c\n", str[*i]);
		copy_and_replace_with_space(str, *redir, i, j);
	}	
	while (str[*i] != NULL_TERM)
	{
		// TUT 1
		printf("HEREWEGO%s\n", str);
		//quote_tracker(str, *i, &quote_status);
		is_inside_quotes(str[*i], &quote_status);
		if (quote_status == 0 && (is_special_char(str[*i]) || str[*i] == NULL_TERM))
			break ;
		copy_and_replace_with_space(str, *redir, i, j);
	}
	(*redir)[(*j)++] = SEPARATOR;
	(*i)--;
	printf("🔚 Resulting string after removing quotes: [%s]\n", str);
}

int	convert_input_to_redirects(char *str, char **redir, int i,
	int j)
{
	int	quote_status;

	quote_status = 0;
	if (!str)
		return (-1);
	*redir = (char *)ft_calloc(ft_strlen(str) * 2, sizeof(char));
	if (!*redir)
		return (-1);
	//("Initial String: %s\n", str);
	while (str[i] != NULL_TERM)
	{
		printf("HEREWEGO______ex__%c\n", str[i]);
		quote_tracker(str, &i, &quote_status);
		if (quote_status == 0 && (str[i] == REDIR_L || str[i] == REDIR_R))
		{
			extract_redirect(str, redir, &i, &j);
			printf("______________________Modified Redirection String: %s\n", *redir);
		}
		if (str[i] == NULL_TERM)
			break ;
		i++;
	}
	printf("Final REDIR___________________: %s\n", *redir);
	printf("Final String___________________: %s\n", str);
	return (1);
}
