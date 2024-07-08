#include "minishell.h"

void ft_echo(t_minishell *shell, char **cmd)
{
    int     i;
    int     n_flag;

    i = 1;
    // TODO: Implement -n flag
    if (cmd[i] && ft_strcmp(cmd[i], "-n") == 0)
    {
        n_flag = 1;
        i++;
    }
    else
        n_flag = 0;
    while (cmd[i])
    {
        ft_putstr_fd(cmd[i], 1);
        if (cmd[i + 1])
            ft_putstr_fd(" ", shell->fd_output);
        i++;
    }
    if (!n_flag)
        ft_putstr_fd("\n", 1);
    shell->exit_status = 0;
}
