#include "minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while(*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return(*s1 - *s2);
}

void ft_readline(char **cmdline, char *prompt)
{
	*cmdline = readline(prompt);
}

void	run_commandline(t_minishell **ms)
{
    char    *cmdline;
    int     hd;
    //(void)**ms;

     while (1)
    {
        ft_readline(&cmdline, "minishell> ");
        if (cmdline)
        {
            if (ft_strcmp(cmdline, "exit") == 0)
            {
                free(cmdline);
                break;
            }
            add_history(cmdline);
            hd = 0;
            (*ms)->root = NULL;
            create_tree(cmdline, &((*ms)->root), &hd, *ms);
            free(cmdline);
        }
    }
    if (cmdline)
    {
        add_history(cmdline);
        free(cmdline);
    }
}