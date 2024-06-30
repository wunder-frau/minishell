#include "minishell.h"

void ft_export(t_minishell *shell, char **argv)
{
    int i, j;
    char *key;
    char *value;

    i = 1;
    while (argv[i])
    {
        j = 0;
        while (argv[i][j] && argv[i][j] != '=')
            j++;
        key = ft_substr(argv[i], 0, j);
        if (argv[i][j] == '=')
            value = ft_strdup(argv[i] + j + 1);
        else
            value = NULL;
        ft_add_env_hash(shell->hashmap, key, value ? value : "");
        free(key);
        free(value);
        i++;
    }
    shell->exit_status = 0;
}
void ft_unset(t_minishell *shell, char **argv)
{
    int i;

    i = 1;
    while (argv[i])
    {
        ft_remove_env_hash(shell->hashmap, argv[i]);
        i++;
    }
    shell->exit_status = 0;
}