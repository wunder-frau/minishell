#include "../minishell.h"

void sigint_handler(int signum)
{
    if (signum == SIGINT)
    {
			//printf("Hello!");
			// write (1, "\n", 1);
			// rl_on_new_line();
			// rl_replace_line("", 0);
			// rl_redisplay();
	    ioctl(0, TIOCSTI, "\n");
			rl_on_new_line();
			rl_replace_line("", 0);
			ft_printf("\033[1A");
			g_sigint_received = signum;
    }
}

// static void interceptor_init(void (*handler_int)(int), void (*handler_quit)(int)) {
//     struct sigaction sa = {0};

//     sa.sa_handler = handler_int;
//     sa.sa_flags = SA_RESTART;
//     sigemptyset(&sa.sa_mask);
//     sigaction(SIGINT, &sa, NULL);

//     sa.sa_handler = handler_quit;
//     sigaction(SIGQUIT, &sa, NULL);
// }

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
	if (mode == 0)
	{
		interceptor_init(SIG_DFL, SIG_DFL);
	}
	else if (mode == 1)
		interceptor_init(sigint_handler, SIG_IGN);
	else if (mode == 2)
		interceptor_init(SIG_DFL, SIG_IGN);
	else if (mode == 3)
		interceptor_init(SIG_IGN, SIG_IGN);
}

void set_signals(int mode)
{
  struct termios	terminal;

	ft_bzero(&terminal, sizeof(terminal));
	tcgetattr(STDIN_FILENO, &terminal);
	if (mode)
		terminal.c_lflag |= ECHOCTL;
	else if (!mode)
		terminal.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &terminal);
}

int ctrl_d_handler(char *input)
{
    if (!input)
    {
			ft_printf("\033[1A"); // Move cursor up one line
			write(1, "\033[13C", 6); // Move cursor right two characters
			write(1, "exit\n", 6);  // Write "exit" and newline
			return (0);
    }
    return (1);
}

