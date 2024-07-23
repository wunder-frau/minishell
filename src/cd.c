#include "../minishell.h"

void ft_cd(t_minishell *shell, char **argv)
{
    char *home;
    char *oldpwd;
    char *pwd;
    int ret;

    // Check for too many arguments
    if (argv[1] && argv[2])
    {
        ft_putstr_fd("minishell: cd: too many arguments\n", 2);
        shell->exit_status = 1;
        return;
    }

    home = ft_get_env(*(shell->hashmap), "HOME");
    oldpwd = getcwd(NULL, 0);

    if (!oldpwd)
    {
        perror("getcwd error");
        shell->exit_status = 1;
        return;
    }

    if (!argv[1] || strcmp(argv[1], "~") == 0)
    {
        ret = chdir(home);
    }
    else if (strcmp(argv[1], "-") == 0)
    {
        if (shell->oldpwd)
        {
            ret = chdir(shell->oldpwd);
        }
        else
        {
            ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
            free(oldpwd);
            shell->exit_status = 1;
            return;
        }
    }
    else
    {
        ret = chdir(argv[1]);
    }
    if (ret == -1)
    {
        ft_putstr_fd("minishell: cd: ", 2);
        ft_putstr_fd(argv[1], 2);
        ft_putstr_fd(": ", 2);
        ft_putstr_fd(strerror(errno), 2);
        ft_putstr_fd("\n", 2);
        shell->exit_status = 1;
        free(oldpwd);
        return;
    }
    pwd = getcwd(NULL, 0);
    if (!pwd)
    {
        perror("getcwd error after chdir");
        free(oldpwd);
        shell->exit_status = 1;
        return;
    }
    ft_add_env_hash(shell->hashmap, "OLDPWD", oldpwd);
    ft_add_env_hash(shell->hashmap, "PWD", pwd);
    if (shell->oldpwd)
        free(shell->oldpwd);
    shell->oldpwd = oldpwd;
    free(pwd);
    shell->exit_status = 0;
}
