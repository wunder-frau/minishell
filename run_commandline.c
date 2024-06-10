#include "parser.h"
#include "minishell.h"
#include "lexer.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while(*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return(*s1 - *s2);
}

void ft_readline(char **cmdline, char *prompt)
{
	*cmdline = readline(prompt);
}

void	run_commandline(t_minishell **ms)
{
    char	*cmdline;
    (void)**ms;

     while (1)
    {
        ft_readline(&cmdline, "minishell> ");
        if (cmdline)
        {
            if (ft_strcmp(cmdline, "exit") == 0)
            {
                free(cmdline);
                break;
            }
            add_history(cmdline);

            int num_tokens;
            Token **tokens = lexer(cmdline, &num_tokens);

            for (int i = 0; i < num_tokens; i++)
            {
                printf("Token type: %d, value: '%s'\n", tokens[i]->type, tokens[i]->value);
                free_token(tokens[i]);
            }
            free(tokens);

            free(cmdline);
        }
    }
    if (cmdline)
    {
        add_history(cmdline);
        free(cmdline);
    }
}