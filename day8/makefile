# Makefile

# Compiler and flags
CC = gcc
CFLAGS_RELEASE = -Wall -Wextra -std=c99 -I../../include
CFLAGS_DEBUG = -Wall -Wextra -std=c99 -g -I../../include

# Directories
SRC_DIR = ../../src
BUILD_DIR = ./build

# Mode (default is release)
MODE ?= RELEASE
CFLAGS = $(CFLAGS_$(MODE))

# Files
ARRAY2D_SRC = $(SRC_DIR)/array2d.c
ARRAY_SRC = $(SRC_DIR)/array.c
DAY_SRC = day8.c
OBJ_FILES = $(BUILD_DIR)/array2d.o $(BUILD_DIR)/day8.o $(BUILD_DIR)/array.o
EXECUTABLE = day8

# Rules
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ_FILES)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean debug
