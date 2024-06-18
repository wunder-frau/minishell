#include "../minishell.h"

void	ft_echo(t_minishell *shell, char **argv)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		ft_putstr_fd(argv[i], 1);
		if (argv[i + 1])
			ft_putstr_fd(" ", shell->fd_output);
		i++;
	}
	ft_putstr_fd("\n", 1);
	shell->exit_status = 0;
}

void	ft_pwd(t_minishell *shell, char **argv)
{
	char	*pwd;

	(void)argv;
	pwd = getcwd(NULL, 0);
	if (!pwd)
		pwd = shell->pwd;
	ft_putstr_fd("PIZDA\n", shell->fd_output);
	ft_putstr_fd(pwd, shell->fd_output);
	ft_putstr_fd("\n", shell->fd_output);
	if (pwd)
		free(pwd);
	shell->exit_status = 0;
}
void	ft_exit(t_minishell *shell, char **argv)
{
	int	i;

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
void	ft_env(t_minishell *shell, char **argv)
{
	int	i;

	i = 0;
	(void)argv;
	while (shell->env[i])
	{
		ft_putstr_fd(shell->env[i], shell->fd_output);
		ft_putstr_fd("\n", shell->fd_output);
		i++;
	}
	shell->exit_status = 0;
}

void	ft_free_array(char **array)
{
	int	i;

	//i = 0;
	//while (array[i++])
	//	printf("array[%i] = %s,\n",i,  array[i]);
	if (!array)
		return ;
	i = 0;
    while (array[i])
    {
        printf("Freeing element %d: %p\n", i, (void *)array[i]);
        //free(array[i]);
        i++;
    }
    printf("Freeing array pointer: %p\n", (void *)array);
    //free(array);
}
/*
void	ft_export(t_minishell *shell, char **argv)
{
	int		i;
	int		j;
	char	*key;
	char	*value;

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
		if (value)
			shell->env = ft_add_env(shell->env, key, value);
		else
			shell->env = ft_add_env(shell->env, key, "");
		free(key);
		free(value);
		i++;
	}
	shell->exit_status = 0;
}

void	ft_unset(t_minishell *shell, char **argv)
{
	int		i;
	int		j;
	char	*key;

	i = 1;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j] && argv[i][j] != '=')
			j++;
		key = ft_substr(argv[i], 0, j);
		shell->env = ft_remove_env(shell->env, key);
		free(key);
		i++;
	}
	shell->exit_status = 0;
}*/
char **ft_add_env(char **env, char *key, char *value)
{
	int		i;
	char	**new_env;
	char	*new_var;

	i = 0;
	while (env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		if (!new_env[i]) 
		{
			ft_free_array(new_env);
			return (NULL);
		}
		i++;
	}
	new_var = ft_strjoin(key, "=");
	if (!new_var)
	{
		ft_free_array(new_env);
		return (NULL);
	}
	new_env[i] = ft_strjoin(new_var, value);
	free(new_var);
	if (!new_env[i])
	{
		ft_free_array(new_env);
		return (NULL);
	}
	new_env[i + 1] = NULL;
	ft_free_array(env);
	return (new_env);
}

char *ft_get_env(char **env, char *key)
{
	int	i;
	int	j;
	int	len;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		len = j;
		if (ft_strncmp(env[i], key, len) == 0)
			return (ft_strdup(env[i] + len + 1));
		i++;
	}
	return (NULL);
}
void	ft_cd(t_minishell *shell, char **argv)
{
	char	*home;
	char	*oldpwd;
	char	*pwd;
	int		ret;


	printf("SIDIIIIIIIIIIIIII\n");
	home = ft_get_env(shell->env, "HOME");
	oldpwd = getcwd(NULL, 0);
	if (!argv[1] || ft_strncmp(argv[1], "~\0", 2) == 0)
		ret = chdir(home);
	else if (ft_strncmp(argv[1], "-\0", 2) == 0)
		ret = chdir(shell->oldpwd);
	else
		ret = chdir(argv[1]);
	if (ret == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd("strerror", 2);
		ft_putstr_fd("\n", 2);
		shell->exit_status = 1;
	}
	else
	{
		pwd = getcwd(NULL, 0);
		// ZACHEM
		shell->env = ft_add_env(shell->env, "OLDPWD", oldpwd);
		shell->env = ft_add_env(shell->env, "PWD", pwd);
		free(pwd);
		free(oldpwd);
		shell->exit_status = 0;
	}
}


int	is_builtin(char *data)
{
	if (!data)
		return (0);
	if (ft_strncmp(data, "echo\0", 5) == 0
		|| ft_strncmp(data, "cd\0", 3) == 0
		|| ft_strncmp(data, "pwd\0", 4) == 0
		|| ft_strncmp(data, "export\0", 7) == 0
		|| ft_strncmp(data, "unset\0", 6) == 0
		|| ft_strncmp(data, "env\0", 4) == 0
		|| ft_strncmp(data, "exit\0", 5) == 0)
		return (1);
	return (0);
}

void	exec_builtin(t_minishell *shell, char **argv)
{
	if (ft_strncmp(argv[0], "echo\0", 5) == 0)
		ft_echo(shell, argv);
	else if (ft_strncmp(argv[0], "pwd\0", 4) == 0)
		ft_pwd(shell, argv);
	else if (ft_strncmp(argv[0], "env\0", 4) == 0)
		ft_env(shell, argv);
	else if (ft_strncmp(argv[0], "exit\0", 5) == 0)
	{
		ft_exit(shell, argv);
		exit(shell->exit_status);
	}
	else if (ft_strncmp(argv[0], "cd\0", 3) == 0)
		ft_cd(shell, argv);
	/*else if (ft_strncmp(argv[0], "export\0", 7) == 0)
		ft_export(shell, argv);
	else if (ft_strncmp(argv[0], "unset\0", 6) == 0)
		ft_unset(shell, argv);*/
}