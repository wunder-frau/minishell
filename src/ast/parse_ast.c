#include "../minishell.h"
#include <stdio.h> // For debug print

bool parse_ast(char *str, t_parsed_data **data)
{
    int status;
    int last_str_index;

    last_str_index = ft_strlen(str) - 1;
    if (last_str_index < 0)
    {
        return false;
    }

    status = lexer(data, str, T_PIPE, last_str_index);
    return (status);
}

int build_ast(char *str, t_node **root, int *hd_c, t_minishell *shell)
{
    bool status;
    t_parsed_data *data;
    int type;

    if (str == NULL || str[0] == '\0')
    {
        return 200;
    }

    status = parse_ast(str, &data);
    if (status == false)
        return (200);

    if (init_node(data, root) == false)
    {
        return (200);
    }

    type = data->type;

    if (type == T_PIPE)
    {
        status = assemble_ast_pipe(data, root, hd_c, shell);
    }
    else if (type == T_COMMAND)
    {
        status = assemble_ast_command(data, root, hd_c, shell);
    }
    free(data);
    if (status != SUCCESS)
    {
        free_ast(root);
        return (status);
    }

    return (SUCCESS);
}
