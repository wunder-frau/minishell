#include "minishell.h"

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
