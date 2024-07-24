/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_unset.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpilo <nkarpilo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 17:20:13 by nkarpilo          #+#    #+#             */
/*   Updated: 2024/07/24 17:40:40 by nkarpilo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !str[0])
		return (false);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (false);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

void	add_to_env_hash(t_minishell *shell, char *key, char *value)
{
	if (value)
		ft_add_env_hash(shell->hashmap, key, value);
	else
		ft_add_env_hash(shell->hashmap, key, "");
}

void	handle_export_argument(t_minishell *shell, char *arg)
{
	int		j;
	char	*key;
	char	*value;

	j = 0;
	while (arg[j] && arg[j] != '=')
		j++;
	key = ft_substr(arg, 0, j);
	if (!is_valid_identifier(key))
	{
		print_arg_err_msg("export: '", arg, \
		"' : not a valid identifier\n");
		free(key);
		shell->exit_status = 1;
		return ;
	}
	if (arg[j] == '=')
		value = ft_strdup(arg + j + 1);
	else
		value = NULL;
	add_to_env_hash(shell, key, value);
	free(key);
	free(value);
}

void	ft_export(t_minishell *shell, char **argv)
{
	int		i;

	i = 1;
	while (argv[i])
	{
		handle_export_argument(shell, argv[i]);
		i++;
	}
}

void	ft_unset(t_minishell *shell, char **argv)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (is_valid_identifier(argv[i]))
		{
			ft_remove_env_hash(shell->hashmap, argv[i]);
		}
		else
		{
			print_arg_err_msg("unset: '", argv[i], \
			"' : not a valid identifier\n");
			shell->exit_status = 1;
		}
		i++;
	}
	shell->exit_status = 0;
}
