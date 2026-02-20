# Variables
CC = gcc
CFLAGS = -Wall
LIB_OBJ = ../lib/math_lib.o
TARGET = test_program

# Phony targets to avoid conflicts
.PHONY: all clean run

# Default target: Build the test program
all: $(TARGET)

# Rule to link the object file and create the test program executable
$(TARGET): test_main.o $(LIB_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to compile test_main.o
test_main.o: test_main.c
	$(CC) $(CFLAGS) -c test_main.c -o test_main.o

# Clean target: Remove the object file and the test program executable
clean:
	rm -f test_main.o $(TARGET)
