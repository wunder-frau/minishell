#include "minishell.h"

int	main(void)
{
	t_minishell	*shell;
	init_minishell(&shell);
	run_commandline(&shell);
	free_minishell(shell);
	return(0);
	shell = NULL;
	return(0);
}