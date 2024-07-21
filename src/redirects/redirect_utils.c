/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 23:44:22 by istasheu          #+#    #+#             */
/*   Updated: 2024/07/22 00:42:27 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	parse_cmd(char *cmd, char ***res, t_minishell *shell)
{
	char	**arr;
	int		status;
	int		i;

	if (is_blank_string(cmd))
	{
		cmd = "";
	}
	arr = split_handle_quotes_and_spaces(cmd);
	if (!arr)
		return (MALLOC_ERR);
	status = 0;
	i = 0;
	while (arr[i] && status == 0)
	{
		status = dollar_expansion(&arr[i++], shell, shell->exit_status);
	}
	if (status != 0 && arr != NULL)
		free_arr_2d(arr);
	if (status == 0)
	{
		remove_quotes_arr(arr, 0);
		*res = arr;
	}
	return (status);
}

int	check_redir(char **redir, t_minishell *shell)
{
	int		status;
	char	**rdr;

	status = parse_cmd(*redir, &rdr, shell);
	if (status != 0)
		return (status);
	if (ft_size((void **)rdr) != 1)
	{
		status = 1;
		print_err_shellg(*redir, ": ambiguous redirect\n");
		free_arr_2d(rdr);
	}
	else
	{
		*redir = rdr[0];
		free(rdr);
	}
	return (status);
}

int	prepare_redirects(char *redirects_line, int *hd_count, char ***redirs,
		t_minishell *shell)
{
	int	status;

	if (redirects_line == NULL)
	{
		*redirs = NULL;
		return (0);
	}
	*redirs = ft_split(redirects_line, SEPARATOR);
	free(redirects_line);
	if (!*redirs)
		return (200);
	status = 0;
	status = prepare_heredocs(redirs, hd_count, shell);
	if (status != 0)
	{
		free_arr_2d(*redirs);
		remove_hd_files(hd_count);
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
		if (dup2(in, STDIN_FILENO) == -1)
			status = DUP_FAILURE;
		close(in);
	}
	if (status == 0 && out != -1)
	{
		if (dup2(out, STDOUT_FILENO) == -1)
			status = DUP_FAILURE;
		close(out);
	}
	return (status);
}
