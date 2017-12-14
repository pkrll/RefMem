#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Automated.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "../src/refmen.h"

struct test_struct {

  int x;
  int y;
  int z;

} typedef test_t;

void test_allocate() {
  test_t *object = allocate(sizeof(test_t), NULL);
  CU_ASSERT_EQUAL(sizeof(*object), 12);
}

void test_retain() {
  test_t *object = allocate(sizeof(test_t), NULL);

  for (size_t i = 1; i < 10; i++) {
    retain(object);
    CU_ASSERT_EQUAL(rc(object), i);
  }

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
}

// Global object for destructor testing 
test_t *dealloc_object;

void test_destructor(obj object) {
  // Make sure the object we get from destructor is the same as the one we deallocated
  CU_ASSERT_PTR_EQUAL(dealloc_object, object);
}

void test_deallocate() {
  dealloc_object = allocate(sizeof(test_t), (function1_t) test_destructor);
  deallocate(dealloc_object);
}

int main(int argc, char *argv[]) {

  CU_initialize_registry();

  // Set up suites and tests
  CU_pSuite creation = CU_add_suite("Test allocation, deallocation", NULL, NULL);
  CU_add_test(creation, "Allocation", test_allocate);
  CU_add_test(creation, "Deallocation", test_deallocate);
  CU_add_test(creation, "Retain", test_retain);
  CU_add_test(creation, "Release", test_release);
  CU_add_test(creation, "RC", test_rc);

  CU_basic_run_tests();

  // Tear down
  CU_cleanup_registry();
  return CU_get_error();
}
