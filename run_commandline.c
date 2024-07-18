#include "minishell.h"

void	ft_readline(char **cmdline, char *prompt)
{
	*cmdline = readline(prompt);
}

void	run_commandline(t_minishell **ms)
{
	int		status;
	char	*cmdline = NULL;
	int		hd;

	while (1)
	{
		ft_readline(&cmdline, "minishell> ");
		if (ctrl_d_handler(cmdline) == 0)
		{
			free(cmdline);
			break;
		}
		// if (cmdline == NULL)
		// 	continue;
		if (is_blank_string(cmdline))
		{
			free(cmdline);
			continue;
		}
		// if (ft_strcmp(cmdline, "exit") == 0)
		// {
		// 	free(cmdline);
		// 	break;
		// }
		if (cmdline[0] != '\0' && cmdline[0] != '\n')
			add_history(cmdline);
		status = validate_input(cmdline);
		if (status != SUCCESS)
		{
			free(cmdline);
			continue;
		}
		hd = 0;
		(*ms)->root = NULL;
		printf("Debug: Creating command tree\n");
		status = build_ast(cmdline, &((*ms)->root), &hd, *ms);
		if (status != 0)
			(*ms)->exit_status = status;
		printf("Debug: Traversing command tree\n");
		if (status == 0)
			(*ms)->exit_status = traverse_tree(&((*ms)->root), *ms);
		//printf("Debug: Command executed, freeing cmdline\n");
		free(cmdline);
		cmdline = NULL;
	}
}
