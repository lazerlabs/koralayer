CC = gcc
CFLAGS = -Wall -Wextra -I../include -g
LDFLAGS = -L../lib -lkora_sys

# Get all source files
SRC = test_io.c test_dir.c test_fileinfo.c

# Generate object files from source files
OBJ = $(SRC:.c=.o)

# Generate executable names from source files
PROGS = $(SRC:.c=)

all: $(PROGS)

# Rule to link each program
%: %.o
	$(CC) -o $@ $< $(LDFLAGS)

# Rule to compile each source file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(PROGS)

run-tests: all
	@echo "Running tests..."
	@for prog in $(PROGS); do \
		echo "\n>>> Running $$prog <<<"; \
		./$$prog; \
		if [ $$? -ne 0 ]; then \
			echo "$$prog FAILED!"; \
			exit 1; \
		fi; \
	done
	@echo "\nAll tests passed!" 