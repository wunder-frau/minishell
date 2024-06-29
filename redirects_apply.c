#include "minishell.h"

int	apply_append(char *redir, t_minishell *ms, int *out)
{
	int		status;
	int		fd;

	if (*out != -1)
		close(*out);
	while (ft_is_space(*redir))
		redir++;
	printf("______________apply_append____________");
	status = check_redir(&redir, ms);
	if (status != 0)
		return (status);
	fd = open(redir, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd != -1)
		*out = fd;
	else
	{
		status = GENERIC_ERROR;
		perror_err_msg(redir, "");
	}
	free(redir);
	return (status);
}

int	apply_redir_in(char *redir, t_minishell *ms, int *in)
{
	int		status;
	int		fd;

	printf("______________lol_out_2____________");
	if (*in != -1)
		close(*in);
	while (ft_is_space(*redir))
		redir++;
	status = check_redir(&redir, ms);
	if (status != 0)
		return (status);
	fd = open(redir, O_RDONLY);
	if (fd != -1)
		*in = fd;
	else
	{
		status = GENERIC_ERROR;
		perror_err_msg(redir, "");
	}
	printf("lol");
	free(redir);
	return (status);
}

int	apply_redir_out(char *redir, t_minishell *ms, int *out)
{
	int		status;
	int		fd;

	printf("lol_out_1");
	if (*out != -1)
		close(*out);
	while (ft_is_space(*redir))
		redir++;
	status = check_redir(&redir, ms);
	if (status != 0)
		return (status);
	fd = open(redir, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd != -1)
		*out = fd;
	else
	{
		status = GENERIC_ERROR;
		perror_err_msg(redir, "");
	}
	printf("lol_out");
	free(redir);
	return (status);
}

int	apply_redirect(char *redir, t_minishell *ms, int *in, int *out)
{
	int	status;
	//(void)ms;
	status = 0;
	printf("test");
	printf("\n");
	if (ft_strncmp(redir, "<<", 2) == 0)
		status = apply_heredoc(redir + 2, in);
	else if (ft_strncmp(redir, ">>", 2) == 0)
		status = apply_append(redir + 2, ms, out);
	else if (ft_strncmp(redir, "<", 1) == 0)
	{
		printf("______________<____________");
				printf("\n");
		status = apply_redir_in(redir + 1, ms, in);
	}
	else if (ft_strncmp(redir, ">", 1) == 0)
	{
				printf("______________>____________");
				printf("\n");
						status = apply_redir_out(redir + 1, ms, out);
	}
	else
	{
		fprintf(stderr, "Unsupported redirection: %s\n", redir);
		status = 1;
	}
	return (status);
}

int	apply_redirects(char **redirs, t_minishell *ms)
{
	int		i;
	int		status;
	int		in;
	int		out;

	in = -1;
	out = -1;
	status = 0;
	i = 0;
	while (redirs[i] && status == 0)
	{
		status = apply_redirect(redirs[i], ms, &in, &out);
		i++;
	}
	if (status == 0)
	{
		printf("replace_fd");
		status = replace_fd(in, out);
				printf("replace_fd_____");
	}
	return (status);
}
