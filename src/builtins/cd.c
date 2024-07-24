/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpilo <nkarpilo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 16:03:34 by nkarpilo          #+#    #+#             */
/*   Updated: 2024/07/24 16:15:49 by nkarpilo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	change_directory(t_minishell *shell, char **argv, char *home)
{
	int	ret;

	if (!argv[1] || strcmp(argv[1], "~") == 0)
	{
		ret = chdir(home);
	}
	else if (strcmp(argv[1], "-") == 0)
	{
		if (shell->oldpwd)
		{
			ret = chdir(shell->oldpwd);
		}
		else
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
			ret = -1;
		}
	}
	else
	{
		ret = chdir(argv[1]);
	}
	return (ret);
}

void	handle_chdir_error(t_minishell *shell, char *arg, char *oldpwd)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	shell->exit_status = 1;
	free(oldpwd);
}

void	update_environment_variables(t_minishell *shell, \
	char *oldpwd, char *pwd)
{
	ft_add_env_hash(shell->hashmap, "OLDPWD", oldpwd);
	ft_add_env_hash(shell->hashmap, "PWD", pwd);
	if (shell->oldpwd)
		free(shell->oldpwd);
	shell->oldpwd = oldpwd;
	free(pwd);
	shell->exit_status = 0;
}

void	ft_cd(t_minishell *shell, char **argv)
{
	char	*home;
	char	*oldpwd;
	char	*pwd;
	int		ret;

	handle_too_many_arguments(shell, argv);
	if (shell->exit_status == 1)
		return ;
	home = ft_get_env(*(shell->hashmap), "HOME");
	oldpwd = get_current_directory(shell);
	if (!oldpwd)
		return ;
	ret = change_directory(shell, argv, home);
	if (ret == -1)
	{
		handle_chdir_error(shell, argv[1], oldpwd);
		return ;
	}
	pwd = get_current_directory(shell);
	if (!pwd)
	{
		free(oldpwd);
		return ;
	}
	update_environment_variables(shell, oldpwd, pwd);
}
