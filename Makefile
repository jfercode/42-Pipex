# Static library name
NAME = build/pipex.a

# Compiler and rules
CC = cc
CFLAGS = -Wall -Werror -Wextra -g3 

# Directories
SRC_DIR = source
OBJ_DIR = build/obj
LIBFT_DIR = source/ft_libft
FT_PRINTF_DIR = source/ft_printf

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# # Sub-sources
LIBFT_LIB = $(LIBFT_DIR)/build/libft.a
FT_PRINTF_LIB = $(FT_PRINTF_DIR)/build/ft_printf.a

# Default rule
all: $(NAME)

$(NAME):  $(FT_PRINTF_LIB) $(LIBFT_LIB) $(OBJS)
	@mkdir -p $(OBJ_DIR)
	@echo "\033[1;32mCREATING STATIC LIBRARY $@\033[0m"
	ar rcs $@ $^

# Rule to compile .c files into .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@echo "\033[1;33mCOMPILING $<...\033[0m"
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to compile the ft_printf library
$(FT_PRINTF_LIB):
	@echo "\033[1;35mCOMPILING FT_PRINTF LIBRARY...\033[0m"
	$(MAKE) -C $(FT_PRINTF_DIR)

# Rule to compile the libft library
$(LIBFT_LIB):
	@echo "\033[1;35mCOMPILING LIBFT LIBRARY...\033[0m"
	$(MAKE) -C $(LIBFT_DIR)

# Rule to clean object files and executables
clean:
		@echo "\033[1;31mCLEANING OBJECT FILES AND EXECUTABLES...\033[0m" 
		rm -rf $(OBJ_DIR)
		rm -f ./pipex
		$(MAKE) -C $(FT_PRINTF_DIR) clean
		$(MAKE) -C $(LIBFT_DIR) clean
		@echo "\033[1;32mCLEANING DONE.\033[0m"

# Rule for full clean
fclean:	clean
		@echo "\033[1;31mDELETING FILES $(NAME)...\033[0m"
		rm -f $(NAME)
		$(MAKE) -C $(FT_PRINTF_DIR) fclean
		$(MAKE) -C $(LIBFT_DIR) fclean		
		@echo "\033[1;32mALL CLEANED UP.\033[0m" 

# Rule to rebuild the project
re:		fclean all
		@echo "\033[1;34mPROJECT REBUILD.\033[0m"

# Rule to compile and generate the executable
gen_exec:
		@echo "\033[1;36mCOMPILING AND GENERATE THE EXECUTABLE...\033[0m"
		$(CC) $(CFLAGS) -o pipex $(OBJ_DIR)/main.o $(NAME) $(FT_PRINTF_LIB) $(LIBFT_LIB)

# Valgrind command: valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes ./pipex infile cmd1 cmd2 outfile

# Phony rules
.PHONY:	all clean fclean re