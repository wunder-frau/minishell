#include "minishell.h"

void	init_minishell(t_minishell **ms, char **envp, t_cmd_data *cmd_data)
{
	//extern char	**environ;
	int			status;

	status = 0;

	*ms = ft_calloc(1, sizeof(t_minishell));
	if (!*ms)
		status = 200;
	if (status == 0)
	{
		//(*ms)->env = dup_envp(environ);
		cmd_data->hashmap = init_hmap(envp);
		if (!cmd_data->hashmap)
			status = 200;
	}
	(*ms)->hashmap = cmd_data->hashmap;
	(*ms)->cmd_data = *cmd_data;
	add_shlvl(*ms);
		// if (status == 0)
		// status = shlvl_init(&((*ms)->env));
	if (status != 0)
	{
		terminate_minishell(ms, status);
	}
}

void	terminate_minishell(t_minishell **ms, int status)
{
	free_minishell(*ms);
	ft_putstr_fd("Error: Failed to initialize t_minishell"
		"structure\n", STDERR_FILENO);
	exit (status);
}