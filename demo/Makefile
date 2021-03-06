CC=gcc
CFLAGS= -Wall -pedantic
VFLAGS= --leak-check=full
PROFFL=tree.c

all: goods

debug: CFLAGS += -g
debug: goods

goods: goods.o database.o item.o stack.o list.o tree.o utils.o utils_goods.o file.o action.o refmem.o listset.o queue.o
# Ovan regel kör nedan command automatiskt:
# gcc   goods.o item.o list.o tree.o utils.o utils_db.o   -o goods

refmem.o: ../src/refmem.c
	$(CC) $(CFLAGS) -c $<

listset.o: ../src/listset.c
	$(CC) $(CFLAGS) -c $<

queue.o: ../src/queue.c
	$(CC) $(CFLAGS) -c $<

goods.o: goods.c
	$(CC) $(CFLAGS) -c goods.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

utils_goods.o: utils_goods.c utils_goods.h
	$(CC) $(CFLAGS) -c utils_goods.c

database.o: database.c database.h
	$(CC) $(CFLAGS) -c database.c

item.o: item.c item.h
	$(CC) $(CFLAGS) -c item.c

stack.o: stack.c stack.h
	$(CC) $(CFLAGS) -c stack.c

list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c

tree.o: tree.c tree.h
	$(CC) $(CFLAGS) -c tree.c

file.o: file.c file.h
	$(CC) $(CFLAGS) -c file.c

action.o: action.c action.h
	$(CC) $(CFLAGS) -c action.c

########### Profiler

profiler_avl: PROFFL=avl_tree.c
profiler_avl: profiler

profiler: CLFAGS += -g -pg
profiler:
	gcc -Wall -g -pg profiling/profile.c $(PROFFL) -o profiling/profile.out

########### Tests

tests: CFLAGS += -g -lcunit --coverage #-fprofile-arcs -ftest-coverage
tests: refmem.o listset.o queue.o test_action test_stack test_item test_utils test_database test_utils_goods #test_list test_tree

test_tree:
	$(CC) $(CFLAGS) -I/usr/local/Cellar/cunit/2.1-3/include -L/usr/local/Cellar/cunit/2.1-3/lib refmem.o listset.o queue.o tree.c tests/test_tree.c -o test_tree

test_list: refmem.o
	$(CC) $(CFLAGS) -g -lcunit --coverage -I/usr/local/Cellar/cunit/2.1-3/include -L/usr/local/Cellar/cunit/2.1-3/lib refmem.o listset.o queue.o list.c tests/test_list.c -o test_list

test_stack:
	$(CC) $(CFLAGS) -I/usr/local/Cellar/cunit/2.1-3/include -L/usr/local/Cellar/cunit/2.1-3/lib refmem.o listset.o queue.o utils.c stack.c tests/test_stack.c -o test_stack

test_item:
	$(CC) $(CFLAGS) -I/usr/local/Cellar/cunit/2.1-3/include -L/usr/local/Cellar/cunit/2.1-3/lib refmem.o listset.o queue.o utils.c item.c list.c tests/test_item.c -o test_item

test_utils:
	$(CC) $(CFLAGS) -I/usr/local/Cellar/cunit/2.1-3/include -L/usr/local/Cellar/cunit/2.1-3/lib refmem.o listset.o queue.o utils.c tests/test_utils.c -o test_utils

test_utils_goods:
	$(CC) $(CFLAGS) -I/usr/local/Cellar/cunit/2.1-3/include -L/usr/local/Cellar/cunit/2.1-3/lib refmem.o listset.o queue.o utils.c utils_goods.c tests/test_utils_goods.c -o test_utils_goods

test_action:
	$(CC) $(CFLAGS) -I/usr/local/Cellar/cunit/2.1-3/include -L/usr/local/Cellar/cunit/2.1-3/lib refmem.o listset.o queue.o utils.c action.c tests/test_action.c -o test_action

test_database:
	$(CC) $(CFLAGS) -I/usr/local/Cellar/cunit/2.1-3/include -L/usr/local/Cellar/cunit/2.1-3/lib refmem.o listset.o queue.o database.c tree.c item.c action.c stack.c utils_goods.c list.c utils.c file.c tests/test_database.c -o test_database

# Phony targets

.PHONY: valgrind gdb lldb clean run test gcov

valgrind: debug goods
	valgrind $(VFLAGS) ./goods

gdb:	debug
	gdb goods

lldb:	debug
	lldb goods

clean:
	rm -f *.o
	rm -f *.out
	rm -rf *.dSYM
	rm -rf *.dtps
	rm -f *~
	rm -f goods
	rm -f test_*
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.gcov
	rm -f *.gch
	rm -f tests/*.c~
	rm -f profiling/*.out
	rm -rf profiling/*.dSYM

run:	goods
	./goods

demo: goods
	./goods < test.txt

memtest-demo: goods
	valgrind $(VFLAGS) ./goods < test.txt

test:	tests
	@echo "--------------------------------------------- RUNNING TESTS ON UTILS --------------------------------------------"
	./test_utils
	@echo "--------------------------------------------- RUNNING TESTS ON UTILS GOODS --------------------------------------"
	./test_utils_goods
	@echo "--------------------------------------------- RUNNING TESTS ON ACTION -------------------------------------------"
	./test_action
	@echo "--------------------------------------------- RUNNING TESTS ON STACK --------------------------------------------"
	./test_stack
	@echo "--------------------------------------------- RUNNING TESTS ON LIST ---------------------------------------------"
	./test_list
	@echo "--------------------------------------------- RUNNING TESTS ON TREE ---------------------------------------------"
	./test_tree
	@echo "--------------------------------------------- RUNNING TESTS ON ITEM ---------------------------------------------"
	./test_item
	@echo "--------------------------------------------- RUNNING TESTS ON DATABASE -----------------------------------------"
	./test_database
