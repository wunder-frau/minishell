#include "../minishell.h"
#include <stdio.h> // For debug print

void ft_echo(t_minishell *shell, char **cmd)
{
    int i;
    char *temp;

    i = 1;
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
    ft_putstr_fd("\n", 1);
}