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
SOURCES_TEST = $(shell find $(TESTDIR) -maxdepth 1 -type f -name '*.c')
OBJECTS_TEST = $(patsubst $(TESTDIR)/%.c, $(OBJECTDIR)/%.o, $(SOURCES_TEST))

DEMO_SOURCES = $(shell find $(DEMODIR) -maxdepth 1 -type f -name '*.c')
DEMO_OBJECTS = $(patsubst $(DEMODIR)/%.c, $(OBJECTDIR)/%.o, $(DEMO_SOURCES))

DEMO_SOURCES_TEST = $(shell find $(DEMOTESTDIR) -type f -name '*.c')
DEMO_OBJECTS_TEST = $(patsubst $(DEMOTESTDIR)/%.c, $(OBJECTDIR)/%.o, $(DEMO_SOURCES_TEST))

# Debug, profiling files & folders
DEBUG_FILES = $(shell find . -type f -name '*.gcda' -o -name '*.gcno' -o -name '*.dSYM')

# Targets

all: compile

compile: $(OBJECTS)

compile-tests: CFLAGS +=-g
compile-tests: $(OBJECTS) $(OBJECTS_TEST)

$(OBJECTDIR)/%.o: $(SOURCEDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJECTDIR)/%.o: $(TESTDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJECTDIR)/%.o: $(DEMODIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJECTDIR)/%.o: $(DEMOTESTDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

compile-demo-tests: CFLAGS +=-g
compile-demo-tests: $(DEMO_OBJECTS) $(DEMO_OBJECTS_TEST)

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

test-action: compile-tests compile-demo-tests
test-action:
	$(CC) $(CFLAGS) -I/usr/local/Cellar/cunit/2.1-3/include -L/usr/local/Cellar/cunit/2.1-3/lib $(OBJECTDIR)/refmem.o $(OBJECTDIR)/queue.o $(OBJECTDIR)/listset.o $(OBJECTDIR)/utils.o $(OBJECTDIR)/action.o $(OBJECTDIR)/test_action.o -o $(BINARYDIR)/test_action $(TFLAGS)


test-database: compile-tests compile-demo-tests
test-database:
	$(CC) $(CFLAGS) -I/usr/local/Cellar/cunit/2.1-3/include -L/usr/local/Cellar/cunit/2.1-3/lib $(OBJECTDIR)/refmem.o $(OBJECTDIR)/queue.o $(OBJECTDIR)/listset.o $(OBJECTDIR)/utils.o $(OBJECTDIR)/item.o $(OBJECTDIR)/action.o $(OBJECTDIR)/list.o $(OBJECTDIR)/tree.o $(OBJECTDIR)/utils_goods.o $(OBJECTDIR)/file.o $(OBJECTDIR)/stack.o $(OBJECTDIR)/database.o $(OBJECTDIR)/test_database.o -o $(BINARYDIR)/test_database $(TFLAGS)


test-item: compile-tests compile-demo-tests
test-item:
	$(CC) $(CFLAGS) -I/usr/local/Cellar/cunit/2.1-3/include -L/usr/local/Cellar/cunit/2.1-3/lib $(OBJECTDIR)/refmem.o $(OBJECTDIR)/queue.o $(OBJECTDIR)/listset.o $(OBJECTDIR)/utils.o $(OBJECTDIR)/item.o $(OBJECTDIR)/list.o $(OBJECTDIR)/test_item.o -o $(BINARYDIR)/test_item $(TFLAGS)


test-list: compile-tests compile-demo-tests
test-list:
	$(CC) $(CFLAGS) -g -I/usr/local/Cellar/cunit/2.1-3/include -L/usr/local/Cellar/cunit/2.1-3/lib $(OBJECTDIR)/refmem.o $(OBJECTDIR)/queue.o $(OBJECTDIR)/listset.o $(OBJECTDIR)/utils.o $(OBJECTDIR)/list.o $(OBJECTDIR)/test_list.o -o $(BINARYDIR)/test_list $(TFLAGS)


test-stack: compile-tests compile-demo-tests
test-stack:
	$(CC) $(CFLAGS) -I/usr/local/Cellar/cunit/2.1-3/include -L/usr/local/Cellar/cunit/2.1-3/lib $(OBJECTDIR)/refmem.o $(OBJECTDIR)/queue.o $(OBJECTDIR)/listset.o $(OBJECTDIR)/utils.o $(OBJECTDIR)/stack.o $(OBJECTDIR)/test_stack.o -o $(BINARYDIR)/test_stack $(TFLAGS)


test-tree: compile-tests compile-demo-tests
test-tree:
	$(CC) $(CFLAGS) -I/usr/local/Cellar/cunit/2.1-3/include -L/usr/local/Cellar/cunit/2.1-3/lib $(OBJECTDIR)/refmem.o $(OBJECTDIR)/queue.o $(OBJECTDIR)/listset.o $(OBJECTDIR)/utils.o $(OBJECTDIR)/tree.o $(OBJECTDIR)/test_tree.o -o $(BINARYDIR)/test_tree $(TFLAGS)

test-utils-goods: compile-tests compile-demo-tests
test-utils-goods:
	$(CC) $(CFLAGS) -I/usr/local/Cellar/cunit/2.1-3/include -L/usr/local/Cellar/cunit/2.1-3/lib $(OBJECTDIR)/refmem.o $(OBJECTDIR)/queue.o $(OBJECTDIR)/listset.o $(OBJECTDIR)/utils.o $(OBJECTDIR)/utils_goods.o $(OBJECTDIR)/test_utils_goods.o -o $(BINARYDIR)/test_utils_goods $(TFLAGS)

test-utils: compile-tests compile-demo-tests
test-utils:
	$(CC) $(CFLAGS) -I/usr/local/Cellar/cunit/2.1-3/include -L/usr/local/Cellar/cunit/2.1-3/lib $(OBJECTDIR)/refmem.o $(OBJECTDIR)/queue.o $(OBJECTDIR)/listset.o $(OBJECTDIR)/utils.o $(OBJECTDIR)/test_utils.o -o $(BINARYDIR)/test_utils $(TFLAGS)


# TEST TARGETS INDIVIDUAL RUN

test-queue-run: test-queue
	@echo "--------------------------------------------- RUNNING TESTS ON queue   --------------------------------------------"
	@./$(BINARYDIR)/test_queue

test-refmem-run: test-refmem
	@echo "--------------------------------------------- RUNNING TESTS ON refmem --------------------------------------------"
		@./$(BINARYDIR)/test_refmem

test-listset-run: test-listset
	@echo "--------------------------------------------- RUNNING TESTS ON LIST   --------------------------------------------"
	@./$(BINARYDIR)/test_listset

# TEST TARGETS INDIVIDUAL RUN (FOR DEMO)

test-action-run: test-action
	@echo "--------------------------------------------- RUNNING TESTS ON ACTION -------------------------------------------"
	@./$(BINARYDIR)/test_action

test-database-run: test-database
	@echo "--------------------------------------------- RUNNING TESTS ON DATABASE -----------------------------------------"
	@./$(BINARYDIR)/test_database

test-item-run: test-item
	@echo "--------------------------------------------- RUNNING TESTS ON ITEM ---------------------------------------------"
	@./$(BINARYDIR)/test_item

test-list-run: test-list
	@echo "--------------------------------------------- RUNNING TESTS ON LIST ---------------------------------------------"
	@./$(BINARYDIR)/test_list

test-stack-run: test-stack
	@echo "--------------------------------------------- RUNNING TESTS ON STACK --------------------------------------------"
	@./$(BINARYDIR)/test_stack

test-tree-run: test-tree
	@echo "--------------------------------------------- RUNNING TESTS ON TREE ---------------------------------------------"
	@./$(BINARYDIR)/test_tree

test-utils-goods-run: test-utils-goods
	@echo "--------------------------------------------- RUNNING TESTS ON UTILS GOODS --------------------------------------"
	@./$(BINARYDIR)/test_utils_goods

test-utils-run: test-utils
	@echo "--------------------------------------------- RUNNING TESTS ON UTILS --------------------------------------------"
	@./$(BINARYDIR)/test_utils

# PHONY TARGETS

.PHONY: clean test demo memtest-demo

clean:
	rm -f $(OBJECTDIR)/*.o
	rm -rf $(BINARYDIR)/*
	rm -f *~
	rm -f $(DEBUG_FILES)
	rm -f $(OBJECTDIR)/*.gcno
	rm -f $(OBJECTDIR)/*.gcda
	rm -rf $(DEMODIR)/*.o
	rm -rf $(DEMODIR)/goods
	rm -rf ./out
	rm -f coverage.info

test: test-refmem-run test-queue-run test-listset-run

test-demo: test-action-run test-stack-run test-utils-run test-utils-goods-run test-tree-run test-list-run test-item-run test-database-run

memtest: test-refmem test-queue test-list
	valgrind --leak-check=full --track-origins=yes --show-possibly-lost=yes bin/test_refmem
	valgrind --leak-check=full --track-origins=yes --show-possibly-lost=yes bin/test_queue
	valgrind --leak-check=full --track-origins=yes --show-possibly-lost=yes bin/test_list

memtest-demo: test-action test-stack test-utils test-utils-goods test-tree test-list test-item test-database
	valgrind --leak-check=full --track-origins=yes --show-possibly-lost=yes bin/test_action
	valgrind --leak-check=full --track-origins=yes --show-possibly-lost=yes bin/test_stack
	valgrind --leak-check=full --track-origins=yes --show-possibly-lost=yes bin/test_utils
	valgrind --leak-check=full --track-origins=yes --show-possibly-lost=yes bin/test_utils_goods
	valgrind --leak-check=full --track-origins=yes --show-possibly-lost=yes bin/test_tree
	valgrind --leak-check=full --track-origins=yes --show-possibly-lost=yes bin/test_list
	valgrind --leak-check=full --track-origins=yes --show-possibly-lost=yes bin/test_item
	valgrind --leak-check=full --track-origins=yes --show-possibly-lost=yes bin/test_database

run:
	cd demo; make run

mem-run:
	cd demo; make valgrind

demo:
	make -C demo demo

mem-demo:
	make -C demo memtest-demo

style:
	astyle --style=google --indent=spaces=2 --indent-continuation=2 $(SOURCES) $(SOURCES_TEST)

# GCOV - requires LCOV installed

compile-gcov: CFLAGS +=-fprofile-arcs -ftest-coverage
compile-gcov: compile-tests test
compile-gcov:
		cd bin
		$(CC) $(CFLAGS) -I/usr/local/Cellar/cunit/2.1-3/include -L/usr/local/Cellar/cunit/2.1-3/lib $(OBJECTDIR)/listset.o $(OBJECTDIR)/queue.o  $(OBJECTDIR)/refmem.o $(OBJECTDIR)/refmem_test.o -o $(BINARYDIR)/test_refmem $(TFLAGS)
		lcov --capture --directory ./obj --output-file coverage.info
		genhtml coverage.info --output-directory out
