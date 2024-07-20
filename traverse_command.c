#include "minishell.h"

int traverse_command(char *cmd, char **redir, t_minishell *ms)
{
    int status;
    char **command;
    status = parse_cmd(cmd, &command, ms);
    ms->command = command;
    if (status == 0)
    {
        if (is_builtin(command[0]))
            status = run_builtin(redir, ms);
        else
            status = run_external_with_redir(command, redir, ms);
    }
    ms->exit_status = status;
    free_arr_2d(command);
    return (status);
}

int run_external_with_redir(char **command, char **redir, t_minishell *ms)
{
    int status;
    pid_t pid;

    pid = fork();
    if (pid == -1)
        return (FORK_FAILURE);
    if (pid == 0)
    {
        signal(SIGINT, SIG_DFL); // Default signal handling in child process
        signal(SIGQUIT, SIG_DFL); // Default signal handling in child process

        status = 0;
        if (redir != NULL)
        {
            status = apply_redirects(redir, ms);
        }
        if (status != 0)
            exit(status);
        execution(ms, &command[0], &ms->cmd_data);
        print_err_msg(command[0], ": execve() error occured\n");
        exit(EXECVE_FAILURE);
    }
    else
    {
        ms->children_pids[ms->num_children++] = pid;
    }
    status = fetch_children_status(&pid, 1);
    ms->num_children--; // Decrement the child count after waiting
    return (status);
}

int traverse_tree(t_node **root, t_minishell *ms)
{
    int type;
    int status;

    status = true;
    type = (*root)->type;
    if (type == T_PIPE)
        status = traverse_pipe(root, ms);
    else if (type == T_COMMAND)
        status = traverse_command(((t_command *)*root)->cmd,
                    ((t_redir *)((t_command *)*root)->redir)->redirs, ms);
    free_ast(root);
    return (status);
}
