#include "../minishell.h"
#include <errno.h>

void ft_add_env_hash(t_hmap **hashmap, char *key, char *value)
{
	if (!hashmap || !key)
		return;

	t_hmap *current = *hashmap;
	while (current)
	{
		if (strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return;
		}
		current = current->next;
	}
	add_new_var(hashmap, key, value);
}

void ft_remove_env_hash(t_hmap **hashmap, char *key)
{
	if (!hashmap || !key)
		return;

	t_hmap *current = *hashmap;
	t_hmap *prev = NULL;

	while (current)
	{
		if (strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*hashmap = current->next;

			free(current->key);
			free(current->value);
			free(current);
			return;
		}
		prev = current;
		current = current->next;
	}
}

char *ft_get_env(t_hmap *hashmap, const char *key)
{
	while (hashmap)
	{
		if (strcmp(hashmap->key, key) == 0)
			return hashmap->value;
		hashmap = hashmap->next;
	}
	return NULL;
}

void ft_echo(t_minishell *shell, char **argv)
{
    int i;
    char *value;
    
    i = 1;
    while (argv[i])
    {
        if (argv[i][0] == '$')
        {
            value = ft_get_env(*(shell->hashmap), argv[i] + 1);  // Skip the '$' and get the variable name
            if (value)
                ft_putstr_fd(value, 1);
        }
        else
        {
            ft_putstr_fd(argv[i], 1);
        }
        if (argv[i + 1])
            ft_putstr_fd(" ", shell->fd_output);
        i++;
    }
    ft_putstr_fd("\n", 1);
    shell->exit_status = 0;
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

void ft_exit(t_minishell *shell, char **argv)
{
	int i;

	i = 0;
	while (argv[i])
		i++;
	if (i == 1)
		shell->exit_status = 0;
	else if (i == 2)
		shell->exit_status = ft_atoi(argv[1]);
	else
	{
		ft_putstr_fd("exit\n", 1);
		ft_putstr_fd("minishell: exit: too many arguments\n", 1);
		shell->exit_status = 1;
	}
}

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

void ft_free_array(char **array)
{
	int i;

	i = 0;
	if (!array)
		return;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

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

void ft_cd(t_minishell *shell, char **argv)
{
	char *home;
	char *oldpwd;
	char *pwd;
	int ret;

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
int is_builtin(char *data)
{
	if (!data)
		return (0);
	if (ft_strncmp(data, "echo", 5) == 0
		|| ft_strncmp(data, "cd", 3) == 0
		|| ft_strncmp(data, "pwd", 4) == 0
		|| ft_strncmp(data, "export", 7) == 0
		|| ft_strncmp(data, "unset", 6) == 0
		|| ft_strncmp(data, "env", 4) == 0
		|| ft_strncmp(data, "exit", 5) == 0)
		return (1);
	return (0);
}

void exec_builtin(t_minishell *shell, char **argv)
{
	if (ft_strncmp(argv[0], "echo", 5) == 0)
		ft_echo(shell, argv);
	else if (ft_strncmp(argv[0], "cd", 3) == 0)
		ft_cd(shell, argv);
	else if (ft_strncmp(argv[0], "pwd", 4) == 0)
		ft_pwd(shell);
	else if (ft_strncmp(argv[0], "export", 7) == 0)
		ft_export(shell, argv);
	else if (ft_strncmp(argv[0], "unset", 6) == 0)
		ft_unset(shell, argv);
	else if (ft_strncmp(argv[0], "env", 4) == 0)
		ft_env(shell);
	else if (ft_strncmp(argv[0], "exit", 5) == 0)
	{
		ft_exit(shell, argv);
		exit(shell->exit_status);
	}
	printf("IN EXEC shell->hashmap PWD w ft_get_env: %s\n", ft_get_env(*(shell->hashmap), "PWD"));
}
