/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redirects.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 00:16:28 by istasheu          #+#    #+#             */
/*   Updated: 2024/07/22 00:35:42 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	apply_append(char *redir, t_minishell *shell, int *out)
{
	int		status;
	int		fd;

	if (*out != -1)
		close(*out);
	while (ft_is_space(*redir))
		redir++;
	status = check_redir(&redir, shell);
	if (status != 0)
		return (status);
	fd = open(redir, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd != -1)
		*out = fd;
	else
	{
		status = GENERIC_ERROR;
		perror_err_shellg(redir, "");
	}
	free(redir);
	return (status);
}

int	apply_redir_in(char *redir, t_minishell *shell, int *in)
{
	int		status;
	int		fd;

	if (*in != -1)
		close(*in);
	while (ft_is_space(*redir))
		redir++;
	status = check_redir(&redir, shell);
	if (status != 0)
		return (status);
	fd = open(redir, O_RDONLY);
	if (fd != -1)
		*in = fd;
	else
	{
		status = GENERIC_ERROR;
		perror_err_shellg(redir, "");
	}
	free(redir);
	return (status);
}

int	apply_redir_out(char *redir, t_minishell *shell, int *out)
{
	int		status;
	int		fd;

	if (*out != -1)
		close(*out);
	while (ft_is_space(*redir))
		redir++;
	status = check_redir(&redir, shell);
	if (status != 0)
		return (status);
	fd = open(redir, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd != -1)
		*out = fd;
	else
	{
		status = GENERIC_ERROR;
		perror_err_shellg(redir, "");
	}
	free(redir);
	return (status);
}

int	apply_redirect(char *redir, t_minishell *shell, int *in, int *out)
{
	int	status;

	status = 0;
	if (ft_strncmp(redir, "<<", 2) == 0)
		status = apply_heredoc(redir + 2, in);
	else if (ft_strncmp(redir, ">>", 2) == 0)
		status = apply_append(redir + 2, shell, out);
	else if (ft_strncmp(redir, "<", 1) == 0)
		status = apply_redir_in(redir + 1, shell, in);
	else if (ft_strncmp(redir, ">", 1) == 0)
		status = apply_redir_out(redir + 1, shell, out);
	return (status);
}

int	apply_redirects(char **redirs, t_minishell *shell)
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
		status = apply_redirect(redirs[i], shell, &in, &out);
		i++;
	}
	if (status == 0)
		status = replace_fd(in, out);
	return (status);
}
