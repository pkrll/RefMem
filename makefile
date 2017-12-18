# Compiler & compiler flags
CC=gcc
CFLAGS= -std=c11 -Wall -pedantic
TFLAGS= -std=c11 -lcunit -lm #--coverage, lm needed to link math lib
TEST=test/test_refmen

# Directories
TESTDIR=test
SOURCEDIR=src
OBJECTDIR=obj
BINARYDIR=bin

# Binary names
BINARY=refmen.out
TESTBINARY=test_refmen.out

# Source & object files
SOURCES = $(shell find $(SOURCEDIR) -type f -name '*.c')
OBJECTS = $(patsubst $(SOURCEDIR)/%.c, $(OBJECTDIR)/%.o, $(SOURCES))
SOURCES_TEST = $(shell find $(TESTDIR) -type f -name '*.c')
OBJECTS_TEST = $(patsubst $(TESTDIR)/%.c, $(OBJECTDIR)/%.o, $(SOURCES_TEST))

# Debug, profiling files & folders
DEBUG_FILES = $(shell find . -type f -name '*.gcda' -o -name '*.gcno' -o -name '*.dSYM')

# Targets

all: build

build: compile
	$(CC) $(CFLAGS) $(OBJECTS) -o $(BINARYDIR)/$(BINARY)

compile: $(OBJECTS)

compile-tests: CFLAGS +=-g
compile-tests: $(OBJECTS) $(OBJECTS_TEST)

$(OBJECTDIR)/%.o: $(SOURCEDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJECTDIR)/%.o: $(TESTDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# TEST TARGETS

test-tree: compile-tests
test-tree:
	$(CC) $(CFLAGS) -I/usr/local/Cellar/cunit/2.1-3/include -L/usr/local/Cellar/cunit/2.1-3/lib $(OBJECTDIR)/tree.o $(OBJECTDIR)/tree_test.o -o $(BINARYDIR)/test_tree $(TFLAGS)

test-refmen: compile-tests
test-refmen:
	$(CC) $(CFLAGS) -I/usr/local/Cellar/cunit/2.1-3/include -L/usr/local/Cellar/cunit/2.1-3/lib $(OBJECTDIR)/refmen.o $(OBJECTDIR)/refmen_test.o -o $(BINARYDIR)/test_refmen $(TFLAGS)

# PHONY TARGETS

.PHONY: clean test

clean:
	rm -f $(OBJECTDIR)/*.o
	rm -f $(BINARYDIR)/*
	rm -f *~
	rm -rf $(DEBUG_FILES)

test: test-refmen test-tree
	@echo "--------------------------------------------- RUNNING TESTS ON REFMEN --------------------------------------------"
	@./$(BINARYDIR)/test_refmen
	@echo "--------------------------------------------- RUNNING TESTS ON TREE   --------------------------------------------"
	@./$(BINARYDIR)/test_tree

style:
	astyle --style=google --indent=spaces=2 $(SOURCES) $(SOURCES_TEST)
