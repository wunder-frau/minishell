/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 19:45:28 by istasheu          #+#    #+#             */
/*   Updated: 2024/07/22 08:38:03 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	prepare_heredocs(char ***redirs, int *hd_count, t_minishell *shell)
{
	int		i;
	int		hd_counter;
	int		status;
	char	*hd_name;

	hd_name = get_hd_name(hd_count);
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
			status = prepare_heredoc(*redirs + i, hd_name, shell);
		}
		i++;
	}
	if (hd_counter > 0)
		(*hd_count)++;
	remove_hd_duplicates(redirs, hd_name, hd_counter);
	return (status);
}

char	*get_hd_name(int *hd_count)
{
	char	*filename;
	char	*num;

	num = ft_itoa(*hd_count);
	if (!num)
		return (NULL);
	filename = ft_strjoin(HEREDOC_NAME, num);
	free(num);
	return (filename);
}

void	remove_hd_files(int *hd_count)
{
	char	*file_name;
	char	*num;
	int		i;

	i = -1;
	while (++i < *hd_count)
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
