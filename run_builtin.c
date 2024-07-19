#include "minishell.h"

int	run_builtin(char **redir, t_minishell *ms)
{
	int	status;

	status = 0;
	if (!redir)
		status = exec_builtin(ms);
	else
		status = exec_builtin_redir(ms->command, redir, ms);
	return (status);
}

int	exec_builtin_redir(char **command, char **redir, t_minishell *ms)
{
	int	status;
	int	in_fd;
	int	out_fd;

	status = copy_std_fd(&in_fd, &out_fd, command[0]);
	if (status == 0)
		status = apply_redirects(redir, ms);
	if (status == 0)
		status = exec_builtin(ms);
	return_std_fd(&in_fd, &out_fd, &status, command[0]);
	return (status);
}

int copy_std_fd(int *in_fd, int *out_fd, char *command)
{
	// Save current standard input and output file descriptors
	*in_fd = dup(STDIN_FILENO);
	*out_fd = dup(STDOUT_FILENO);

	// Check if dup operation was successful
	if (*in_fd == -1 || *out_fd == -1)
	{
			// Handle error and close file descriptors if one of the dups failed
			if (*in_fd != -1)
					close(*in_fd);
			if (*out_fd != -1)
					close(*out_fd);
			print_err_msg(command, ": dup() error occurred\n");
			return (DUP_FAILURE);
	}

	// Debug print statements
	//printf("Debug: Copied STDIN_FILENO to in_fd: %d\n", *in_fd);
	//printf("Debug: Copied STDOUT_FILENO to out_fd: %d\n", *out_fd);

	// Return success status
	return (0);
}

void	return_std_fd(int *in_fd, int *out_fd, int *status, char *command)
{
	int	redir_status;

	redir_status = 0;
	if (dup2(*in_fd, STDIN_FILENO) == -1 || dup2(*out_fd, STDOUT_FILENO) == -1)
	{
		print_err_msg(command, ": dup2() error occured. "
			"Correct behavior is not guaranteed anymore\n");
		redir_status = DUP_FAILURE;
	}
	close(*in_fd);
	close(*out_fd);
	if (*status == 0)
		*status = redir_status;
}