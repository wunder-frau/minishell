#include "minishell.h"

static void	remove_spaces_and_quotes_hd(char *hd);
static void	heredoc(char *limiter, int fd, t_minishell *ms);
static void	handle_heredoc_line(int fd, char *line, t_minishell *ms,
				char *limiter);

int	prepare_heredoc(char **limiter, char *hd_name, t_minishell *ms)
{
	int		status;
	int		fd;

	remove_spaces_and_quotes_hd(*limiter + 2);
		fd = open(hd_name + 2, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd != -1)
			heredoc(*limiter + 2, fd, ms);
		exit(GENERIC_ERROR);
	free(*limiter);
	*limiter = hd_name;
	return (status);
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

static void	heredoc(char *limiter, int fd, t_minishell *ms)
{
	char	*line;
	int		isequal;

	while (true)
	{
		line = readline("> ");
		if (!line)
		{
			close(fd);
			exit(0);
		}
		isequal = ft_strcmp(limiter, line);
		if (isequal != 0)
			handle_heredoc_line(fd, line, ms, limiter);
		if (isequal == 0)
		{
			close(fd);
			exit(0);
		}
	}
}

void	remove_hd_duplicates(char ***redirs, char *hd_name, char hd_counter)
{
	int		i;
	int		j;
	char	**rdr;

	rdr = *redirs;
	i = 0;
	j = 0;
	if (hd_counter == 0)
		free(hd_name);
	while (hd_counter > 1)
	{
		if (rdr[i] == hd_name)
		{
			j = i;
			while (rdr[j] != NULL_TERM)
			{
				rdr[j] = rdr[j + 1];
				j++;
			}
			hd_counter--;
			continue ;
		}
		i++;
	}
}

static void	handle_heredoc_line(int fd, char *line, t_minishell *ms,
				char *limiter)
{
	int	status;
  (void)*ms;
	status = 0;
	if (status == 0)
	{
		ft_putendl_fd(line, fd);
		free(line);
	}
	else
	{
		ft_putstr_fd(limiter, 2);
		ft_putendl_fd(": malloc() error occured", 2);
		close(fd);
		exit(status);
	}
}