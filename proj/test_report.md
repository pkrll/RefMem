# Testing (Re-expressions and changes will be made. Also, comments and memory notations will also be wiped soon.)

## TOC
* [Introduction](#intro)
* [Unit Testing](#unit-testing)
* [Integration Testing](#integration-testing)
* [Regression Testing](#regression-testing)
* [Usage Summary](#summary-of-how-well-the-various-testing-types-were-used)
* [Handled Bugs and Other Nasties](#handled-bugs-and-other-nasties)

## Intro
Since the beginning of this project (And any other serious projects), tests have been manually written to backup the reliability of the components of the program. Tests were first written to require base functionality of the functions while later tests of the same functions focus on solidness and that the functions don't crash when receiving random type-correct data.

There are three types of testing methods that have been required to take into consideration in the project; Unit Testing, Integration Testing and Regression Testing. Generally speaking, the testing methods focus on vastly different levels of the code, and by that they achieve to catch certain errors that may not be present in the other methods.

#### Unit Testing
Common tests and the functions were written side by side, making sure that every component works from the get-go. Later in development, more uncommon type-correct inputs (edge cases) were handled and tested in an attempt to ensure solidness of the functions.

This is to make sure that every component function as expected to guarantee that the individual components don't cause trouble during higher-level testing. (Ex Integration Testing) Basically, a standard white-box testing procedure was used.

In this following example, common inputs, consistency and edge cases are tested.

(Perhaps use a more elaborate example?)

```c
void test_set_cascade_limit() {
  
  for(int i = 0; i <= 100; i++) {
    set_cascade_limit(i);
  }

  CU_ASSERT_EQUAL(get_cascade_limit(), 100);

  set_cascade_limit(0);
  CU_ASSERT_EQUAL(get_cascade_limit(), 0);

  set_cascade_limit(UINT_MAX);
  CU_ASSERT_EQUAL(get_cascade_limit(), UINT_MAX);
}

```

The function ``set_cascade_limit`` is a low-level component which is used in higher level to determine certain behaviour of the GC. The first 100 tests focus on common values and demanding consistency by throwing in a multitude of inputs. The two last tests try to act as edge-cases by checking the minimum and maximum values of the type.

#### Integration Testing
The integration testing of this project followed a clear bottom-up model. Components responsible for core functionality were tested first, quickly followed by higher-level ones.

Finally, functions combining multiple modules were tested as the bottom-up testing model suggests.

```c
database_t *database_new(char *file_path) {
  database_t *database = allocate(sizeof(database_t), database_destructor);

  if (database) {
    database->actions = stack_new(NULL);
    database->goods   = tree_new(element_copy, NULL, NULL, element_compare);

    retain(database->actions);
    retain(database->goods);

    if (file_path && strlen(file_path) > 0) {
      database->file_path = string_duplicate(file_path);
      retain(database->file_path);
      read_file(file_path, database_read_from_file, database);
    }
  }
```
Testing of above. Testing the GC from a higher level module.
Components are tested on a larger scale and if they work together as intended. In the demo, a database takes use of the GC for its memory management.

```c
void test_database_new() {
  database_t *database = database_new(NULL);
  CU_ASSERT_PTR_NOT_NULL(database);
  database_delete(database);
}
```
The database program is tested for expected behaviour and absence of memory leaks.

#### Regression Testing
From a certain perspective, regression testing has always been here as a side effect of our pull request conditions and our unit tests. Since our unit tests both cover common and uncommon inputs, combined with Cunit always being monitored when changes have been made before approving a pull request, regression testing is in effect. Pull requests with code that has been tagged as faulty due to results from Cunit will not be approved.

High-level regression testing was taken into consideration when modifications to the demo was made. The demo itself, is a simulation on how a user may use the GC for memory handling in their database. If any changes are made to the GC, the demo tests will tell if everything's still works after the changes.

TODO; Regression testing in optimizations. (Perhaps where it plays a larger role since this is where changes will be made without actually changing how the program works from a end user's perspective.)

## Summary of How Well The Various Testing Types Were Used
Since day one, we've been made sure to actually write some kinds of tests in symbiosis with the functions. A rule was set requiring tests for all written functions when performing a pull request from the working branch. Anything that didn't follow this rule would not be merged into the master branch. Later on, tests targeting edge-cases were written for eligible functions.

Due to the PR-condition, our project naturally followed a bottom-up structure of integration testing. Higher level testing of how well modules functioned together didn't start until the modules were more or less finished. There was simply no other method to actually do this since the demo, which combined the written modules, wouldn't work at all if the lower-level modules weren't already completed.

Regression testing wasn't actively enforced until later in the project. Until that point, it had only been a non-intended side effect of the set rules.

## Handled bugs and other nasties
**TODO List the most significant bugs encountered and how they were defeated.  Also, code coverage.**

![record](code_coverage_overview.png?)
