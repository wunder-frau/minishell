#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_minishell	*shell;
	t_cmd_data	cmd_data;

	if (argc < 1 || !argv)
		return (0);
	init_minishell(&shell, envp, &cmd_data);
	run_commandline(&shell);
	free_minishell(shell);
	return(0);
	shell = NULL;
	return(0);
}