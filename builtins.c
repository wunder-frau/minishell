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
	//ft_exit(shell, argv);
		return shell->exit_status;
		exit(shell->exit_status);
	}
		//printf("IN EXEC shell->hashmap PWD w ft_get_env: %s\n", ft_get_env(*(shell->hashmap), "PWD"));
		return (shell->exit_status);
}

