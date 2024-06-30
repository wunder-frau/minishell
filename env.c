#include "minishell.h"

void ft_env(t_minishell *shell)
{
    t_hmap *current;

    current = *(shell->hashmap);
    while (current)
    {
        ft_putstr_fd(current->key, shell->fd_output);
        ft_putstr_fd("=", shell->fd_output);
        ft_putstr_fd(current->value, shell->fd_output);
        ft_putstr_fd("\n", shell->fd_output);
        current = current->next;
    }
    shell->exit_status = 0;
}