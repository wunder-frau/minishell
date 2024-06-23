#include "minishell.h"

void	print_msg(int status);

int	wait_children(pid_t *pids, int num)
{
	int	i;
	int	status;

	i = 0;
	while (i < num)
	{
		if (pids[i] == -1)
			return (FORK_FAILURE);
		waitpid(pids[i], &status, 0);
		i++;
	}
	if (WIFSIGNALED(status))
	{
		print_msg(status);
		return (status + 128);
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (EXIT_FAILURE);
}

void	print_msg(int status)
{
	int	sigcode;

	sigcode = WTERMSIG(status);
	if (sigcode == SIGINT)
		ft_putstr_fd("\n", STDERR_FILENO);
	else if (sigcode == SIGQUIT)
		ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
	else if (sigcode == SIGABRT)
		ft_putstr_fd("Abort trap: 6\n", STDERR_FILENO);
	else if (sigcode == SIGKILL)
		ft_putstr_fd("Killed: 9\n", STDERR_FILENO);
	else if (sigcode == SIGBUS)
		ft_putstr_fd("Bus error: 10\n", STDERR_FILENO);
	else if (sigcode == SIGSEGV)
		ft_putstr_fd("Segmentation fault: 11\n", STDERR_FILENO);
	else if (sigcode == SIGPIPE)
		;
	else
		ft_putstr_fd("Unknown signal\n", STDERR_FILENO);
}
