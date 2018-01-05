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
DEMODIR=demo
DEMOTESTDIR=test/demo
# Binary names
BINARY=refmem.out
TESTBINARY=test_refmem.out

# Source & object files
SOURCES = $(shell find $(SOURCEDIR) -type f -name '*.c')
OBJECTS = $(patsubst $(SOURCEDIR)/%.c, $(OBJECTDIR)/%.o, $(SOURCES))
SOURCES_TEST = $(shell find $(TESTDIR) -type f -name '*.c' -maxdepth 1)
OBJECTS_TEST = $(patsubst $(TESTDIR)/%.c, $(OBJECTDIR)/%.o, $(SOURCES_TEST))

DEMO_SOURCES = $(shell find $(DEMODIR) -type f -name '*.c')
DEMO_OBJECTS = $(patsubst $(DEMODIR)/%.c, $(OBJECTDIR)/%.o, $(SOURCES))

DEMO_SOURCES_TEST = $(shell find $(DEMOTESTDIR) -type f -name '*.c')
DEMO_OBJECTS_TEST = $(patsubst $(DEMOTESTDIR)/%.c, $(OBJECTDIR)/%.o, $(DEMO_SOURCES_TEST))

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

# $(OBJECTDIR)/%.o: $(DEMODIR)/%.c
# 	$(CC) $(CFLAGS) -c $< -o $@

$(OBJECTDIR)/%.o: $(DEMOTESTDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

compile-demo-tests: CFLAGS +=-g
compile-demo-tests: $(DEMO_OBJECTS_TEST)

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

# TEST TARGETS (FOR DEMO)

test-action: compile-tests $(OBJECTDIR)/test_action.o
test-action:
	$(CC) $(CFLAGS) -I/usr/local/Cellar/cunit/2.1-3/include -L/usr/local/Cellar/cunit/2.1-3/lib $(OBJECTDIR)/refmem.o $(OBJECTDIR)/queue.o $(OBJECTDIR)/listset.o demo/utils.c demo/action.c $(OBJECTDIR)/test_action.o -o $(BINARYDIR)/test_action $(TFLAGS)

test-database: compile-tests $(OBJECTDIR)/test_database.o
test-database:
	$(CC) $(CFLAGS) -I/usr/local/Cellar/cunit/2.1-3/include -L/usr/local/Cellar/cunit/2.1-3/lib $(OBJECTDIR)/refmem.o $(OBJECTDIR)/queue.o $(OBJECTDIR)/listset.o demo/utils.c demo/item.c demo/database.c $(OBJECTDIR)/test_database.o -o $(BINARYDIR)/test_database $(TFLAGS)

test-item: compile-tests $(OBJECTDIR)/test_item.o
test-item:
	$(CC) $(CFLAGS) -I/usr/local/Cellar/cunit/2.1-3/include -L/usr/local/Cellar/cunit/2.1-3/lib $(OBJECTDIR)/refmem.o $(OBJECTDIR)/queue.o $(OBJECTDIR)/listset.o demo/utils.c demo/item.c demo/list.c $(OBJECTDIR)/test_item.o -o $(BINARYDIR)/test_item $(TFLAGS)

test-list: compile-tests compile-demo-tests
test-list:
	$(CC) $(CFLAGS) -g -I/usr/local/Cellar/cunit/2.1-3/include -L/usr/local/Cellar/cunit/2.1-3/lib $(OBJECTDIR)/refmem.o $(OBJECTDIR)/queue.o $(OBJECTDIR)/listset.o demo/utils.c demo/list.c $(OBJECTDIR)/test_list.o -o $(BINARYDIR)/test_list $(TFLAGS)

test-stack: compile-tests $(OBJECTDIR)/test_stack.o
test-stack:
	$(CC) $(CFLAGS) -I/usr/local/Cellar/cunit/2.1-3/include -L/usr/local/Cellar/cunit/2.1-3/lib $(OBJECTDIR)/refmem.o $(OBJECTDIR)/queue.o $(OBJECTDIR)/listset.o demo/utils.c demo/stack.c $(OBJECTDIR)/test_stack.o -o $(BINARYDIR)/test_stack $(TFLAGS)

test-tree: compile-tests $(OBJECTDIR)/test_tree.o
test-tree:
	$(CC) $(CFLAGS) -I/usr/local/Cellar/cunit/2.1-3/include -L/usr/local/Cellar/cunit/2.1-3/lib $(OBJECTDIR)/refmem.o $(OBJECTDIR)/queue.o $(OBJECTDIR)/listset.o demo/utils.c demo/tree.c $(OBJECTDIR)/test_tree.o -o $(BINARYDIR)/test_tree $(TFLAGS)

test-utils-goods: compile-tests $(OBJECTDIR)/test_utils_goods.o
test-utils-goods:
	$(CC) $(CFLAGS) -I/usr/local/Cellar/cunit/2.1-3/include -L/usr/local/Cellar/cunit/2.1-3/lib $(OBJECTDIR)/refmem.o $(OBJECTDIR)/queue.o $(OBJECTDIR)/listset.o demo/utils.c demo/utils_goods.c $(OBJECTDIR)/test_utils_goods.o -o $(BINARYDIR)/test_utils_goods $(TFLAGS)

test-utils: compile-tests $(OBJECTDIR)/test_utils.o
test-utils:
	$(CC) $(CFLAGS) -I/usr/local/Cellar/cunit/2.1-3/include -L/usr/local/Cellar/cunit/2.1-3/lib $(OBJECTDIR)/refmem.o $(OBJECTDIR)/queue.o $(OBJECTDIR)/listset.o demo/utils.c $(OBJECTDIR)/test_utils.o -o $(BINARYDIR)/test_utils $(TFLAGS)

# PHONY TARGETS

.PHONY: clean test

clean:
	rm -f $(OBJECTDIR)/*.o
	rm -rf $(BINARYDIR)/*
	rm -f *~
	rm -rf $(DEBUG_FILES)

test: test-refmem test-queue test-listset test-action
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
