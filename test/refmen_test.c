#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Automated.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include "../src/refmen.h"

struct test_struct {

  int x;
  int y;
  int z;

} typedef test_t;

// Global object for destructor testing
test_t *dealloc_object;

void test_destructor(obj object) {
  // Make sure the object we get from destructor is the same as the one we deallocated
  CU_ASSERT_PTR_EQUAL(dealloc_object, object);
}

void test_allocate() {
  //Tests of base functionality with common valid data.
  test_t *object = allocate(sizeof(test_t), NULL);
  CU_ASSERT_EQUAL(sizeof(*object), 12);

  //Tests of weird data that shall pass even if they make no sense.
  test_t *test_zero = allocate(0, NULL);
  test_t *test_null = allocate(sizeof(NULL), NULL);
  test_t *test_max = allocate(UINT_MAX, NULL);

  CU_ASSERT_PTR_NOT_NULL(test_zero);
  CU_ASSERT_PTR_NOT_NULL(test_null);
  CU_ASSERT_TRUE(sizeof(size_t) <= sizeof(test_max));

  deallocate(object);
  deallocate(test_zero);
  deallocate(test_null);
  deallocate(test_max);
}

void test_allocate_array() {
  int *numbers = (int *) allocate_array(4, sizeof(int), NULL);
  char *string = (char *)allocate_array(4, sizeof(char), NULL);

  char *test_string = "Foo!";

  CU_ASSERT_PTR_NOT_NULL(numbers);
  CU_ASSERT_PTR_NOT_NULL(string);

  numbers[0] = 4;
  numbers[1] = 3;
  numbers[2] = 2;
  numbers[3] = 1;

  for (size_t i = 0; i < 4; i++) {
    CU_ASSERT_EQUAL(numbers[i], 4 - i);
    string[i] = test_string[i];
  }

  CU_ASSERT_EQUAL(strncmp(string, test_string, 4), 0);

  //Edge case in terms of memory handling?
  int* test_high_mem = (int*) allocate_array((pow(2, 63) + 2), sizeof(int), NULL);

  for(long long int i = 0; i < 1000000000000000000; i+= 10000000000000000) {
    CU_ASSERT_EQUAL(sizeof(test_high_mem[i]), sizeof(int));
  }

  //Tests weird input data, I'm more or less clueless on what the actual edge-cases are by now.
  int* test_no_slots = (int*) allocate_array(0, sizeof(int), NULL);

  CU_ASSERT_NOT_EQUAL(test_no_slots, NULL);

  deallocate(numbers);
  deallocate(string);
}

void test_retain() {
  //Testing common functionality.
  test_t *test_zero = allocate(0, NULL);
  test_t *test_large = allocate(UINT_MAX, NULL);

  CU_ASSERT_EQUAL(rc(test_zero), 0);
  CU_ASSERT_EQUAL(rc(test_large), 0);
  retain(test_zero);
  CU_ASSERT_EQUAL(rc(test_zero), 1);

  retain(test_large);
  CU_ASSERT_EQUAL(rc(test_large), 1);
  retain(test_large);
  retain(test_large);
  retain(test_large);
  CU_ASSERT_EQUAL(rc(test_large), 4);

  release(test_large);
  release(test_large);
  release(test_large);
  release(test_large);
  release(test_zero);

}

void test_release() {
  test_t *object = allocate(sizeof(test_t), NULL);

  for (size_t i = 1; i < 10; i++) {
    retain(object);
  }

  for (size_t i = 8; i > 0; i--) {
    release(object);
    CU_ASSERT_EQUAL(rc(object), i);
  }
}

void test_rc() {
  test_t *object = allocate(sizeof(test_t), NULL);
  CU_ASSERT_EQUAL(rc(object), 0);
  retain(object);
  CU_ASSERT_EQUAL(rc(object), 1);
  retain(object);
  CU_ASSERT_EQUAL(rc(object), 2);
  release(object);
  CU_ASSERT_EQUAL(rc(object), 1);
  release(object);
}

void test_deallocate() {
  dealloc_object = allocate(sizeof(test_t), (function1_t) test_destructor);
  deallocate(dealloc_object);
}

void test_set_cascade_limit() {
  set_cascade_limit(2);
  CU_ASSERT_EQUAL(get_cascade_limit(), 2);

  set_cascade_limit(3);
  CU_ASSERT_EQUAL(get_cascade_limit(), 3);

  for(int i= 0; i<=100; i++) {
    set_cascade_limit(i);
  }
  CU_ASSERT_EQUAL(get_cascade_limit(), 100);

  set_cascade_limit(0);
  CU_ASSERT_EQUAL(get_cascade_limit(), 0);

  set_cascade_limit(UINT_MAX);
  CU_ASSERT_EQUAL(get_cascade_limit(), UINT_MAX);

}

int main(int argc, char *argv[]) {

  CU_initialize_registry();

  // Set up suites and tests
  CU_pSuite creation = CU_add_suite("Test allocation, deallocation", NULL, NULL);
  CU_add_test(creation, "Allocation", test_allocate);
  CU_add_test(creation, "Allocation array", test_allocate_array);
  CU_add_test(creation, "Retain", test_retain);
  CU_add_test(creation, "Release", test_release);
  CU_add_test(creation, "RC", test_rc);
  CU_add_test(creation, "Cascade_lim", test_set_cascade_limit);

  CU_basic_run_tests();

  // Tear down
  CU_cleanup_registry();
  return CU_get_error();
}
