# Testing (Code examples for the various test types will be added soon)

## TOC
* [Introduction](#intro)
* [Unit Testing](#unit-testing)
* [Integration Testing](#integration-testing)
* [Regression Testing](#regression-testing)
* [Handled Bugs and Other Nasties](#handled-bugs-and-other-nasties)

## Intro
Since the beginning of this project (And any other serious projects), tests have been manually written to backup the reliability of the components of the program. Tests were first written to require base functionality of the functions while later tests of the same functions focus on solidness and that the functions don't crash when receiving random type-correct data.
**TODO; What to answer on how we automated the tests. Wot?**

#### Unit Testing
Common tests and the functions were written side by side, making sure that every component works from the get-go. Later in development, more uncommon type-correct inputs (edge cases) were handled and tested in an attempt to ensure solidness of the functions.

This is to make sure that every component function as expected to guarantee that the individual components don't cause trouble during higher-level testing. (Ex Integration Testing) Basically, a standard white-box testing procedure was used.

#### Integration Testing
The integration testing of this project followed a clear bottom-up model. Components responsible for core functionality were tested first, quickly followed by higher-level ones.

Finally, functions combining multiple modules were tested as the bottom-up testing model suggests.
**TODO: Write a valid example which runs the program and tests if the GC works as intended when making the components work together. I fear that the demo falls under vertification and validation which is higher level testing than integration testing. However, it shouldn't take much time to make a modified sub-version of the demo that performs some checks during runtime.**

#### Regression Testing
Higher level regression test haven't been made yet. From a certain perspective, regression testing has always been here as a side effect of our pull request conditions and our unit tests. Since our unit tests both cover common and uncommon inputs, combined with Cunit always being monitored when changes have been made before approving a pull request, regression testing is in effect. Pull requests with code that has been tagged as faulty due to results from Cunit will not be approved.

## Handled bugs and other nasties
**TODO List the most significant bugs encountered and how they were defeated.  Also, code coverage.**
