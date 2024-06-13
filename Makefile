NAME = minishell
MINISHELL_SRC = lexer.c main.c run_commandline.c parse_command_tree.c
MINISHELL_OBJ = $(MINISHELL_SRC:.c=.o)

# Paths
LIBFT_DIR = libft_
LIBFT = $(LIBFT_DIR)/libft.a
CC = cc
CFLAGS = -g -Wall -Wextra -Werror -I$(LIBFT_DIR)

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
	@$(CC) $(MINISHELL_OBJ) -L$(LIBFT_DIR) -lft -lreadline -o $(NAME)
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

.PHONY: all clean fclean re
