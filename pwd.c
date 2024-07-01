#include "minishell.h"

char *ft_get_env(t_hmap *hashmap, const char *key)
{
    while (hashmap)
    {
        if (ft_strcmp(hashmap->key, key) == 0)
            return hashmap->value;
        hashmap = hashmap->next;
    }
    return NULL;
}

void ft_pwd(t_minishell *shell)
{
    char *pwd;
    pwd = ft_get_env(*(shell->hashmap), "PWD");
    if (pwd)
    {
        ft_putstr_fd(pwd, 1);
        ft_putstr_fd("\n", 1);
    }
    else
    {
        ft_putstr_fd("minishell: pwd: PWD not set\n", 2);
        shell->exit_status = 1;
    }
}
