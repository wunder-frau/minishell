#include "minishell.h"

char *ft_get_env(t_hmap *hashmap, const char *key)
{
    while (hashmap)
    {
        if (ft_strcmp(hashmap->key, key) == 0)
            return hashmap->value;
        hashmap = hashmap->next;
    }
    return NULL;
}

void ft_pwd(t_minishell *shell)
{
    char *pwd;
    pwd = ft_get_env(*(shell->hashmap), "PWD");
    if (pwd)
    {
        ft_putstr_fd(pwd, 1);
        ft_putstr_fd("\n", 1);
    }
    else
    {
        ft_putstr_fd("minishell: pwd: PWD not set\n", 2);
        shell->exit_status = 1;
    }
}

// void run_pwd(char **args, t_minishell *minishell)
// {
// 	char *current_dir;
// 	int args_count;

// 	printf("Debug: Entering run_pwd function\n");

// 	args_count = ft_size((void **)args);
// 	printf("Debug: Argument count = %d\n", args_count);

// 	current_dir = getcwd(NULL, 0);
// 	if (!current_dir) {
// 		minishell->exit_status = GETCWD_ERROR;
// 		perror("getcwd");
// 		printf("Debug: getcwd error, setting exit status to %d\n", GETCWD_ERROR);
// 		return;
// 	}

// 	printf("\n");
// 	ft_printf("SUKKKKKKA NAAAAAAAAAAAA 🔥PWD🔥%s\n", current_dir);
// 	printf("\n");
// 	free(current_dir);

// 	printf("Debug: Exiting run_pwd function\n");
// }
