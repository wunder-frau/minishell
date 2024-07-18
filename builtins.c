#include "minishell.h"

bool is_builtin(char *cmd)
{
	if (!cmd)
		return (false);
	if (is_blank_string(cmd))
		return (false);
	if (ft_strncmp(cmd, "echo", 5) == 0
	|| ft_strncmp(cmd, "cd", 3) == 0
	|| ft_strncmp(cmd, "pwd", 4) == 0
	|| ft_strncmp(cmd, "export", 7) == 0
	|| ft_strncmp(cmd, "unset", 6) == 0
	|| ft_strncmp(cmd, "env", 4) == 0
	|| ft_strncmp(cmd, "exit", 5) == 0)
		return (true);
	return (false);
}

int	ft_is_number(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	ft_exit(t_minishell *shell)
{
	if (shell->command[1])
	{
		if (ft_is_number(shell->command[1]))
			shell->exit_status = ft_atoi(shell->command[1]);
		else
		{
			shell->exit_status = 255;
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(shell->command[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
		}
	}
	else
		shell->exit_status = 0;
}

int	exec_builtin(t_minishell *shell)
{
	if (ft_strncmp(shell->command[0], "echo", 5) == 0)
		ft_echo(shell, &shell->command[0]);
	else if (ft_strncmp(shell->command[0], "cd", 3) == 0)
		ft_cd(shell, &shell->command[0]);
	if (ft_strncmp(shell->command[0], "pwd", 4) == 0)
		ft_pwd(shell);
	else if (ft_strncmp(shell->command[0], "export", 7) == 0)
		ft_export(shell, &shell->command[0]);
	else if (ft_strncmp(shell->command[0], "unset", 6) == 0)
		ft_unset(shell, &shell->command[0]);
	else if (ft_strncmp(shell->command[0], "env", 4) == 0)
		ft_env(shell);
	else if (ft_strncmp(shell->command[0], "exit", 5) == 0)
	{
		ft_exit(shell);
		return shell->exit_status;
		exit(shell->exit_status);
	}
		printf("IN EXEC shell->hashmap PWD w ft_get_env: %s\n", ft_get_env(*(shell->hashmap), "PWD"));
		return (shell->exit_status);
}

