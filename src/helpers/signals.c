/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 17:54:02 by nkarpilo          #+#    #+#             */
/*   Updated: 2024/07/27 11:58:55 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sigint_handler(int signum)
{
	if (signum == SIGINT)
	{
		g_sigint_received = signum;
		ioctl(0, TIOCSTI, "\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		ft_printf("\033[1A");
	}
}

void	set_signals(int mode)
{
	struct termios	term;

	ft_bzero(&term, sizeof(term));
	tcgetattr(STDIN_FILENO, &term);
	if (mode)
		term.c_lflag |= ECHOCTL;
	else if (!mode)
		term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

static void	interceptor_init(void (*first_handler)(int),
	void (*second_handler)(int))
{
	struct sigaction	sa;
	struct sigaction	sq;

	ft_memset(&sa, 0, sizeof(sa));
	ft_memset(&sq, 0, sizeof(sq));
	sa.sa_handler = first_handler;
	sq.sa_handler = second_handler;
	sigemptyset(&sa.sa_mask);
	sigemptyset(&sq.sa_mask);
	sa.sa_flags = SA_RESTART;
	sq.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sq, NULL);
}

void	signal_interceptor(int mode)
{
	if (mode == DEFAULT)
		interceptor_init(SIG_DFL, SIG_DFL);
	else if (mode == INTERACTIVE)
		interceptor_init(sigint_handler, SIG_IGN);
	else if (mode == HEREDOC)
		interceptor_init(SIG_DFL, SIG_IGN);
	else if (mode == IGNORE)
		interceptor_init(SIG_IGN, SIG_IGN);
}

int	ctrl_d_handler(char *input)
{
	if (!input)
	{
		ft_printf("\033[1A");
		write(1, "\033[13C", 6);
		write(1, "exit\n", 6);
		return (0);
	}
	return (1);
}
