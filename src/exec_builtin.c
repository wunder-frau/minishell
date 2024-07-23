#include "../minishell.h"

void	command_not_found_error(t_minishell *shell)
{
	if (shell->command[0] == NULL)
		return ;
	shell->exit_status = CMD_NF_FAILURE;
	print_err_msg(shell->command[0], ": command not found\n");
}

bool	is_builtin(char *cmd)
{
	if (cmd == NULL)
		return (true);
	if (is_blank_string(cmd))
		return (true);
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
	if (shell->command[0] == NULL || is_blank_string(shell->command[0]))
		command_not_found_error(shell);
	else if (ft_strncmp(shell->command[0], "echo", 5) == 0)
		ft_echo(shell, &shell->command[0]);
	else
	{
		shell->exit_status = SUCCESS;
		if (ft_strncmp(shell->command[0], "cd", 3) == 0)
			ft_cd(shell, &shell->command[0]);
		else if (ft_strncmp(shell->command[0], "pwd", 4) == 0)
			ft_pwd(shell);
		else if (ft_strncmp(shell->command[0], "export", 7) == 0)
			ft_export(shell, &shell->command[0]);
		else if (ft_strncmp(shell->command[0], "unset", 6) == 0)
			ft_unset(shell, &shell->command[0]);
		else if (ft_strncmp(shell->command[0], "env", 4) == 0)
			ft_env(shell);
		else if (ft_strncmp(shell->command[0], "exit", 5) == 0)
			ft_exit(&shell->command[1], shell);
	}
	return (shell->exit_status);
}
