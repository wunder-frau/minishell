#include "../minishell.h"

int g_sigint_received = 0;

void sigint_handler(int signum)
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

void set_signals(void)
{
    struct termios term;

    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~ECHOCTL;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);
}

int ctrl_d_handler(char *input)
{
    if (!input)
    {
        write(1, "exit\n", 5);
        return (0);
    }
    return (1);
}
