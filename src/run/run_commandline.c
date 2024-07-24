#include "../minishell.h"

void ft_readline(char **cmdline, char *prompt)
{
    *cmdline = readline(prompt);
}

void run_commandline(t_minishell **shell)
{
    int status;
    char *cmdline = NULL;
    int hd_c;

    while (1)
    {
        ft_readline(&cmdline, "minishell> ");
        if (ctrl_d_handler(cmdline) == 0)
        {
            free(cmdline);
            break ;
        }
        if (is_blank_string(cmdline))
        {
            free(cmdline);
            continue ;
        }
        if (cmdline[0] != '\0' && cmdline[0] != '\n')
            add_history(cmdline);
        // Perform dollar expansion here;
        dollar_expansion(&cmdline, *shell, (*shell)->exit_status);
        status = syntax_checker_expression(cmdline);
        if (status != SUCCESS)
        {
            free(cmdline);
            continue ;
        }
        hd_c = 0;
        (*shell)->root = NULL;
		// TUT LOMAYETSIA $EMPTY
        status = build_ast(cmdline, &((*shell)->root), &hd_c, *shell);
        if (status != 0 && cmdline[0] != '\0' && cmdline[0] != '\n')
            (*shell)->exit_status = status;
        if (status == 0)
            (*shell)->exit_status = traverse_tree(&((*shell)->root), *shell);
        free(cmdline);
        cmdline = NULL;
    }
}
