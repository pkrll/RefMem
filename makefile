CC=gcc
CFLAGS=-Wall -pedantic

SOURCEDIR=src
OBJECTDIR=obj
BINARY=memref

SOURCES = $(shell find $(SOURCEDIR) -type f -name '*.c')
OBJECTS = $(patsubst $(SOURCEDIR)/%.c, $(OBJECTDIR)/%.o, $(SOURCES))

all: build

build: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(BINARY)

$(OBJECTDIR)/%.o: $(SOURCEDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(OBJECTS)
	rm -f memref
	rm -f *~
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.gcov
	rm -f *.gch
