# Compiler & compiler flags
CC=gcc
CFLAGS= -Wall -pedantic
TFLAGS= -lcunit -lm #--coverage, lm needed to link math lib
TEST=test/test_refmem

# Directories
TESTDIR=test
SOURCEDIR=src
OBJECTDIR=obj
BINARYDIR=bin

# Binary names
BINARY=refmem.out
TESTBINARY=test_refmem.out

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


test-queue: compile-tests
test-queue:
	$(CC) $(CFLAGS) -I/usr/local/Cellar/cunit/2.1-3/include -L/usr/local/Cellar/cunit/2.1-3/lib $(OBJECTDIR)/queue.o $(OBJECTDIR)/queue_test.o -o $(BINARYDIR)/test_queue $(TFLAGS)


test-refmem: compile-tests
test-refmem:
	$(CC) $(CFLAGS) -I/usr/local/Cellar/cunit/2.1-3/include -L/usr/local/Cellar/cunit/2.1-3/lib $(OBJECTDIR)/listset.o $(OBJECTDIR)/queue.o  $(OBJECTDIR)/refmem.o $(OBJECTDIR)/refmem_test.o -o $(BINARYDIR)/test_refmem $(TFLAGS)

test-listset: compile-tests
test-listset:
	$(CC) $(CFLAGS) -I/usr/local/Cellar/cunit/2.1-3/include -L/usr/local/Cellar/cunit/2.1-3/lib $(OBJECTDIR)/listset.o $(OBJECTDIR)/listset_test.o -o $(BINARYDIR)/test_listset $(TFLAGS)

# PHONY TARGETS

.PHONY: clean test

clean:
	rm -f $(OBJECTDIR)/*.o
	rm -f $(BINARYDIR)/*
	rm -f *~
	rm -rf $(DEBUG_FILES)

test: test-refmem test-queue test-listset
	@echo "--------------------------------------------- RUNNING TESTS ON refmem --------------------------------------------"
	@./$(BINARYDIR)/test_refmem
	@echo "--------------------------------------------- RUNNING TESTS ON queue   --------------------------------------------"
	@./$(BINARYDIR)/test_queue
	@echo "--------------------------------------------- RUNNING TESTS ON LIST   --------------------------------------------"
	@./$(BINARYDIR)/test_listset

memtest: test-refmem
	valgrind --leak-check=full --track-origins=yes --show-possibly-lost=yes bin/test_refmem

memtest-queue: test-queue
	valgrind --leak-check=full --track-origins=yes --show-possibly-lost=yes bin/test_queue

style:
	astyle --style=google --indent=spaces=2 --indent-continuation=2 $(SOURCES) $(SOURCES_TEST)
