# Testing (Re-expressions and changes will be made. Also, comments and memory notations will also be wiped soon.)

## TOC
* [Introduction](#intro)
* [Unit Testing](#unit-testing)
* [Integration Testing](#integration-testing)
* [Regression Testing](#regression-testing)
* [Summary](#summary)
* [Handled Bugs and Other Nasties](#handled-bugs-and-other-nasties)
* [Code Coverage](#code-coverage)

## Intro
Since the beginning of this project (And any other serious projects), tests have been manually written to backup the reliability of the components of the program. Tests were first written to require base functionality of the functions while later tests of the same functions focus on solidness and that the functions don't crash when receiving random type-correct data.

There are three types of testing methods that have been required to take into consideration in the project; Unit Testing, Integration Testing and Regression Testing. Generally speaking, the testing methods focus on vastly different levels of the code, and by that they achieve to catch certain errors that may not be present in the other methods.

#### Unit Testing
Common tests and the functions were written side by side, making sure that every component works from the get-go. Later in development, more uncommon type-correct inputs (edge cases) were handled and tested in an attempt to ensure solidness of the functions.

This is to make sure that every component function as expected to guarantee that the individual components don't cause trouble during higher-level testing. Basically, a standard white-box testing procedure was used.

#### Integration Testing
The integration testing of this project followed a clear bottom-up model. Components responsible for core functionality were tested first, quickly followed by higher-level ones.

Finally, functions combining multiple modules were tested as the bottom-up testing model suggests.

```c
void test_database_insert_item() {
  database_t *database = database_new(NULL);
  retain(database);

  item_t *item_add = create_item("Test Name", "Test Desc", 1, "A1", 2);

  retain(item_add);

  CU_ASSERT_TRUE(database_insert_item(database, item_add));
  CU_ASSERT_FALSE(database_insert_item(database, item_add));

  char *name = string_duplicate("Test Name");
  retain(name);

  item_t *item_get = database_get_item(database, name);
  retain(item_get);

  CU_ASSERT_STRING_EQUAL(item_get_name(item_get), name);
```

The above test involves not only the ``database`` module, but also ``item``, ``list`` and ``tree``. The database program is tested for expected behaviour and absence of memory leaks.

#### Regression Testing
From a certain perspective, regression testing has always been here as a side effect of our pull request conditions and our unit tests. Since our unit tests both cover common and uncommon inputs, combined with Cunit always being monitored when changes have been made before approving a pull request, regression testing is in effect. Pull requests with code that has been tagged as faulty due to results from Cunit will not be approved.

High-level regression testing was taken into consideration when modifications to the demo was made. The demo itself, is a simulation on how a user may use the GC for memory handling in their database. If any changes are made to the GC, the demo tests will tell if everything's still works after the changes.

To allow regression testing of certain parts of the program, we've divided the tests into their own files depending on what module they belong to. Typing ``make test`` will run all tests while ``make test-listset`` only runs the tests for the listset. There is no support for testing certain parts of a module. This could be achieved by dividing the module into multiple suites depending on what category the functions belong to. But since none of the modules in the program of the project are of relatively large size, it wouldn't save much time running tests that only check a specific suite in the module.

## Summary
Since day one, we've been made sure to actually write some kinds of tests in symbiosis with the functions. A rule was set requiring tests for all written functions when performing a pull request from the working branch. Anything that didn't follow this rule would not be merged into the master branch. Later on, tests targeting edge-cases were written for eligible functions.

Due to the PR-condition, our project naturally followed a bottom-up structure of integration testing. Higher level testing of how well modules functioned together didn't start until the modules were more or less finished. There was simply no other method to actually do this since the demo, which combined the written modules, wouldn't work at all if the lower-level modules weren't already completed.

Regression testing couldn't actively be enforced until later in the project.

## Handled bugs and other nasties

No fatal bugs were really encountered during this project. Due to our set conditions for a PR to be approved, bugs of significance never rarely or never reached the master branch. Other pieces of faulty code did pass a PR, but they would never affect the functionality of the program. Instead, it would more commonly be related to memory issues such as memory leakages.

When we did encounter a bug, it was mostly through our integration tests.

Our 6 most nasty bugs include:

* [Error in (demo) list remove](https://github.com/IOOPM-UU/goktytorna/issues/44)
* [Cannot run demo after merge](https://github.com/IOOPM-UU/goktytorna/issues/29)
* [Assertion failure](https://github.com/IOOPM-UU/goktytorna/issues/50)
* [Memory leakage when running demo with inputs from text file](https://github.com/IOOPM-UU/goktytorna/issues/54)
* [Leaks in Tree.c](https://github.com/IOOPM-UU/goktytorna/issues/19)
* [Demo tests do not work](https://github.com/IOOPM-UU/goktytorna/issues/41)

## Code Coverage

![record](code_covrage_overview.png?)
