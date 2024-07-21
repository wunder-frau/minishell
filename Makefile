# # Your existing Makefile

# NAME = minishell
# MINISHELL_SRC = utils.c \
# 								prepare_heredocs.c \
# 								redirects_apply.c \
# 								lexer_utils.c lexer.c \
# 								main.c run_commandline.c \
# 								parse_ast.c \
# 								traverse_command.c \
# 								dup_envp.c \
# 								utils_free.c \
# 								init_minishell.c \
# 								assemble_ast.c \
# 								init_node.c \
# 								to_delete.c \
# 								heredoc_utils.c \
# 								error_handling.c \
# 								redirect_utils.c \
# 								builtins.c \
# 								run_builtin.c \
# 								pwd.c \
# 								echo.c \
# 								export_unset.c \
# 								env.c \
# 								cd.c \
# 								pipe_execve.c \
# 								traverse_pipe.c \
# 								signals.c \
# 								validation_input.c \
# 								shlvl.c \
# 								dollar_expansion.c
# MINISHELL_OBJ = $(MINISHELL_SRC:.c=.o)

# # Paths
# LIBFT_DIR = libft_
# LIBFT = $(LIBFT_DIR)/libft.a
# CC = gcc
# CFLAGS = -g -Wall -Wextra -Werror -I$(LIBFT_DIR)
# LDFLAGS = -L$(LIBFT_DIR) -lft

# # Check if readline is installed via brew or system
# READLINE_INC = $(shell brew --prefix readline 2>/dev/null || echo /usr/local/opt/readline/include)
# READLINE_LIB = $(shell brew --prefix readline 2>/dev/null || echo /usr/local/opt/readline/lib)
# CFLAGS += -I$(READLINE_INC)
# LDFLAGS += -L$(READLINE_LIB) -lreadline

# # Colors and formatting
# RED = \033[0;31m
# GREEN = \033[0;32m
# YELLOW = \033[0;33m
# BLUE = \033[0;34m
# PURPLE = \033[0;35m
# CYAN = \033[0;36m
# RESET = \033[0m

# # Emojis
# SUCCESS_EMOJI = ✅
# CLEAN_EMOJI = 🧹
# BUILD_EMOJI = 🔨
# REMOVE_EMOJI = 🗑️
# REBUILD_EMOJI = ♻️

# all: $(NAME)

# $(NAME): $(LIBFT) $(MINISHELL_OBJ)
# 	@$(CC) $(MINISHELL_OBJ) $(LDFLAGS) -o $(NAME)
# 	@echo "$(BUILD_EMOJI) $(BLUE)Linked:$(RESET) $(NAME)"

# $(LIBFT):
# 	@$(MAKE) -C $(LIBFT_DIR)
# 	@echo "$(BUILD_EMOJI) $(BLUE)libft built!$(RESET)"

# $(MINISHELL_OBJ): %.o: %.c
# 	@$(CC) $(CFLAGS) -c $< -o $@
# 	@echo "$(BUILD_EMOJI) $(BLUE)Compiled:$(RESET) $<"

# clean:
# 	@rm -f $(MINISHELL_OBJ)
# 	@$(MAKE) -C $(LIBFT_DIR) clean
# 	@echo "$(CLEAN_EMOJI) $(PURPLE)Object files removed!$(RESET)"

# fclean: clean
# 	@rm -f $(NAME)
# 	@$(MAKE) -C $(LIBFT_DIR) fclean
# 	@echo "$(REMOVE_EMOJI) $(RED)All files removed!$(RESET)"

# re: fclean all
# 	@echo "$(REBUILD_EMOJI) $(YELLOW)Rebuild complete!$(RESET)"

# # Test-related variables and targets
# TEST_SRCS = tests/test_minishell.c
# TEST_OBJS = $(TEST_SRCS:.c=.o)
# TEST_EXEC = test_minishell
# TEST_CFLAGS = $(CFLAGS) -DUNIT_TEST

# test: $(TEST_EXEC)
# 	./$(TEST_EXEC)

