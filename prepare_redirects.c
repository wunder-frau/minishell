#include "minishell.h"

int	check_redir(char **redir, t_minishell *ms)
{
	int		status;
	char	**rdr;

	status = parse_cmd(*redir, &rdr, ms);
	if (status != 0)
		return (status);
	if (ft_arrlen((void **)rdr) != 1)
	{
		status = 1;
		print_err_msg(*redir, ": ambiguous redirect\n");
		ft_free_2d_array(rdr);
	}
	else
	{
		*redir = rdr[0];
		free(rdr);
	}
	return (status);
}

void	print_err_msg(char *cmd, char *msg)
{
	cmd = ft_strjoin("\033[0;31me-bash: \033[0;0m", cmd);
	if (!cmd)
		return ;
	msg = ft_strjoin(cmd, msg);
	free(cmd);
	if (!msg)
		return ;
	ft_putstr_fd(msg, STDERR_FILENO);
	free(msg);
}

void	perror_err_msg(char *cmd, char *arg)
{
	cmd = ft_strjoin("\033[0;31me-bash: \033[0;0m", cmd);
	if (!cmd)
		return ;
	arg = ft_strjoin(cmd, arg);
	free(cmd);
	if (!arg)
		return ;
	perror(arg);
	free(arg);
}

int	prepare_redirects(char *redirects_line, int *hd_num, char ***redirs,
		t_minishell *ms)
{
	int	status;
	(void)*ms;
	if (redirects_line == NULL)
	{
		*redirs = NULL;
		return (0);
	}
	*redirs = ft_split(redirects_line, SEPARATOR);
	free(redirects_line);
	if (!*redirs)
		return (200);
	//status = 0;
	status = prepare_heredocs(redirs, hd_num, ms);
	if (status != 0)
	{
		ft_free_2d_array(*redirs);
		remove_hd_files(hd_num);
	}
	if (status == SIGINT + 128)
		status = 1;
	return (status);
}

int	prepare_heredocs(char ***redirs, int *hd_num, t_minishell *ms)
{
	int		i;
	int		hd_counter;
	int		status;
	char	*hd_name;
	(void)*ms;
	hd_name = get_hd_name(hd_num);
	if (!hd_name)
		return (200);
	i = 0;
	hd_counter = 0;
	status = 0;
	while ((*redirs)[i] && status == 0)
	{
		if (ft_strncmp("<<", (*redirs)[i], 2) == 0)
		{
			hd_counter++;
			//status = 0;
			status = prepare_heredoc(*redirs + i, hd_name, ms);
		}
		i++;
	}
	if (hd_counter > 0)
		(*hd_num)++;
	remove_hd_duplicates(redirs, hd_name, hd_counter);
	return (status);
}

char	*get_hd_name(int *hd_num)
{
	char	*filename;
	char	*num;

	num = ft_itoa(*hd_num);
	if (!num)
		return (NULL);
	filename = ft_strjoin(HEREDOC_NAME, num);
	free(num);
	return (filename);
}

void	remove_hd_files(int *hd_num)
{
	char	*file_name;
	char	*num;
	int		i;

	i = -1;
	while (++i < *hd_num)
	{
		num = ft_itoa(i);
		if (!num)
		{
			print_err_msg("unlink", ": malloc error occured");
			continue ;
		}
		file_name = ft_strjoin(&(HEREDOC_NAME[2]), num);
		free(num);
		if (!file_name)
		{
			print_err_msg("unlink", ": malloc error occured");
			continue ;
		}
		if (unlink(file_name) != 0)
			perror_err_msg("unlink: ", file_name);
		free(file_name);
	}
}
