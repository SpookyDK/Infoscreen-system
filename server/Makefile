# Name of your final executable
TARGET = InfoscreenServer

# Compilers and Flags
CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -pg -I$(INC_DIR)

# Directories
SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj

# Find all .c files in the src directory
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Convert the list of .c files into a list of .o files in the obj directory
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Default rule (runs when you just type 'make')
all: $(TARGET)

# Rule to link the object files into the final executable
$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $^ -o $@
	@echo "Build successful: $(TARGET)"

# Rule to compile .c files into .o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove generated files
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
	@echo "Cleaned up build files."

# Phony targets (prevents conflicts with files named 'all' or 'clean')
.PHONY: all clean
