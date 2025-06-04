# Makefile

# Compiler and flags
CC = gcc
CFLAGS_RELEASE = -Wall -Wextra -std=c99 -I$(HOME)/code/c/include
CFLAGS_DEBUG = -Wall -Wextra -std=c99 -g -I$(HOME)/code/c/include

# Directories
SRC_DIR = $(HOME)/code/c/src
BUILD_DIR = ./build
LIB_DIR = $(HOME)/code/c/lib

# Mode (default is release)
MODE ?= RELEASE
CFLAGS = $(CFLAGS_$(MODE))

# Files
SRC_FILES = $(wildcard *.c)
OBJ_FILES = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRC_FILES))
EXECUTABLE = $(patsubst %.c, %, $(SRC_FILES))
LIBDS = $(LIB_DIR)/libds.a

# Rules
all: $(EXECUTABLE)

%: %.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBDS)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean debug
