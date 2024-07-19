#include "minishell.h"

long	ft_atol(const char *str)
{
	unsigned long	result;
	int				sign;

	result = 0;
	sign = 1;
	while(ft_is_space(*str))
		str++;
	if ((*str == '-' || *str == '+') && ft_isdigit(*(str + 1)))
	{
		if (*str == '-')
			sign = -1;
		else
			sign = 1;
		str++;
	}
	while (ft_isdigit(*str))
	{
		result = result * 10 + (*str - 48);
		str++;
	}
	return ((long)sign * result);
}

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

static bool	is_non_empty_after_trim(const char *str)
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

// static void exit_numeric_arg_error(t_minishell *ms, char *str)
// {
//     if (ms->is_parent)
//     {
//         ft_putstr_fd("exit\n", STDERR_FILENO);
//         clean_and_exit(ms);
//     }
//     print_arg_err_msg("exit: ", str, ": numeric argument required\n");
//     ms->exit_status = UNEXPECTED_EXIT;
//     exit(ms->exit_status);
// }

// static void exit_amount_of_arg_error(t_minishell *ms)
// {
//     if (ms->is_parent)
//     {
//         ft_putstr_fd("exit\n", STDERR_FILENO);
//         clean_and_exit(ms);
//     }
//     print_err_msg("exit: ", "too many arguments\n");
//     ms->exit_status = GENERIC_ERROR;
// }

static void	exit_numeric_arg_error(t_minishell *ms, char *str)
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

static void	exit_amount_of_arg_error(t_minishell *ms)
{
	if (ms->is_parent)
		ft_putstr_fd("exit\n", STDERR_FILENO);
	print_err_msg("exit: ", "too many arguments\n");
	ms->exit_status = GENERIC_ERROR;
}

void	handle_multiple_args(char **arg, t_minishell *ms)
{
	long num;
	
	num = ft_atol(arg[0]);
	if ((is_non_empty_after_trim(arg[0]) && num == 0) || is_non_digit_space_sign(arg[0]))
		exit_numeric_arg_error(ms, arg[0]);
	else
		exit_amount_of_arg_error(ms);
}

void	handle_single_arg(char *arg, t_minishell *ms)
{
	long num;
	num = ft_atol(arg);
	if ((is_non_empty_after_trim(arg) && num == 0) || is_non_digit_space_sign(arg))
		exit_numeric_arg_error(ms, arg);
	else
		ms->exit_status = (unsigned char)num;
}

void	ft_exit(char **arg, t_minishell *ms)
{
	bool is_multiple_args;
	bool is_empty_args;

	is_multiple_args = (ft_size((void **)arg) > 1);
	is_empty_args = !*arg;
	if (is_multiple_args)
		handle_multiple_args(arg, ms);
	else if (is_empty_args)
		ms->exit_status = SUCCESS;
	else
		handle_single_arg(arg[0], ms);
	if (ms->is_parent == true)
		ft_putstr_fd("[>>exit<<]\n", STDERR_FILENO);
	if (ms->is_parent == true)
		clean_and_exit(ms);
	// if (ms->is_parent)
	// {
	// 	ft_putstr_fd("exit\n", STDERR_FILENO);
	// 	clean_and_exit(ms);
	// }
	exit(ms->exit_status);
}

void	clean_and_exit(t_minishell *ms)
{
	int status;

	status = ms->exit_status;
	rl_clear_history();
	free_minishell(ms);
	exit(status);
}
