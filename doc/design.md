# High level design
This document explains our implementation of a reference counting garbage collector.

## Static variables
The refmem module contains five static variables:
- ``cascade_limit`` determines how many objects can be free’d consecutively.
- ``cascade_counter`` keeps track of how many objects has been free’d since last allocation.
- ``mem_register`` is a queue containing the overflow resulting from ``cascade_counter`` reaching ``cascade_limit``. 
  When that happens deallocate will add every object it’s meant to free to this queue.
- ``destr_register`` is a linked list containing all destructor functions passed to allocate.
- ``size_register`` is a linked list containing all different sizes passed to allocate.

## Structs

![record](HeaderDiagram.png?)

To keep track of each objects reference count some overhead is necessary which is contained in what we call a record. 
This contains three unsigned shorts: 
- ``reference_count`` for keeping track of the number of references to this object.
- ``destr_index`` is the index of the objects destructor function in ``destr_register``.
- ``size_index`` is the index of the objects size in ``size_register``.

## Main functions
**Allocate**(*size*, *destructor*)
- Call ``clear_mem_register``
- Create a new record with a ``reference_count`` of 0.
- Add the destructor to ``destr_register``, save the index to records ``destr_index``.
- Add the size to ``size_register``, save the index to records ``size_index``.
- Return a pointer to the part of the allocated memory where the overhead ends.

**Retain**(*object*)
- Increment an objects ``reference_count``.

**Release**(*object*)
- Decrement an objects ``reference_count``.
- Call ``deallocate`` on object if ``reference_count`` becomes 0.

**Deallocate**(*object*)
- Call objects destructor.
- Free if ``cascade_limit`` not reached, add to ``mem_register`` and increment ``cascade_counter`` otherwise.

**Clear_mem_register**(*request*)
- Set ``cascade_counter`` to 0.
- Free the first object in ``mem_register`` either ``cascade_limit`` number of times or until the sum of the 
  size of the objects free’d is equal or greater than the size requested(request).

**Cleanup**
- Clear ``mem_register`` completely.

**Shutdown**
- Run ``cleanup`` and delete ``mem_register``, ``destr_register`` and ``size_register``.

## Graphical example

![record](record.png?)

The rectangles represent our structures. Size is the size of the allocation our user gives us, the destructor is a function pointer also given to us. LÄGG TILL

## Overhead

How does the overhead change over time in comparison to the allocated memory size:
MAX GOT THIS


We had to decide for ourselves how to keep the overhead down, we solved this by creating a list with the pointers to our destructors and only sending the index, to the destructors place in the list, with our object and not the entire adress.
We did the same thing when we had to save the size of the object, NÅGON ANNAN FORTSÄTT 

How to save all of our allocations was also something we had to reason about so we could find the smartest way to keep all our info in a way that doesnt use unreasonble resource. We started off with a tree but later changed our minds and implemented a queue/list. We realised that the time complexity didn't matter when we changed our integer type to unsigned short, because it limited the trees size to 2^16. Therefor the tree would only be available to hold 2^16 objects and the time complexity O(logn) compared to O(n) was not valuable enough to use an AVL tree instead of a linked list.
