#include "minishell.h"

/**
 * @brief    Validates the syntax of an input string according to the BNF
 *           syntax format logic.
 *
 *           This function is responsible for ensuring the syntactic
 *           correctness of the input string according to the specified Backus-Naur
 *           Form (BNF) syntax format.
 *
 *           BNF form looks as follows:
 *           - <input>      ::= <pipeline> <newline>
 *           - <pipeline>   ::= <command> { '|' <command> }
 *           - <command>    ::= { <redirect> | <word> | "any" | 'any' }
 *           - <redirect>   ::= '<' | '>' | '<<' | '>>' <word>
 *
 * @param    str String obtained from the readline function.
 *
 * @return   -1 if a memory allocation error occurs during processing,
 *           1 if the input string passes syntax validation,
 *           0 if the input string fails to meet the required conditions.
 */

int	validate_input(char *str)
{
	char	*next_token;
	bool	status;

	status = true;
	while (ft_is_space(*str))
		str++;
	if (*str == NULL_TERM)
		return (SUCCESS);
	next_token = validate_pipeline(str, &status);
	if (*next_token != NULL_TERM || status == false)
	{
		handle_and_display_error(next_token);
		return (SYNTAX_ERROR);
	}
	return (SUCCESS);
}


char	*validate_pipeline(char *str, bool *status)
{
	char	*next_token;

	while (ft_is_space(*str))
		str++;
	next_token = validate_command(str, status);
	if (next_token == str)
		*status = false;
	if (*status == false)
		return (next_token);
	else if (*next_token == PI && next_token[1] != PI)
	{
		next_token++;
		if (is_blank_string(next_token) == true)
		{
			*status = false;
			return (next_token);
		}
		next_token = validate_pipeline(next_token, status);
	}
	return (next_token);
}

char	*validate_command(char *str, bool *status)
{
	char	*next_token;

	while (ft_is_space(*str))
		str++;
	next_token = validate_redirect(str, status);
	if (next_token == str)
		next_token = validate_word(str, status);
	if (*status == false)
		return (next_token);
	if (next_token != str)
		next_token = validate_command(next_token, status);
	return (next_token);
}

char	*validate_redirect(char *str, bool *status)
{
	char	*next_token;

	if (ft_strncmp(">>", str, 2) == 0)
		str += 2;
	else if (ft_strncmp("<<", str, 2) == 0)
		str += 2;
	else if (ft_strncmp(">", str, 1) == 0)
		str += 1;
	else if (ft_strncmp("<", str, 1) == 0)
		str += 1;
	else
		return (str);
	while (ft_is_space(*str))
		str++;
	next_token = validate_word(str, status);
	if (next_token == str)
		*status = false;
	if (*status == false)
		return (next_token);
	while (ft_is_space(*next_token))
		next_token++;
	next_token = validate_redirect(next_token, status);
	return (next_token);
}

char *validate_word(char *str, bool *status)
{
	char *special_characters = "|<> '\"";
	printf("Validating word: %s\n", str);

	while (*str && ft_strchr(special_characters, *str) == NULL)
		str++;
	if (*str == S_QUO || *str == D_QUO)
		str = validate_quotes(str, status);
	return (str);
}

char *validate_quotes(char *str, bool *status)
{
	char quote_char;

	quote_char = *str;
		str++;
	while (*str && *str != quote_char)
		str++;
	if (*str == quote_char)
		str++;
	else
		*status = false;
	return (str);
}

bool is_special_token(char *str, int *length)
{
	if (strncmp(">>", str, 2) == 0 || strncmp("<<", str, 2) == 0)
	{
		*length = 2;
		return (true);
	}
	else if (*str == PI || *str == REDIR_L || *str == REDIR_R)
	{
		*length = 1;
		return (true);
	}
	return (false);
}

bool	is_alnum_or_quote(char c)
{
	return (isalnum(c) || c == S_QUO || c == D_QUO);
}

void	print_error_message(char *str)
{
	ft_putstr_fd("shell: syntax error near unexpected token `", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putendl_fd("`", STDERR_FILENO);
}

void	handle_and_display_error(char *str)
{
	int i;
	int token_length;

	i = 0;
	token_length = 0;
	if (*str == NULL_TERM)
	{
			str = "newline";
			printf("LOOOOOK%s\n", str);
	}
	else if (is_special_token(str, &token_length))
	{
		i = token_length;
		printf("loooooooool:%s\n", str);
	}
	else if (is_alnum_or_quote(*str))
	{
		while (is_alnum_or_quote(str[i]))
			i++;
	}
	if (i != 0)
		str[i] = NULL_TERM;
	print_error_message(str);
}
