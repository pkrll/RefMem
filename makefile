CC=gcc
CFLAGS=-Wall -pedantic

TESTDIR=test
SOURCEDIR=src
OBJECTDIR=obj
BINARY=refmen

TESTS = $(shell find $(TESTDIR) -type f -name '*.c')
SOURCES = $(shell find $(SOURCEDIR) -type f -name '*.c')
OBJECTS = $(patsubst $(SOURCEDIR)/%.c, $(OBJECTDIR)/%.o, $(SOURCES))

TESTSOBJECTS = $(patsubst $(TESTDIR)/%.c, $(TESTDIR)/%.o, $(TESTS))

all: build

build: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(BINARY)

$(OBJECTDIR)/%.o: $(SOURCEDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TESTDIR)/%.o: $(m)/%.c
	$(CC) $(CFLAGS) -lcunit -c $< -o $@

test: CFLAGS += -g  #--coverage

test: $(OBJECTS) $(TESTSOBJECTS)
	$(CC) $(CFLAGS) -lcunit -I/usr/local/Cellar/cunit/2.1-3/include -L/usr/local/Cellar/cunit/2.1-3/lib $(TESTSOBJECTS) $(OBJECTS) -o $(TESTDIR)/refmen_test

.PHONY: clean

clean:
	rm -f $(OBJECTS)
	rm -f $(TESTSOBJECTS)
	rm -f refmen
	rm -f $(TESTDIR)/refmen_test
	rm -f *~
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.gcov
	rm -f *.gch
	rm -f $(TESTDIR)/*.gcda
	rm -f $(TESTDIR)/*.gcno
	rm -f $(TESTDIR)/*.gcov
	rm -f $(TESTDIR)/*.gch
	rm -f $(OBJECTDIR)/*.gcda
	rm -f $(OBJECTDIR)/*.gcno
	rm -f $(OBJECTDIR)/*.gcov
	rm -f $(OBJECTDIR)/*.gch

runtest:
	./$(TESTDIR)/refmen_test
