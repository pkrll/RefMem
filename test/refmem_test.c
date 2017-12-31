#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Automated.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "../src/refmem.h"

struct test_struct {

  int x;
  int y;
  int z;

} typedef test_primitive_t;

struct test_struct_2 {

  int number;
  char *string;

} typedef test_t;

// Global object for destructor testing
void *dealloc_object;

char *new_string(char *str) {
  int length = strlen(str);
  char *rtrn = allocate(length + 1, NULL);

  strcpy(rtrn, str);

  return rtrn;
}

void test_t_destructor(obj object) {
  test_t *ptr = (test_t *)object;
  char *string = ptr->string;
  release(string);
}

void test_destructor(obj object) {
  // Make sure the object we get from destructor is the same as the one we deallocated
  CU_ASSERT_PTR_EQUAL(dealloc_object, object);
}

int test_cleanup() {
  shutdown();
  return 0;
}

void test_allocate() {
  //Tests of base functionality with common valid data.
  test_t *object = allocate(sizeof(test_t), test_t_destructor);
  test_t *test_zero = allocate(0, NULL);

  CU_ASSERT_PTR_NOT_NULL(object);
  CU_ASSERT_PTR_NOT_NULL(test_zero);

  // We can't test sizeof(*object), as it will be 12 bytes,
  // but it doesn't really guarantee that we have allocated 12 bytes

  char *string = new_string("Hello World!");
  retain(string);

  object->number = 10;
  object->string = string;

  CU_ASSERT_EQUAL(object->number, 10);
  CU_ASSERT_STRING_EQUAL(object->string, "Hello World!");

  deallocate(object);
  deallocate(test_zero);
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

  int* test_no_slots = (int*) allocate_array(0, sizeof(int), NULL);
  CU_ASSERT_NOT_EQUAL(test_no_slots, NULL);

  deallocate(numbers);
  deallocate(string);
  deallocate(test_no_slots);
}

void test_retain() {
  //Testing common functionality.
  test_primitive_t *test_zero = allocate(0, NULL);

  CU_ASSERT_EQUAL(rc(test_zero), 0);
  retain(test_zero);
  CU_ASSERT_EQUAL(rc(test_zero), 1);
  retain(test_zero);
  release(test_zero);
  CU_ASSERT_EQUAL(rc(test_zero), 1);
  release(test_zero);

  char *string = new_string("Foo");

  test_t *test = allocate(sizeof(test_t), test_t_destructor);
  test->string = string;

  retain(test);

  CU_ASSERT_EQUAL(rc(test->string), 0);
  retain(string);
  CU_ASSERT_EQUAL(rc(test->string), 1);
  retain(string);
  CU_ASSERT_EQUAL(rc(test->string), 2);
  release(string);

  test_t *same = test;
  retain(same);
  CU_ASSERT_EQUAL(rc(test), 2);
  release(same);

  for (size_t i = 0; i < 100; i++) {
    retain(test);
  }

  CU_ASSERT_EQUAL(rc(test), 101);

  for (size_t i = 0; i < 101; i++) {
    release(test);
  }
}

void test_release() {
  test_t *object = allocate(sizeof(test_t), NULL);

  for (size_t i = 1; i < 10; i++) {
    retain(object);
  }

  for (size_t i = 9; i > 0; i--) {
    release(object);
    if (i > 1) {
      CU_ASSERT_EQUAL(rc(object), i-1);
    }
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
  dealloc_object = allocate(sizeof(test_t), test_destructor);
  deallocate(dealloc_object);

  dealloc_object = allocate_array(5, sizeof(test_t), test_destructor);
  char *string = new_string("Hello World!");

  deallocate(dealloc_object);

  dealloc_object = string;
  deallocate(dealloc_object);
}

void test_set_cascade_limit() {
  set_cascade_limit(2);
  CU_ASSERT_EQUAL(get_cascade_limit(), 2);

  set_cascade_limit(3);
  CU_ASSERT_EQUAL(get_cascade_limit(), 3);

  for(int i= 0; i <= 100; i++) {
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
  CU_pSuite creation = CU_add_suite("Test allocation, deallocation", NULL, test_cleanup);
  CU_add_test(creation, "Allocation", test_allocate);
  CU_add_test(creation, "Allocation array", test_allocate_array);
  CU_add_test(creation, "Retain", test_retain);
  CU_add_test(creation, "Release", test_release);
  CU_add_test(creation, "Deallocate", test_deallocate);
  CU_add_test(creation, "RC", test_rc);
  CU_add_test(creation, "Cascade_lim", test_set_cascade_limit);

  CU_basic_run_tests();

  // Tear down
  CU_cleanup_registry();

  return CU_get_error();

}
