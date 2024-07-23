#include "../minishell.h"
#include <stdio.h> // For debug print

void ft_echo(t_minishell *shell, char **cmd)
{
    int i;
    char *temp;
    int n_flag;

    i = 1;
    if (cmd[i] && ft_strcmp(cmd[i], "-n") == 0)
    {
        n_flag = 1;
        i++;
    }
    else
        n_flag = 0;
    (void)shell;
    while (cmd[i])
    {
        temp = ft_strdup(cmd[i]);
        ft_putstr_fd(temp, 1);
        free(temp);

        if (cmd[i + 1])
            ft_putstr_fd(" ", 1);
        i++;
    }
    if (!n_flag)
        ft_putstr_fd("\n", 1);
}