/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 19:45:28 by istasheu          #+#    #+#             */
/*   Updated: 2024/07/23 01:24:41 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	remove_hd_duplicates(char ***redirs, char *hd_name, char hd_c)
{
	int		i;
	int		j;
	char	**rdr;

	rdr = *redirs;
	i = 0;
	j = 0;
	if (hd_c == 0)
		free(hd_name);
	while (hd_c > 1)
	{
		if (rdr[i] == hd_name)
		{
			j = i;
			while (rdr[j] != NULL_TERM)
			{
				rdr[j] = rdr[j + 1];
				j++;
			}
			hd_c--;
			continue ;
		}
		i++;
	}
}

int	prepare_heredocs(char ***redirs, int *hd_c, t_minishell *shell)
{
	int		i;
	int		hd_c;
	int		status;
	char	*hd_name;

	hd_name = get_hd_name(hd_c);
	if (!hd_name)
		return (200);
	i = 0;
	hd_c = 0;
	status = 0;
	while ((*redirs)[i] && status == 0)
	{
		if (ft_strncmp("<<", (*redirs)[i], 2) == 0)
		{
			hd_c++;
			status = prepare_heredoc(*redirs + i, hd_name, shell);
		}
		i++;
	}
	if (hd_c > 0)
		(*hd_c)++;
	remove_hd_duplicates(redirs, hd_name, hd_c);
	return (status);
}

char	*get_hd_name(int *hd_c)
{
	char	*filename;
	char	*num;

	num = ft_itoa(*hd_c);
	if (!num)
		return (NULL);
	filename = ft_strjoin(HEREDOC_NAME, num);
	free(num);
	return (filename);
}

void	remove_hd_files(int *hd_c)
{
	char	*file_name;
	char	*num;
	int		i;

	i = -1;
	while (++i < *hd_c)
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
			perror_err_shellg("unlink: ", file_name);
		free(file_name);
	}
}
