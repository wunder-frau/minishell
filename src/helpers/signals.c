/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 17:54:02 by nkarpilo          #+#    #+#             */
/*   Updated: 2024/07/27 10:09:32 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// void	sigint_handler(int signum)
// {
// 	if (signum == SIGINT)
// 	{
// 		g_sigint_received = signum;
// 		//ioctl(0, TIOCSTI, "\n");
// 		rl_on_new_line();
// 		rl_replace_line("", 0);
// 		ft_printf("\033[1A");
// 	}
// }

// void	set_signals(int mode)
// {
// 	struct termios	term;

// 	ft_bzero(&term, sizeof(term));
// 	tcgetattr(STDIN_FILENO, &term);
// 	if (mode)
// 		term.c_lflag |= ECHOCTL;
// 	else if (!mode)
// 		term.c_lflag &= ~ECHOCTL;
// 	tcsetattr(STDIN_FILENO, TCSANOW, &term);
// }

// int	ctrl_d_handler(char *input)
// {
// 	if (!input)
// 	{
// 		// ft_printf("\033[1A");
// 		// write(1, "\033[13C", 6);
// 		write(1, "exit\n", 6);
// 		return (0);
// 	}
// 	return (1);
// }

void	sigint_handler(int signum)
{
	if (signum == SIGINT)
	{
		g_sigint_received = 1;
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		//rl_redisplay();
	}
}

void	set_signals(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

int	ctrl_d_handler(char *input)
{
	if (!input)
	{
		write(1, "exit\n", 5);
		return (0);
	}
	return (1);
}