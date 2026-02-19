# Phony targets to avoid conflicts with real file names
.PHONY: all clean run

# Default target: Build everything by calling the Makefiles in lib, src, and tests
all:
	$(MAKE) -C lib
	$(MAKE) -C src
	$(MAKE) -C tests

# Clean target: Clean all subdirectories
clean:
	$(MAKE) -C lib clean
	$(MAKE) -C src clean
	$(MAKE) -C tests clean

# Run target: Run both the main program and the test program
run:
	./src/program
	./tests/test_program
