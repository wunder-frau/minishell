#include "minishell.h"

int	add_to_env_list_new_env(char **envp, char ***result,
	int *i, int *len)
{
	*len = ft_size((void **)envp);
	*result = (char **)ft_calloc(*len + *i + 1, sizeof(char *));
	if (!*result)
		return (MALLOC_ERR);
	ft_memcpy((void *)*result, (void *)envp, *len * sizeof(char *));
	free(envp);
	*i = 0;
	return (SUCCESS);
}

void	shlvl_warning(int number)
{
	char	*first_part;
	char	*num;
	char	*first_num_part;
	char	*msg;

	first_part = "\033[0;31me-bash: \033[0;0mwarning: minishell level (";
	num = ft_itoa(number);
	if (!num)
		return ;
	first_num_part = ft_strjoin(first_part, num);
	free(num);
	if (!first_num_part)
		return ;
	msg = ft_strjoin(first_num_part, ") too high, resetting to 1\n");
	free(first_num_part);
	if (!msg)
		return ;
	ft_putstr_fd(msg, STDERR_FILENO);
	free(msg);
}

int	env_var(char **envp, char *var, int i, int j)
{
	while (envp[++i])
	{
		if (ft_strncmp(envp[i], var, j) == 0)
			return (i);
	}
	return (-1);
}

int	check_for_non_digits(char *str)
{
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	if (*str == '+' || *str == '-')
		str++;
	while (*str >= '0' && *str <= '9')
		str++;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	if (*str != NULL_TERM)
		return (true);
	return (false);
}

int	shlvl_init_when_shlvl_exists(int position, char ***envp,
	int number, char *shlvl)
{
	char	*new_number;
	char	*temp;

	shlvl = ft_strrchr((*envp)[position], EQUAL) + 1;
	if (check_for_non_digits(shlvl))
		number = 0;
	else
		number = ft_atoi(shlvl);
	if (number < 0)
		number = -1;
	else if (number > 998)
	{
		shlvl_warning(number + 1);
		number = 0;
	}
	new_number = ft_itoa(number + 1);
	if (!new_number)
		return (MALLOC_ERR);
	temp = ft_strjoin("SHLVL=", new_number);
	free(new_number);
	if (!temp)
		return (MALLOC_ERR);
	free((*envp)[position]);
	(*envp)[position] = temp;
	return (SUCCESS);
}

int	var_init_when_no_var_exists(char ***envp, int i, char *var)
{
	int		len;
	char	**new_env;

	if (add_to_env_list_new_env(*envp, &new_env, &i,
			&len) == MALLOC_ERR)
		return (MALLOC_ERR);
	new_env[len] = ft_strdup(var);
	if (!new_env[len])
	{
		free_arr_2d(new_env);
		return (MALLOC_ERR);
	}
	*envp = new_env;
	return (SUCCESS);
}

int	shlvl_init(char ***envp)
{
	int		position;
	int		status;

	status = 0;
	position = env_var(*envp, "SHLVL=", -1, 6);
	if (position == -1)
	{
		status = var_init_when_no_var_exists(envp, 1, "SHLVL=1");
		if (status != 0)
			return (status);
	}
	else
	{
		status = shlvl_init_when_shlvl_exists(position, envp, 0, NULL);
		if (status != 0)
			return (status);
	}
	return (status);
}