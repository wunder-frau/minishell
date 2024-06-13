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
}

void ft_cd(t_minishell *shell, char **argv)
{
	char	*home;
	char	*oldpwd;
	char	*pwd;
	int		ret;

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
		//ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		shell->exit_status = 1;
	}
	else
	{
		pwd = getcwd(NULL, 0);
		shell->env = ft_add_env(shell->env, "OLDPWD", oldpwd);
		shell->env = ft_add_env(shell->env, "PWD", pwd);
		free(pwd);
		free(oldpwd);
		shell->exit_status = 0;
	}
}*/

int	is_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "echo\0", 5) == 0)
		return (1);
	else if (ft_strncmp(cmd, "pwd\0", 4) == 0)
		return (1);
	else if (ft_strncmp(cmd, "env\0", 4) == 0)
		return (1);
	else if (ft_strncmp(cmd, "exit\0", 5) == 0)
	{
		printf("da\n");
		return (1);
	}
	/*else if (ft_strncmp(cmd, "cd\0", 3) == 0)
		return (1);
	else if (ft_strncmp(cmd, "export\0", 7) == 0)
		return (1);
	else if (ft_strncmp(cmd, "unset\0", 6) == 0)
		return (1);*/
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
	/*else if (ft_strncmp(argv[0], "cd\0", 3) == 0)
		ft_cd(shell, argv);
	else if (ft_strncmp(argv[0], "export\0", 7) == 0)
		ft_export(shell, argv);
	else if (ft_strncmp(argv[0], "unset\0", 6) == 0)
		ft_unset(shell, argv);*/
}