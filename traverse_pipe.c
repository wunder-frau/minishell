#include "minishell.h"

/**
* This function sets up a pipe and coordinates the execution
* of two child processes representing the commands in a pipeline.
*/
int traverse_pipe(t_node **root, t_minishell *ms)
{
    int status;
    int pipefd[2];
    pid_t pids[2];
    t_node *node;

    node = *root;
    if (pipe(pipefd) == -1)
        return (PIPE_FAILURE);
    status = traverse_lhs(&(node->left), ms, pipefd, pids);
    if (status == 0)
        status = traverse_rhs(&(node->right), ms, pipefd, pids);
    status = fetch_children_status(pids, 2);
    return (status);
}

/**
 * Executes the first command, redirecting its output to the pipe.
 */
int traverse_lhs(t_node **node, t_minishell *ms, int pipefd[2], int pids[2])
{
    int status;

    pids[0] = fork();
    if (pids[0] == -1)
        return (FORK_FAILURE);
    if (pids[0] == 0)
    {
        signal(SIGINT, SIG_DFL); // Default signal handling in child process
        signal(SIGQUIT, SIG_DFL); // Default signal handling in child process

        close(pipefd[0]);
        if (dup2(pipefd[1], STDOUT_FILENO) == -1)
        {
            close(pipefd[1]);
            exit(DUP_FAILURE);
        }
        close(pipefd[1]);

        status = traverse_tree(node, ms);
        exit(status);
    }
    else
    {
        ms->children_pids[ms->num_children++] = pids[0];
        close(pipefd[1]);
    }
    return (0);
}

int traverse_rhs(t_node **node, t_minishell *ms, int pipefd[2], int pids[2])
{
    int status;

    pids[1] = fork();
    if (pids[1] == -1)
        return (FORK_FAILURE);
    if (pids[1] == 0)
    {
        signal(SIGINT, SIG_DFL); // Default signal handling in child process
        signal(SIGQUIT, SIG_DFL); // Default signal handling in child process

        if (dup2(pipefd[0], STDIN_FILENO) == -1)
        {
            close(pipefd[0]);
            exit(DUP_FAILURE);
        }
        close(pipefd[0]);

        status = traverse_tree(node, ms);
        exit(status);
    }
    else
    {
        ms->children_pids[ms->num_children++] = pids[1];
        close(pipefd[0]);
    }
    return (0);
}

int fetch_children_status(pid_t *pids, int num)
{
    int i;
    int status;

    i = 0;
    while (i < num)
    {
        if (pids[i] == -1)
            return (FORK_FAILURE);
        waitpid(pids[i], &status, 0);
        i++;
    }
    if (WIFEXITED(status))
        return (WEXITSTATUS(status));
    return (EXIT_FAILURE);
}
