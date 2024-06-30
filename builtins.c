#include "minishell.h"

// int	is_builtin(char *cmd)
// {
// 	if (cmd == NULL)
// 		return (C_EMPTY);
// 	if (is_blank_string(cmd))
// 		return (C_BLANK);
// 	if (ft_strncmp(cmd, "pwd", PWD_LEN) == 0)
// 		return (C_PWD);
// 	return (NOT_BUILTIN);
// }

int is_builtin(char *cmd)
{
if (!cmd)
return (0);
if (is_blank_string(cmd))
	return (C_BLANK);
if (ft_strncmp(cmd, "echo", 5) == 0
|| ft_strncmp(cmd, "cd", 3) == 0
|| ft_strncmp(cmd, "pwd", 4) == 0
|| ft_strncmp(cmd, "export", 7) == 0
|| ft_strncmp(cmd, "unset", 6) == 0
|| ft_strncmp(cmd, "env", 4) == 0
|| ft_strncmp(cmd, "exit", 5) == 0)
	return (1);
return (0);
}

int	exec_builtin(t_minishell *shell)
{
// if (ft_strncmp(argv[0], "echo", 5) == 0)
// ft_echo(shell, argv);
// else if (ft_strncmp(argv[0], "cd", 3) == 0)
// ft_cd(shell, argv);
if (ft_strncmp(shell->command[0], "pwd", 4) == 0)
{
ft_pwd(shell);
}
// else if (ft_strncmp(argv[0], "export", 7) == 0)
// ft_export(shell, argv);
// else if (ft_strncmp(argv[0], "unset", 6) == 0)
// ft_unset(shell, argv);
// else if (ft_strncmp(argv[0], "env", 4) == 0)
// ft_env(shell);
else if (ft_strncmp(shell->command[0], "exit", 5) == 0)
{
//ft_exit(shell, argv);
	return shell->exit_status;
	exit(shell->exit_status);
}
	printf("IN EXEC shell->hashmap PWD w ft_get_env: %s\n", ft_get_env(*(shell->hashmap), "PWD"));
	return shell->exit_status;
}

// int	exec_builtin(char **cmd, t_minishell *ms, int cmd_type)
// {
// 	if (cmd_type == C_EMPTY || cmd_type == C_BLANK)
// 	{
// 		if (*cmd == NULL)
// 			return ms->exit_status;
// 		ms->exit_status = CMD_NF_FAILURE;
// 		printf("command not found\n %s", *cmd);
// 	}
// 	else
// 	{
// 		ms->exit_status = SUCCESS;
// 		// if (cmd_type == C_CD)
// 		// 	run_cd(cmd, ms, 0);
// 		if (cmd_type == C_PWD)
// 			run_pwd(cmd, ms);
// 	}
// 	int i = 0;

// 	while (cmd[i] != NULL)
// 	{
// 		printf("String at index %d: %s\n", i, cmd[i]);
// 		i++;
// 	}
// 	return ms->exit_status;
// }
