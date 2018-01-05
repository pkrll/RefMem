# Code Quality

Hello world!

## Table of Contents
* [Introduction](#introduction)
  * [What we mean when we talk about quality](#what-we-mean-when-we-talk-about-quality)
* [Naming things](#naming-things)
* [Abstracting stuff](#abstracting-stuff)
* [Formatting matters](#formatting-matters)
* [Deviations](#deviations)
  * [Function names](#function-names)

## Introduction
The team members of **Göktytorna** all have varying experience of programming in general, and of C in particular, making it more important to follow a coding standard that most are familiar and comfortable with.

We chose to adopt (and in one instance adapt) [Google's C/C++ style guide](https://google.github.io/styleguide/cppguide.html), as it is fairly close to what the team members have seen so far, ensuring maximum readability for everyone on the team.

Being comfortable and understanding code is the first step to attaining high quality. or something blabla.... **STRYKA???**

**Full disclosure**: We first decided on a coding standard a couple of weeks into the project. Though, the decision did not cause any bigger problems, as we were to a large degree following the convention already.

#### What we mean when we talk about quality
There is no easy answer to the question of what constitutes high quality code. Perhaps, there is no answer at all.

For our purposes, though, quality is the same thing as having code that is readable, maintainable and testable.

While performance may also be a key component, it is not always the case that high performance leads to higher quality code, as it may lower readability and maintainability (**KAN NÅN TYP GE ETT EXEMPEL?**). This stands in contrast to readability, which done right should make code easier to maintain. One clear example where when we tried to implement AVL-tree. It would result in better time complexity but added a whole new level of complexity for a simple task as store and extract elements.

## Naming things
> There are only two hard things in Computer Science: cache invalidation and **naming things**.

> -- Phil Karlton

As *Phil Karlton*, a developer on the web browser Netscape, has often been quoted saying, naming things is hard. Unfortunately, the names we choose to signify variables, functions, data members, etc., are an integral part of achieving high readability.

For that reason, you must take care while declaring, for instance, a variable. Pick a descriptive name that not only you but also your present and future colleagues understand. At the same time, there is a fine line between being just perfectly descriptive and being too verbose (unless you're an ``Objective-C`` or ``Swift`` developer).

In other words, the name should reflect what a function does, or what a variable holds. It should be clear by just reading the name what's going on (to a reasonable degree, of course!). And it is this principle (outlined in our own style guide) that we have tried to follow.

It is not easy to show an example of how we've applied a good naming standard to our project, as this is something that permeates the code.

But, for instance, an integer representing the number of references to an object should be called ``reference_count`` instead of just ``rc``. A function casting a variable from ``void *`` to a ``struct Record`` is better named ``convert_to_record()``, instead of the shorter ``convert()``.

Another example is the following function which retrieves the reference count of an object.

```c
size_t rc(obj object) {
  if (object != NULL) {
    Record_t *record = convert_to_record(object);

    return record->reference_count;
  }

  return 0;
}
```

It should be clear, by just reading the variable and function names, what is going on.

We could have first also declared a new variable, called ``reference_count``, that we assign to the reference count of the object, before returning that variable, but as most C programmers are familiar with how the ``->`` operator works, that wouldn't really have added much to the readability of the code.

 This function could have been written in a much shorter, concise way, but with lost readability.
 ```c
 // How NOT to do it:
 size_t rc(obj o) {
   if (o) {
     return convert(o)->reference_count;
   }

   return 0;
 }
 ```

**TODO**: Lägga till mer???

## Abstracting stuff
> The first rule of functions is that they should be small. The second rule of functions is that they should be smaller than that.

> -- Robert C. Martin

Another important part of keeping the code readable, maintainable and also testable has been through abstraction.

Not only does this allow for us to write simpler functions focused on one specific task, but it is generally easier to test or debug smaller functions.

## Deviations

Here we'll detail any deviations from the coding convention.

#### Function names

According to Google's conventions, functions should be named using *Pascal case*, for example ``SomeFunction()``. But as most of the team members are used to naming functions using snake case, we decided to use our own standard in this case.
