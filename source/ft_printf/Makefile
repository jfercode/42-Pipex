# Static library name
NAME = build/ft_printf.a

# Compiler and compiler flags
CC = cc
C_FLAGS = -Wall -Werror -Wextra -g3 -I $(HEADER_DIR)

# Directories
SRC_DIR = source
OBJ_DIR = build/obj
HEADER_DIR = /include

# Source files
SRC_FILES = $(SRC_DIR)/ft_printf.c $(SRC_DIR)/ft_printf_utils.c
OBJS = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Default rule
all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

# Rule to compile .c files into .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@echo "\033[1;33mCOMPILING $<...\033[0m"
	$(CC) $(CFLAGS) -c $< -o $@

# Clean object files and executable
clean:
	@echo "\033[1;31mCLEANING OBJECT FILES AND EXECUTABLES FROM FT_PRINTF...\033[0m" 
	rm -rf $(OBJ_DIR)
	rm -f ./test_program
	@echo "\033[32mCLEANING PRINTF DONE\033[0m"

# Full clean (including library)
fclean: clean
	rm -f $(NAME)
	@echo "\033[32mPRINTF CLEAR\033[0m"

# Rebuild the project
re: fclean all
	@echo "\033[1;34mREBUILDING FT_PRINTF LIBRARY...\033[0m"

.PHONY: all clean fclean re
