#include "minishell.h"

// TO DO add `ft_strchr`, `ft_strcmp`, `ft_arrlen`, `free` to libft_
char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return((char *)s);
		s++;
	}
	return(NULL);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while(*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return(*s1 - *s2);
}

size_t	ft_arrlen(void **arr)
{
	size_t	length;

	length = 0;
	while (arr[length] != NULL)
		length++;
	return (length);
}

void	ft_free(void *ptr)
{
	void	**arr;
	size_t	i;

	arr = (void **)ptr;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

char	**env_c(char **envp)
{
	char	**env;
	int		len;
	int		i = 0;

	len = ft_arrlen((void **)envp);
	env = ft_calloc(len + 1, sizeof(char *));
	if (!env)
		return (NULL);
	while (i < len)
	{
		env[i] = ft_strdup(envp[i]);
		if (!env[i])
		{
			ft_free((void *)env);
			return (NULL);
		}
		i++;
	}
	i = 0;

	while(i < len && env[i] != NULL)
	{
		printf("%s\n", env[i]);
		i++;
	}
	return (env);
}

void ft_readline(char **cmdline, char *prompt)
{
	*cmdline = readline(prompt);
}

void init_minishell(t_minishell *shell)
{
	extern char	**environ;
	int			status;

	status = 0;
	shell = ft_calloc(1, sizeof(t_minishell));
	// if(!shell)
	// 	return (0);
	shell->env = env_c(environ);
	shell->oldpwd = NULL;
	shell->history = NULL;
	shell->history_path = NULL;
	shell->exit_status = 0;
	shell->root = NULL;
	shell->is_parent = 1;
	shell->is_oldpwd_unset = 1;

	char *cmdline = NULL;

	// !! input check
	while (1)
	{
		char *cmdline = NULL;
		ft_readline(&cmdline, "minishell> ");
		if (cmdline)
		{
			if (ft_strcmp(cmdline, "exit") == 0)
			{
				free(cmdline);
				break;
			}
			add_history(cmdline);
			printf("You entered: %s\n", cmdline);
			free(cmdline);
		}
    }
    //.....
    if (cmdline)
	{
		add_history(cmdline);
		free(cmdline);
    }
}


int main(void)
{
	t_minishell shell;

	init_minishell(&shell);
	return 0;
}