# $(TEST_EXEC): $(TEST_OBJS) $(filter-out main.o, $(MINISHELL_OBJ))
# 	@$(CC) $(TEST_OBJS) $(filter-out main.o, $(MINISHELL_OBJ)) $(LDFLAGS) -o $@
# 	@echo "$(BUILD_EMOJI) $(GREEN)Test executable built!$(RESET)"

# clean_test:
# 	@rm -f $(TEST_OBJS) $(TEST_EXEC)
# 	@echo "$(CLEAN_EMOJI) $(PURPLE)Test object files removed!$(RESET)"

# .PHONY: all clean fclean re test clean_test


NAME = minishell
MINISHELL_SRC = utils.c \
								prepare_heredocs.c \
								redirects_apply.c \
								lexer_utils.c lexer.c \
								main.c run_commandline.c \
								parse_ast.c \
								traverse_command.c \
								dup_envp.c \
								utils_free.c \
								init_minishell.c \
								assemble_ast.c \
								init_node.c \
								to_delete.c \
								heredoc_utils.c \
								error_handling.c \
								redirect_utils.c \
								builtins.c \
								run_builtin.c \
								pwd.c \
								echo.c \
								export_unset.c \
								env.c \
								cd.c \
								pipe_execve.c \
								traverse_pipe.c \
								signals.c \
								validation_input.c \
								shlvl.c \
								dollar_expansion.c \
								exit.c
MINISHELL_OBJ = $(MINISHELL_SRC:.c=.o)

# Paths
LIBFT_DIR = libft_
LIBFT = $(LIBFT_DIR)/libft.a
CC = gcc
CFLAGS = -g -Wall -Wextra -Werror -I$(LIBFT_DIR) -I$(shell brew --prefix readline)/include
LDFLAGS = -L$(LIBFT_DIR) -L$(shell brew --prefix readline)/lib -lft -lreadline
# CFLAGS = -g -Wall -Wextra -Werror -I$(LIBFT_DIR)
# LDFLAGS = -L$(LIBFT_DIR) -lft -lreadline

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

all: $(NAME)

$(NAME): $(LIBFT) $(MINISHELL_OBJ)
	@$(CC) $(MINISHELL_OBJ) $(LDFLAGS) -o $(NAME)
	@echo "$(BUILD_EMOJI) $(BLUE)Linked:$(RESET) $(NAME)"

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)
	@echo "$(BUILD_EMOJI) $(BLUE)libft built!$(RESET)"

$(MINISHELL_OBJ): %.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(BUILD_EMOJI) $(BLUE)Compiled:$(RESET) $<"

clean:
	@rm -f $(MINISHELL_OBJ)
	@$(MAKE) -C $(LIBFT_DIR) clean
	@echo "$(CLEAN_EMOJI) $(PURPLE)Object files removed!$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "$(REMOVE_EMOJI) $(RED)All files removed!$(RESET)"

re: fclean all
	@echo "$(REBUILD_EMOJI) $(YELLOW)Rebuild complete!$(RESET)"

# Test-related variables and targets
TEST_SRCS = tests/test_minishell.c
TEST_OBJS = $(TEST_SRCS:.c=.o)
TEST_EXEC = test_minishell
TEST_CFLAGS = $(CFLAGS) -DUNIT_TEST

test: $(TEST_EXEC)
	./$(TEST_EXEC)

$(TEST_EXEC): $(TEST_OBJS) $(filter-out main.o, $(MINISHELL_OBJ))
	@$(CC) $(TEST_OBJS) $(filter-out main.o, $(MINISHELL_OBJ)) $(LDFLAGS) -o $@
	@echo "$(BUILD_EMOJI) $(GREEN)Test executable built!$(RESET)"

clean_test:
	@rm -f $(TEST_OBJS) $(TEST_EXEC)
	@echo "$(CLEAN_EMOJI) $(PURPLE)Test object files removed!$(RESET)"

.PHONY: all clean fclean re test clean_test