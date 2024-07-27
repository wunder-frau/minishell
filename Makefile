NAME = minishell

# Paths
LIBFT_DIR = libft_
LIBFT = $(LIBFT_DIR)/libft.a
CC = gcc
# CFLAGS = -g -Wall -Wextra -Werror -I$(LIBFT_DIR)
# LDFLAGS = -L$(LIBFT_DIR) -lft -lreadline -no-pie
CFLAGS = -g -Wall -Wextra -Werror -I$(LIBFT_DIR) -I$(shell brew --prefix readline)/include
LDFLAGS = -L$(LIBFT_DIR) -L$(shell brew --prefix readline)/lib -lft -lreadline

OBJ_PATH = build/
SRC_PATH = src/
LEXER_PATH = lexer/
BUILTINS_PATH = builtins/
BNF_PATH = syntax_checker/
REDIR_PATH = redirects/
TRAVERSE_PATH = traverse_tree/
AST_PATH = ast/
HELPERS_PATH = helpers/
RUN_PATH = run/
ENV_PATH = env/
DOLLAR_PATH = dollar_exp/

LEXER_SRC = lexer.c lexer_utils.c lexer_parse_node_data.c
BUILTINS_SRC = exit.c exit_utils.c run_builtin.c cd.c cd_2.c echo.c pwd.c \
export_unset.c
BNF_SRC = expression.c factor.c handle_syntax_error.c term.c
REDIR_SRC = apply_heredocs.c apply_redirects.c heredoc_utils.c redirect_utils.c
TRAVERSE_SRC = traverse_command.c traverse_pipe.c
AST_SRC = assemble_ast.c init_node.c parse_ast.c
HELPERS_SRC = error_handling.c free_utils.c string_utils.c signals.c
RUN_SRC = init_minishell.c main.c run_commandline.c pipe_execve.c pipe_execve_2.c \
exec_builtin.c
ENV_SRC = env.c envp_utils.c shlvl.c dup_envp.c 
DOLLAR_SRC = dollar_expansion.c dollar_expansion_2.c dollar_expansion_3.c

MINISHELL_SRC = $(addprefix $(LEXER_PATH), $(LEXER_SRC)) \
								$(addprefix $(BUILTINS_PATH), $(BUILTINS_SRC)) \
								$(addprefix $(BNF_PATH), $(BNF_SRC)) \
								$(addprefix $(REDIR_PATH), $(REDIR_SRC)) \
								$(addprefix $(TRAVERSE_PATH), $(TRAVERSE_SRC)) \
								$(addprefix $(AST_PATH), $(AST_SRC)) \
								$(addprefix $(HELPERS_PATH), $(HELPERS_SRC)) \
								$(addprefix $(RUN_PATH), $(RUN_SRC)) \
								$(addprefix $(ENV_PATH), $(ENV_SRC)) \
								$(addprefix $(DOLLAR_PATH), $(DOLLAR_SRC))

MINISHELL_OBJ = $(MINISHELL_SRC:.c=.o)
OBJS =	$(addprefix $(OBJ_PATH), $(MINISHELL_OBJ))

# Colors and formatting
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
PURPLE = \033[0;35m
CYAN = \033[0;36m
RESET = \033[0m

# Emojis
SUCCESS_EMOJI = ✅
CLEAN_EMOJI = 🧹
BUILD_EMOJI = 🔨
REMOVE_EMOJI = 🗑️
REBUILD_EMOJI = ♻️

all: $(OBJ_PATH) $(NAME)

# Submodule initialization and update
submodule:
	@git submodule init
	@git submodule update
	@echo "$(SUBMODULE_EMOJI) $(BLUE)Submodules initialized and updated!$(RESET)"

$(NAME): submodule $(LIBFT) $(OBJS)
	@$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)
	@echo "$(BUILD_EMOJI) $(BLUE)Linked:$(RESET) $(NAME)"

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)
	@echo "$(BUILD_EMOJI) $(BLUE)libft built!$(RESET)"

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(BUILD_EMOJI) $(BLUE)Compiled:$(RESET) $<"

$(OBJ_PATH):
	@mkdir $(OBJ_PATH) \
				$(OBJ_PATH)lexer \
				$(OBJ_PATH)builtins \
				$(OBJ_PATH)syntax_checker \
				$(OBJ_PATH)redirects \
				$(OBJ_PATH)traverse_tree \
				$(OBJ_PATH)ast \
				$(OBJ_PATH)helpers \
				$(OBJ_PATH)run \
				$(OBJ_PATH)env \
				$(OBJ_PATH)dollar_exp

clean:
	@rm -f $(MINISHELL_OBJ)
	@rm -rf $(OBJ_PATH)
	@$(MAKE) -C $(LIBFT_DIR) clean
	@echo "$(CLEAN_EMOJI) $(PURPLE)Object files removed!$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "$(REMOVE_EMOJI) $(RED)All files removed!$(RESET)"

re: fclean all
	@echo "$(REBUILD_EMOJI) $(YELLOW)Rebuild complete!$(RESET)"

.PHONY: all clean fclean re

