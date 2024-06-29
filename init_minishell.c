#include "minishell.h"

void	init_minishell(t_minishell **ms)
{
	extern char	**environ;
	int			status;

	status = 0;
	*ms = ft_calloc(1, sizeof(t_minishell));
	if (!*ms)
		status = 200;
	if (status == 0)
	{
		(*ms)->env = dup_envp(environ);
		if (!(*ms)->env)
			status = 200;
	}
		// if (status == 0)
		// status = shlvl_init(&((*ms)->env));
	if (status != 0)
		terminate_minishell(ms, status);
}

void	terminate_minishell(t_minishell **ms, int status)
{
	free_minishell(*ms);
	ft_putstr_fd("Error: Failed to initialize t_minishell"
		"structure\n", STDERR_FILENO);
	exit (status);
}