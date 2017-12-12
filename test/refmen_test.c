#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Automated.h>
#include <stdlib.h>
#include <stdio.h>
#include "../src/refmen.h"

struct test_struct {

  int x;
  int y;
  int z;
  int e;
  int t;
  int r;
  int u;
  int l;

} typedef test_t;

void test_allocate() {
  test_t *object = allocate(sizeof(test_t), NULL);

  CU_ASSERT_EQUAL(sizeof(*object), 32);
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
  // test_t *object = allocate(sizeof(test_t), NULL);
  // CU_ASSERT_EQUAL(rc(object), 0);
  // retain(object);
  // CU_ASSERT_EQUAL(rc(object), 1);
  // release(object);
  // CU_ASSERT_EQUAL(rc(object), 1);
}

int main(int argc, char *argv[]) {

  CU_initialize_registry();

  // Set up suites and tests
  CU_pSuite creation = CU_add_suite("Test allocation, deallocation", NULL, NULL);
  CU_add_test(creation, "Allocation", test_allocate);
  CU_add_test(creation, "Retain", test_retain);
  CU_add_test(creation, "Release", test_release);

  CU_basic_run_tests();

  // Tear down
  CU_cleanup_registry();
  return CU_get_error();
}
