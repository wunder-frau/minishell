#include "minishell.h"

void	ft_readline(char **cmdline, char *prompt)
{
	*cmdline = readline(prompt);
}

void	run_commandline(t_minishell **ms)
{
	char *cmdline = NULL;
	int hd;

	while (1) {
		ft_readline(&cmdline, "minishell> ");

		if (cmdline == NULL)
			continue;
		if (ft_strlen(cmdline) == 0)
		{
			free(cmdline);
			continue;
		}
		if (ft_strcmp(cmdline, "exit") == 0)
		{
			free(cmdline);
			break;
		}
		add_history(cmdline);
		hd = 0;
		(*ms)->root = NULL;
		printf("Debug: Creating command tree\n");
		build_ast(cmdline, &((*ms)->root), &hd, *ms);
		printf("Debug: Traversing command tree\n");
		traverse_tree(&((*ms)->root), *ms);
		printf("Debug: Command executed, freeing cmdline\n");
		free(cmdline);
		cmdline = NULL;
	}
}
