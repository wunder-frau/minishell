#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft_/libft.h"

typedef struct s_node
{
    int             type;
    struct s_node   *left;
    struct s_node   *right;
} t_node;

typedef struct s_hmap
{
    char            *key;
    char            *value;
    struct s_hmap   *next;
} t_hmap;

typedef struct s_cmd_data
{
    char *cmd;
    char **argv;
    char **cmd_paths;
    char *cmd_path;
    char *cmd_line;
    t_hmap **hashmap;
} t_cmd_data;

typedef struct s_minishell
{
    t_hmap **hashmap;
    char *pwd;
    char *oldpwd;
    t_list *history;
    char *history_path;
    int exit_status;
    int fd_output;
    t_node *root;
    bool is_parent;
    bool is_oldpwd_unset;
    t_cmd_data *cmd_data;
} t_minishell;

int is_builtin(char *cmd);
void exec_builtin(t_minishell *shell, char **argv);
void add_new_var(t_hmap **v, char *akey, char *avalue);
char *return_value_hash(t_hmap *v, char *key);
char *ft_get_env(t_hmap *hashmap, const char *key);
void ft_add_env_hash(t_hmap **hashmap, char *key, char *value);

#endif
