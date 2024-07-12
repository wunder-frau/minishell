#include "minishell.h"

// void ft_echo(t_minishell *shell, char **cmd)
// {
//     int i;
//     char *value;

//     i = 1;
//     while (cmd[i])
//     {
//         if (cmd[i][0] == '$')
//         {
//             value = ft_get_env(*(shell->hashmap), cmd[i] + 1);  // Skip the '$' and get the variable name
//             if (value)
//                 ft_putstr_fd(value, 1);
//         }
//         else
//         {
//             ft_putstr_fd(cmd[i], 1);
//         }
//         if (cmd[i + 1])
//             ft_putstr_fd(" ", shell->fd_output);
//         i++;
//     }
//     ft_putstr_fd("\n", 1);
//     shell->exit_status = 0;
// }

void	ft_echo(t_minishell *shell, char **cmd)
{
	int		i;
	char	*value;

	i = 1;
	while (cmd[i])
	{
		if (cmd[i][0] == '$')
		{
			value = ft_get_env(*(shell->hashmap), cmd[i] + 1);  // Skip the '$' and get the variable name
			if (value)
					ft_putstr_fd(value, 1);
		}
		else
		{
			while(cmd[i])
			{
				ft_putstr_fd(cmd[i], 1);
				if (cmd[i + 1])
						ft_putstr_fd(" ", 1);
				i++;
			}
		}
	}
	ft_putstr_fd("\n", 1);
	shell->exit_status = 0;
}