#include "minishell.h"

int	check_redir(char **redir, t_minishell *ms)
{
	int		status;
	char	**rdr;

	printf("🔍 Checking redirections... ");
	status = parse_cmd(*redir, &rdr, ms);
	if (status != 0)
		return (status);
	if (ft_size((void **)rdr) != 1)
	{
		status = 1;
		print_err_msg(*redir, ": ambiguous redirect\n");
		free_arr_2d(rdr);
	}
	else
	{
		*redir = rdr[0];
		free(rdr);
	}
	return (status);
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
		free_arr_2d(*redirs);
		remove_hd_files(hd_num);
	}
	if (status == SIGINT + 128)
		status = 1;
	return (status);
}

int	replace_fd(int in, int out)
{
	int	status;

	status = 0;
	if (in != -1)
	{
	printf("______PPP");
				printf("\n");
		if (dup2(in, STDIN_FILENO) == -1)
			status = DUP_FAILURE;
		close(in);
								printf("______PPP}}");
				printf("\n");
	}
	if (status == 0 && out != -1)
	{
		printf("______here");
		printf("\n");
		if (dup2(out, STDOUT_FILENO) == -1)
			status = DUP_FAILURE;
		close(out);
								printf("______here}}}}");
				printf("\n");
	}
	return (status);
}