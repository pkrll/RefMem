# Compiler & compiler flags
CC=gcc
CFLAGS=-Wall -pedantic
TFLAGS=-g -lcunit# --coverage

# Directories
TESTDIR=test
SOURCEDIR=src
OBJECTDIR=obj
BINARYDIR=bin

# Binary names
BINARY=refmen.out
TESTBINARY=refmen_test.out

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

linux-test:compile
	gcc -o refmen_test

build-tests: $(OBJECTS) $(OBJECTS_TEST)
	$(CC) $(CFLAGS) $(TFLAGS) -I/usr/local/Cellar/cunit/2.1-3/include -L/usr/local/Cellar/cunit/2.1-3/lib $(OBJECTS_TEST) $(OBJECTS) -o $(BINARYDIR)/$(TESTBINARY)

$(OBJECTDIR)/%.o: $(SOURCEDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJECTDIR)/%.o: $(TESTDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# PHONY TARGETS

.PHONY: clean test

clean:
	rm -f $(OBJECTDIR)/*.o
	rm -f $(BINARYDIR)/*.out
	rm -f *~

# rm -f $(DEBUG_FILES)

test: build-tests
	./$(BINARYDIR)/$(TESTBINARY)

style:
	AStyle --style=google --indent=spaces=2 $(SOURCES) $(SOURCES_TEST)
