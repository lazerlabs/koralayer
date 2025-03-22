# KoraLayer Makefile

# Detect platform
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    PLATFORM = linux
    PLATFORM_SRC = src/linux/syscalls_linux.c
endif
ifeq ($(UNAME_S),Darwin)
    PLATFORM = macos
    PLATFORM_SRC = src/macos/syscalls_macos.c
endif
ifeq ($(OS),Windows_NT)
    PLATFORM = windows
    PLATFORM_SRC = src/windows/syscalls_windows.c
endif

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Werror -fPIC -Iinclude
LDFLAGS = -shared

# Source files
COMMON_SRC = src/syscalls.c
SRC = $(COMMON_SRC) $(PLATFORM_SRC)

# Output files
LIB_NAME = libkora_sys
LIB_STATIC = $(LIB_NAME).a
ifeq ($(PLATFORM),windows)
    LIB_SHARED = $(LIB_NAME).dll
else
    LIB_SHARED = $(LIB_NAME).so
endif

# Build directories
BUILD_DIR = build
LIB_DIR = lib
TEST_DIR = tests
OBJ_DIR = $(BUILD_DIR)/obj

# Objects
OBJECTS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC))
TEST_OBJECTS = $(patsubst $(TEST_DIR)/%.c,$(OBJ_DIR)/$(TEST_DIR)/%.o,$(wildcard $(TEST_DIR)/*.c))
TEST_BINS = $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/$(TEST_DIR)/%,$(wildcard $(TEST_DIR)/*.c))

# Targets
all: dirs $(LIB_DIR)/$(LIB_SHARED) $(LIB_DIR)/$(LIB_STATIC) tests

dirs:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(OBJ_DIR)/src/linux
	mkdir -p $(OBJ_DIR)/src/macos
	mkdir -p $(OBJ_DIR)/src/windows
	mkdir -p $(OBJ_DIR)/$(TEST_DIR)
	mkdir -p $(LIB_DIR)
	mkdir -p $(BUILD_DIR)/$(TEST_DIR)

# Compile .c to .o
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Build shared library
$(LIB_DIR)/$(LIB_SHARED): $(OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) -o $@ $^

# Build static library
$(LIB_DIR)/$(LIB_STATIC): $(OBJECTS)
	@mkdir -p $(dir $@)
	ar rcs $@ $^

# Build tests
tests: $(TEST_BINS)

$(BUILD_DIR)/$(TEST_DIR)/%: $(OBJ_DIR)/$(TEST_DIR)/%.o $(LIB_DIR)/$(LIB_STATIC)
	$(CC) -o $@ $< -L$(LIB_DIR) -lkora_sys

# Run tests
run-tests: tests
	@echo "Running tests..."
	@for test in $(TEST_BINS); do \
		echo "Running $$test"; \
		LD_LIBRARY_PATH=$(LIB_DIR) $$test; \
	done

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(LIB_DIR)

.PHONY: all clean tests run-tests dirs 