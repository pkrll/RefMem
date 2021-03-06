# Code Quality

## Table of Contents
* [Introduction](#introduction)
  * [What we mean when we talk about quality](#what-we-mean-when-we-talk-about-quality)
* [Naming things](#naming-things)
* [Abstracting stuff](#abstracting-stuff)
* [Deviations](#deviations)
  * [Function names](#function-names)

## Introduction
The team members of **Göktytorna** all have varying experience of programming in general, and of C in particular,
making it more important to follow a coding standard that most are familiar and comfortable with.

We chose the [Google's C/C++ style guide](https://google.github.io/styleguide/cppguide.html), as it is fairly close to what the team members have seen so far, ensuring maximum readability for everyone on the team. Being comfortable and understanding code is the first step to attaining high quality.

**Full disclosure**: We first decided on a coding standard a couple of weeks into the project. Though, the decision did not cause any bigger problems, as we were to a large degree following the convention already.

#### What we mean when we talk about quality
There is no easy answer to the question of what constitutes high quality code. Perhaps, there is no answer at all.

For our purposes, though, quality is the same thing as having code that is readable, maintainable and testable.

While performance also is a key component, it may not always be the case that high performance leads to higher quality code in general, as it may lower readability and maintainability. This stands in contrast to readability, which done correctly should make code easier to maintain.

To ensure high quality, we have throughout the project (with varying degrees of success, of course) tried to always keep in mind a couple of things:

* Picking clear, descriptive names for our variables and functions.
* Writing short functions that are only focused on one thing.

Formatting our code correctly is also important, but with the help of modern editors and the tool ``astyle``, this is not something that needs to be in focus when writing code.

## Naming things
> There are only two hard things in Computer Science: cache invalidation and **naming things**.

> -- Phil Karlton

As *Phil Karlton*, a developer on the web browser Netscape, has been quoted saying, naming things is hard. Unfortunately, the names we choose to signify variables, functions, data members, etc., are an integral part of achieving high readability.

For that reason, you must take care while declaring, for instance, a variable. Pick a descriptive name that not only you but also your present and future colleagues understand. At the same time, there is a fine line between being just perfectly descriptive and being too verbose (unless you're an ``Objective-C`` developer).

In other words, the name should reflect what a function does, or what a variable holds. It should be clear by just reading the name what's going on (to a reasonable degree, of course!).

It is this principle (outlined in our own style guide) that we have tried to follow. It is not easy to show an example of how we've applied a good naming standard to our project, as this is something that should permeate the code.

For instance, a function casting a variable from ``void *`` to a ``record_t`` is better named ``convert_to_record()``, than the shorter ``convert()``. Another example is the following function which retrieves the reference count of an object:

```c
unsigned short rc(obj object) {
  assert(object != NULL);
  header_t * header = convert_to_header(object);

  return header->reference_count;
}
```

It should be clear, by just reading the variable and function names, what is going on.

We could have first also declared a new variable, called ``reference_count``, that we assign to the reference count of the object, before returning that variable, but as most C programmers are familiar with how the ``->`` operator works, that wouldn't really have added much to the readability of the code.

 This function could have been written in a much shorter way, but with lost readability.
 ```c
 // How NOT to do it:
 size_t rc(obj o) {
   if (o) {
     return convert(o)->reference_count;
   }

   return 0;
 }
 ```

## Abstracting stuff
> The first rule of functions is that they should be small. The second rule of functions is that they should be smaller than that.

> -- Robert C. Martin

Another important part of keeping the code readable, maintainable and also testable has been through abstraction.

Not only does this allow for us to write simpler functions focused on one specific task, but it is generally easier to test or debug smaller functions and to change existing code.

Take for example the function ``allocate``. This is a function that has grown and been refactored a number of times, resulting in:

```c
obj allocate(size_t bytes, function1_t destructor) {
  prepare_for_allocation(bytes);

  element_t elem = {.f = destructor};
  element_t size = {.s = bytes};

  header_t *header = calloc(1, sizeof(header_t) + bytes);

  if (header) {
    header->reference_count = 0;
    header->destr_index = listset_expand(destr_register, elem, compare_destructor);
    header->size_index  = listset_expand(size_register, size, compare_size);

    header++;
  }

  return (obj)header;
}
```
Before each allocation, the system should clear the register holding all deallocated objects. By moving that logic to its own function (called ``clear_obj_register``, which is invoked by ``prepare_for_allocation`` above), the code is easier to read, understand and maintain.

This makes it possible to reuse the same function ``allocate_array``, and, again, reducing the risk of bugs.

## Deviations

Here we'll detail any deviations from the coding convention.

#### Function and type names

According to Google's conventions, functions and types should be named using *Pascal case*, for example ``SomeFunction()`` or ``Header_t``. But as most of the team members are used to naming functions using snake case, we decided to use our own standard in this case.
