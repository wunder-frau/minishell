#include "minishell.h"

void ft_readline(char **cmdline, char *prompt)
{
    *cmdline = readline(prompt);
}

void kill_child_processes(t_minishell *ms)
{
    if (g_sigint_received == 1)
    {
        // Loop through all child processes and kill them
        // This assumes you have a way to track child PIDs
        for (int i = 0; i < ms->num_children; i++)
        {
            if (ms->children_pids[i] != -1)
            {
                kill(ms->children_pids[i], SIGKILL);
            }
        }
        g_sigint_received = 0; // Reset the signal flag
    }
}

void run_commandline(t_minishell **ms)
{
    int status;
    char *cmdline = NULL;
    int hd;

    while (1)
    {
        ft_readline(&cmdline, "minishell> ");
        
        if (g_sigint_received == 1)
        {
            kill_child_processes(*ms);
            printf("ASSAS\n");
            if (cmdline)
            {
                free(cmdline);
                cmdline = NULL;
            }
            continue;
        }

        if (ctrl_d_handler(cmdline) == 0)
        {
            free(cmdline);
            break;
        }

        if (is_blank_string(cmdline))
        {
            free(cmdline);
            continue;
        }

        if (ft_strcmp(cmdline, "exit") == 0)
        {
            free(cmdline);
            break;
        }

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
        status = build_ast(cmdline, &((*ms)->root), &hd, *ms);
        if (status != 0)
            (*ms)->exit_status = status;
        if (status == 0)
            (*ms)->exit_status = traverse_tree(&((*ms)->root), *ms);
        free(cmdline);
        cmdline = NULL;
    }
}
