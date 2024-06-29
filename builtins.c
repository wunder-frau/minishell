#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (cmd == NULL)
		return (C_EMPTY);
	if (is_blank_string(cmd))
		return (C_BLANK);
	if (ft_strncmp(cmd, "pwd", PWD_LEN) == 0)
		return (C_PWD);
	return (NOT_BUILTIN);
}

int	execution_builtin(char **cmd, t_minishell *ms, int cmd_type)
{
	if (cmd_type == C_EMPTY || cmd_type == C_BLANK)
	{
		if (*cmd == NULL)
			return ms->exit_status;
		ms->exit_status = CMD_NF_FAILURE;
		printf("command not found\n %s", *cmd);
	}
	else
	{
		ms->exit_status = SUCCESS;
		if (cmd_type == C_PWD)
			run_pwd(cmd + 1, ms);
	}
	int i = 0;

	while (cmd[i] != NULL)
	{
		printf("String at index %d: %s\n", i, cmd[i]);
		i++;
	}
	return ms->exit_status;
}
