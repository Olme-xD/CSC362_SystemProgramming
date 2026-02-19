# Variables
CC = gcc
CFLAGS = -Wall
OBJ = main.o utils.o
LIB_OBJ = ../lib/math_lib.o
TARGET = program

# Phony targets to avoid conflicts
.PHONY: all clean run

# Default target: Build the main program
all: $(TARGET)

# Rule to link the object files and create the program executable
$(TARGET): main.o $(LIB_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Rules to compile main.o and utils.o

# Clean target: Remove the object files and the program executable
clean:
	rm -f main.o $(TARGET)
