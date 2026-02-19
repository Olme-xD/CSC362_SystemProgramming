# Variables
CC = gcc
CFLAGS = -Wall
OBJ = test_main.o
TARGET = test_program

ifeq ($(filter debug,$(MAKECMDGOALS)),debug)
	CFLAGS += -DDEBUG -g
endif
debug: all

# Phony targets
.PHONY: clean all debug

# Default target
all: $(TARGET)

# Rule to build program
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Pattern rule for object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(OBJ) $(TARGET)

# Run target (Optional)

