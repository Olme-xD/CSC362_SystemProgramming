# Variables
CC = gcc
CFLAGS = -Wall

# Phony targets to avoid conflicts
.PHONY: all clean run

# Default target: Build the object file for the math library
all:
	math_lib.o

# Rule to build math_lib.o from math_lib.c
math_lib.o: math_lib.c
	$(CC) $(CFLAGS) -c math_lib.c -o math_lib.o

# Clean target: Remove the object file
clean:
	rm -f math_lib.o
