#include "../minishell.h"
#include <stdio.h> // For debug print

bool is_valid_identifier(const char *str)
{
    if (!str || !str[0])
        return false;

    if (!ft_isalpha(str[0]) && str[0] != '_')
        return false;

    for (int i = 1; str[i]; i++)
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return false;
    }

    return true;
}

void ft_export(t_minishell *shell, char **argv)
{
    int i;
    int j;
    char *key;
    char *value;

    i = 1;
    while (argv[i])
    {
        j = 0;
        while (argv[i][j] && argv[i][j] != '=')
            j++;
        key = ft_substr(argv[i], 0, j);
        if (!is_valid_identifier(key))
        {
            print_arg_err_msg("export: '", argv[i], "' : not a valid identifier\n");
            free(key);
            shell->exit_status = 1;
            i++;
            continue;
        }
        if (argv[i][j] == '=')
            value = ft_strdup(argv[i] + j + 1);
        else
            value = NULL;
        ft_add_env_hash(shell->hashmap, key, value ? value : "");
        free(key);
        free(value);
        i++;
    }
    // shell->exit_status = 0;
}

void ft_unset(t_minishell *shell, char **argv)
{
    int i;

    i = 1;
    while (argv[i])
    {
        if (is_valid_identifier(argv[i]))
        {
            ft_remove_env_hash(shell->hashmap, argv[i]);
        }
        else
        {
            print_arg_err_msg("unset: '", argv[i], "' : not a valid identifier\n");
            shell->exit_status = 1;
        }
        i++;
    }
    shell->exit_status = 0;
}