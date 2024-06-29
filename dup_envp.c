#include "minishell.h"

char	**dup_envp(char **envp)
{
	char	**env;
	int		len;
	int		i = 0;

	len = ft_size((void **)envp);
	env = ft_calloc(len + 1, sizeof(char *));
	if (!env)
		return (NULL);
	while (i < len)
	{
		env[i] = ft_strdup(envp[i]);
		if (!env[i])
		{
			free_arr_2d((void *)env);
			return (NULL);
		}
		i++;
	}
	i = 0;
// !
	// while(i < len && env[i] != NULL)
	// {
	// 	printf("%s\n", env[i]);
	// 	i++;
	// }
	return (env);
}
