# Göktytorna

Refmem is a reference counting library for C, created as student project for the course Imperative and Object-Oriented Programming Methodology (1DL221) at Uppsala university.

## Building & testing

### Compiling the library
To compile the library, type ``make compile`` or simply ``make`` in the terminal:

```bash
$ make compile
```

### Run the demo

The demo can be executed with ``make run`` -- or, if you'd like to run it through *Valgrind*, ``make mem-demo``.

```bash
$ make run        # <-- Without Valgrind
$ make mem-demo   # <-- With Valgrind
```

### Running the tests

The ``make test`` rule will execute all unit tests for the library itself, while ``make test-demo`` executes the integration tests.

```bash
$ make test       # <-- Unit tests for Refmem
$ make test-demo  # <-- Integration tests
```

These tests can also be executed with Valgrind:

```bash
$ make memtest
$ make memtest-demo
```

Code coverage tests are are also available (**requires the tool lcov**):

```
make compile-gcov
```

The coverage information will be placed in the folder ``out``.
