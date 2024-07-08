#include "minishell.h"

int	dollar_expansion(char **str, t_minishell *ms, int last_status)
{
	char	*value;
	char	*key;
	int		status;

	status = 0;
	if ((*str)[0] == '$')
	{
        if ((*str)[1] == '?')
        {
            free(*str);
            *str = ft_itoa(last_status);
            if (!*str)
                status = MALLOC_ERR;
            return (status);
        }
		key = *str + 1;
		value = ft_get_env(*(ms->hashmap), key);
		if (value)
		{
			free(*str);
			*str = ft_strdup(value);
			if (!*str)
				status = MALLOC_ERR;
		}
        else
            value = "$";
	}
	return (status);
}