# High level design
This document explains our implementation of a reference counting garbage collector.

## Static variables
The refmem module contains five static variables:
- ``cascade_limit`` determines how many objects can be free’d consecutively.
- ``cascade_counter`` keeps track of how many objects has been free’d since last allocation.
- ``obj_register`` is a queue containing the overflow resulting from ``cascade_counter`` reaching ``cascade_limit``.
  When that happens deallocate will add every object it’s meant to free to this queue.
- ``destr_register`` is a linked list containing all destructor functions passed to allocate.
- ``size_register`` is a linked list containing all different sizes passed to allocate.

## Structs

![record](HeaderDiagram.png?)

To keep track of each objects reference count some overhead is necessary which is contained in a structure called ``header``.

The ``header`` contains three unsigned shorts:
- ``reference_count`` for keeping track of the number of references to this object.
- ``destr_index`` is the index of the objects destructor function in ``destr_register``.
- ``size_index`` is the index of the objects size in ``size_register``.

## Main functions
**allocate**(*size*, *destructor*)
- Call ``prepare_for_allocation``
- Create a new record with a ``reference_count`` of 0.
- Add the destructor to ``destr_register``, save the index to records ``destr_index``.
- Add the size to ``size_register``, save the index to records ``size_index``.
- Return a pointer to the part of the allocated memory where the overhead ends.

**retain**(*object*)
- Increment an objects ``reference_count``.

**release**(*object*)
- Decrement an objects ``reference_count``.
- Call ``deallocate`` on object if ``reference_count`` reaches 0.

**deallocate**(*object*)
- Call the object's destructor.
- Free the object and increment ``cascade_counter``, if ``cascade_limit``. Otherwise, add the object to ``obj_register``.

**prepare_for_allocation(size_t bytes)**
- Create ``destr_register``, if  needed.
- Create ``size_register``, if  needed.
- Invoke ``clear_obj_register``.

**clear_obj_register**(*request*)
- Set ``cascade_counter`` to 0.
- Free the objects in ``obj_register``, with respect to ``cascade_limit``.
- If the amount of bytes requested for allocation (``request``) is larger than the amount of memory free'd, continue to free objects until the requested number of bytes have been free'd or ``obj_register`` is empty.

**cleanup**
- Clear ``obj_register`` completely.

**shutdown**
- Run ``cleanup`` and delete ``obj_register``, ``destr_register`` and ``size_register``.

## Graphical example

![record](record.png?)

The rectangles represent our structures. Size is the amount of bytes the user wants to allocate, and the destructor is a function pointer supplied by the user.

Within our module, we have from the first allocation two lists that start as empty: the destructor list and the size list, in which the system saves the different destructors and object sizes. This allows us to reduce the overhead of the ``header`` structure, by only storing list indices, instead of pointers.

 The cascade limit is by default set to 1000 but can easily be changed by the user.

## Overhead

> A program that uses B bytes of memory with malloc() / calloc() may only use k * B bytes using your system. Additionally, k may not be larger than 2.

Our ``header`` structure contains 3 unsigned shorts with a total size of 6 bytes. These contain the required information for the object: its counter, index to its destructor function and the index to the number of bytes allocated for that object.

The following graphs describe the memory usage of our system.

There are two alternatives: a worst possible memory usage and an expected memory usage, which is represented by a program using 2^16 or 2^4 diffreent allocated sizes and destructor functions.

The graphs describes how many objects that are required to be allocated before the program goes below k = 2, represented by the x = 1 line. The graphs describes the overhead changing over time. Starting with zero allocated objects and the amount of required memory to use our solution.

![record](graph2.png?)
Worst case requires nearly 1 000 000 objects to be allocated.
![record](graph1.png?)
Expected case requires nearly 300 objects to be allocated.

We can assume that we achieve the memory requirements by studying the graphs.
