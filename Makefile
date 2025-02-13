# Executable name
NAME = fdf

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -I. -I GNL -I$(MLX_INC)

# MiniLibX paths (macOS)
MLX_DIR = minilibx_macos
MLX_LIB = $(MLX_DIR)/libmlx.a
MLX_INC = $(MLX_DIR)

# Libraries for macOS (MiniLibX требует фреймворки OpenGL и AppKit)
LIBS = -L$(MLX_DIR) -lmlx -framework OpenGL -framework AppKit

# Source files
SRC = main.c points.c util.c
GNL_SRC = GNL/get_next_line.c GNL/get_next_line_utils.c
OBJ = $(SRC:.c=.o) $(GNL_SRC:.c=.o)

# Header file
HEADERS = fdf.h GNL/get_next_line.h

# Rule to build the executable
$(NAME): $(OBJ) $(MLX_LIB)
	$(CC) $(OBJ) $(LIBS) -o $(NAME)

# Ensure objects are rebuilt if header files change
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Build MiniLibX (для macOS используется make в папке minilibx_macos)
$(MLX_LIB):
	$(MAKE) -C $(MLX_DIR)

# Cleaning
clean:
	rm -f $(OBJ) $(NAME)
	$(MAKE) -C $(MLX_DIR) clean

fclean: clean
	rm -f $(NAME)

# Rebuild project
re: fclean $(NAME)

.PHONY: all clean fclean re
