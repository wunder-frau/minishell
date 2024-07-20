#include "../minishell.h"

bool	is_non_digit_space_sign(const char *str)
{
	while (*str)
	{
		if (!ft_isdigit(*str) && !ft_is_space(*str) && *str != '+' && *str != '-')
			return (true);
		str++;
	}
	return (false);
}

bool	is_non_empty_after_trim(const char *str)
{
	int	i;

	i = 0;
	while (ft_is_space(*str))
		str++;
	if ((*str == '+' || *str == '-') && ft_isdigit(str[i + 1]))
		str++;
	while (*str == '0')
		str++;
	while (ft_is_space(*str))
		str++;
	return (*str != '\0');
}

void	exit_numeric_arg_error(t_minishell *ms, char *str)
{
	if (ms->is_parent)
		ft_putstr_fd("exit\n", STDERR_FILENO);
	print_arg_err_msg("exit: ", str,
		": numeric argument required\n");
	ms->exit_status = UNEXPECTED_EXIT;
	if (ms->is_parent == true)
		clean_and_exit(ms);
	exit(ms->exit_status);
}

void	exit_amount_of_arg_error(t_minishell *ms)
{
	if (ms->is_parent)
		ft_putstr_fd("exit\n", STDERR_FILENO);
	print_err_msg("exit: ", "too many arguments\n");
	ms->exit_status = GENERIC_ERROR;
}

void	clean_and_exit(t_minishell *ms)
{
	int status;

	status = ms->exit_status;
	rl_clear_history();
	free_minishell(ms);
	exit(status);
}