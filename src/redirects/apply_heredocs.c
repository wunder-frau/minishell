/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_heredocs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 00:16:40 by istasheu          #+#    #+#             */
/*   Updated: 2024/07/24 15:06:07 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_heredoc_line(int fd, char *line, t_minishell *shell,
				char *limiter)
{
	int	status;

	status = dollar_expansion(&line, shell, shell->exit_status);
	if (status == 0)
	{
		ft_putendl_fd(line, fd);
		free(line);
	}
	else
	{
		ft_putstr_fd("shell: \033[0;0m", STDERR_FILENO);
		ft_putstr_fd(limiter, STDERR_FILENO);
		ft_putendl_fd(": malloc() error occured", STDERR_FILENO);
		close(fd);
		exit(status);
	}
}

static void	read_until_limiter(char *limiter, int fd, t_minishell *shell)
{
	char	*line;
	int		isequal;

	while (true)
	{
		line = readline("> ");
		if (!line)
		{
			close(fd);
			ft_printf("\033[1A");
			ft_printf("\033[2C");
			exit(0);
		}
		isequal = ft_strcmp(limiter, line);
		if (isequal != 0)
			handle_heredoc_line(fd, line, shell, limiter);
		if (isequal == 0)
		{
			close(fd);
			exit(0);
		}
	}
}

void	remove_spaces_and_quotes_hd(char *hd)
{
	int	i;

	while (ft_is_space(*hd))
	{
		i = 0;
		while (hd[i] != NULL_TERM)
		{
			hd[i] = hd[i + 1];
			i++;
		}
	}
	remove_quotes(hd, 0, 0);
}

int	prepare_heredoc(char **limiter, char *hd_name, t_minishell *shell)
{
	int		status;
	int		fd;
	pid_t	pid;

	remove_spaces_and_quotes_hd(*limiter + 2);
	pid = fork();
	if (pid == -1)
		return (FORK_FAILURE);
	if (pid == 0)
	{
		signal_interceptor(2);
		set_signals(0);
		fd = open(hd_name + 2, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd != -1)
			read_until_limiter(*limiter + 2, fd, shell);
		exit(GENERIC_ERROR);
	}
	free(*limiter);
	*limiter = hd_name;
	status = wait_children_and_fetch_exit_status(&pid, 1);
	return (status);
}

int	apply_heredoc(char *heredoc, int *in)
{
	int		status;
	int		fd;

	status = 0;
	if (*in != -1)
		close(*in);
	fd = open(heredoc, O_RDONLY);
	if (unlink(heredoc) == -1)
		print_err_msg(heredoc, ": Can't delete file\n");
	if (fd != -1)
		*in = fd;
	else
	{
		status = GENERIC_ERROR;
		perror_err_msg(heredoc, "");
	}
	return (status != 0);
}
