#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Automated.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../demo/list.h"
#include "../../demo/utils.h"
#include "../../demo/common.h"

int teardown() {
  cleanup();
  return 0;
}

int *new_integer(int number) {
  int *integer = allocate(sizeof(int), NULL);
  *integer = number;

  return integer;
}

bool apply_fun(obj object, void *data) {
  char *string = data;
  int element  = *(int *)object;

  return (element == 1 && strcmp(string, "Hello World!") == 0);
}

int comp_fun(obj x, obj y) {
  int a = *(int*)x;
  int b = *(int*)y;

  if (a > b) return -1;
  if (a < b) return 1;

  return 0;
}

// -------------------------------
// TESTS
// -------------------------------

void test_list_creation() {
  list_t *list = list_new(NULL, NULL, NULL);
  CU_ASSERT_PTR_NOT_NULL(list);
  CU_ASSERT_EQUAL(list_length(list), 0);

  deallocate(list);
}

void test_list_length() {
  list_t *list = list_new(NULL, NULL, NULL);
  retain(list);

  CU_ASSERT_EQUAL(list_length(list), 0);

  int *n = new_integer(15);
  list_append(list, n);

  CU_ASSERT_EQUAL(list_length(list), 1);

  list_remove(list, 0, false);
  CU_ASSERT_EQUAL(list_length(list), 0);

  for (int i = 0; i < 1500; i++) {
    int *x = new_integer(i);
    list_append(list, x);
  }

  CU_ASSERT_EQUAL(list_length(list), 1500);

  list_remove(list, 1, false);
  CU_ASSERT_EQUAL(list_length(list), 1499);

  list_remove(list, 0, false);
  CU_ASSERT_EQUAL(list_length(list), 1498);

  int *z = new_integer(-5);

  list_append(list, z);
  CU_ASSERT_EQUAL(list_length(list), 1499);

  release(list);
}

void test_list_insertion() {
  obj fst;
  obj lst;
  list_t *list = list_new(NULL, NULL, comp_fun);
  retain(list);

  for (int i = 0; i < 10; i++) {
    int *number = new_integer(i);
    retain(number);
    list_insert(list, i, number);
    release(number);
  }

  CU_ASSERT_EQUAL(list_length(list), 10);

  list_first(list, &fst);
  CU_ASSERT_EQUAL(*(int*)fst, 0);

  list_last(list, &lst);
  CU_ASSERT_EQUAL(*(int*)lst, 9);

  int *number1 = new_integer(99);
  retain(number1);

  list_insert(list, 0, number1);
  CU_ASSERT_EQUAL(list_length(list), 11);

  release(number1);

  int *number2 = new_integer(98);
  retain(number2);

  list_insert(list, 0, number2);
  CU_ASSERT_EQUAL(list_length(list), 12);

  release(number2);

  int *number3 = new_integer(934);
  retain(number3);

  list_insert(list, -1, number3);
  CU_ASSERT_EQUAL(list_length(list), 13);

  release(number3);

  list_first(list, &fst);
  CU_ASSERT_EQUAL(*(int*)fst, 98);

  list_last(list, &lst);
  CU_ASSERT_EQUAL(*(int*)lst, 934);

  // Testing negative indicies some more
  obj test;
  int *number4 = new_integer(88);
  retain(number4);

  list_insert(list, -14, number4);
  list_get(list, 0, &test);
  CU_ASSERT_EQUAL(*(int *)test, 88);

  release(number4);

  for (int i = 0; i < 14; i++) {
    list_remove(list, i, false);
  }

  // testing insertion with 500 elems
  // perhaps unneccesary
  for (int i = 0; i < 500; i++) {
    int *number = new_integer(i);
    retain(number);
    list_insert(list, i, new_integer(i));
    release(number);
  }

  CU_ASSERT_EQUAL(list_length(list), 500);

  release(list);
}

void test_list_append() {
  obj elem;
  list_t *list = list_new(NULL, NULL, NULL);
  retain(list);

  for (int i = 0; i < 100; i++) {
    list_append(list, new_integer(i));
    list_last(list, &elem);
    CU_ASSERT_EQUAL(*(int*)elem, i);
  }

  release(list);
}

void test_list_prepend() {
  obj elem;
  list_t *list = list_new(NULL, NULL, NULL);
  retain(list);

  for (int i = 0; i < 10; i++) {
    list_prepend(list, new_integer(i));
    list_first(list, &elem);
    CU_ASSERT_EQUAL(*(int*)elem, i);
  }

  release(list);
}

void test_list_remove() {
  list_t *list = list_new(NULL, NULL, NULL);
  retain(list);

  for (int i = 0; i < 10; i++) {
    list_append(list, new_integer(i));
  }

  CU_ASSERT_EQUAL(list_length(list), 10);

  list_remove(list, 0, false);
  CU_ASSERT_EQUAL(list_length(list), 9);

  list_remove(list, -1, false);
  CU_ASSERT_EQUAL(list_length(list), 8);

  list_remove(list, 100, false);
  CU_ASSERT_EQUAL(list_length(list), 7);

  list_remove(list, -100, false);
  CU_ASSERT_EQUAL(list_length(list), 6);

  for (int i = 0; i < 6; i++) {
    list_remove(list, i, false);
  }

  CU_ASSERT_EQUAL(list_length(list), 0);
  char *test = string_duplicate("Hello");

  list_insert(list, 0, test);
  CU_ASSERT_EQUAL(list_length(list), 1);

  release(list);
}

void test_list_get() {
  obj elem;
  list_t *list = list_new(NULL, NULL, NULL);

  for (int i = 0; i < 10; i++) {
    list_append(list, new_integer(i));
  }

  CU_ASSERT_TRUE(list_get(list, 0, &elem));
  CU_ASSERT_EQUAL(*(int*)elem, 0);

  CU_ASSERT_TRUE(list_get(list, 9, &elem));
  CU_ASSERT_EQUAL(*(int*)elem, 9);

  CU_ASSERT_FALSE(list_get(list, -1, &elem));
  CU_ASSERT_FALSE(list_get(list, -100, &elem));
  CU_ASSERT_FALSE(list_get(list, 1200, &elem));
  CU_ASSERT_FALSE(list_get(list, -5, &elem));

  deallocate(list);
}

void test_list_first() {
  obj elem;
  list_t *list = list_new(NULL, NULL, NULL);
  retain(list);

  for (int i = 0; i < 10; i++) {
    list_append(list, new_integer(i));
  }

  CU_ASSERT_TRUE(list_first(list, &elem));
  CU_ASSERT_EQUAL(*(int*)elem, 0);

  list_remove(list, 0, false);
  CU_ASSERT_TRUE(list_first(list, &elem));
  CU_ASSERT_EQUAL(*(int*)elem, 1);

  release(list);
}

void test_list_last() {
  obj elem;
  list_t *list = list_new(NULL, NULL, NULL);
  retain(list);

  for (int i = 0; i < 10; i++) {
    list_append(list, new_integer(i));
  }

  CU_ASSERT_TRUE(list_last(list, &elem));
  CU_ASSERT_EQUAL(*(int*)elem, 9);

  list_remove(list, 9, false);
  CU_ASSERT_TRUE(list_last(list, &elem));
  CU_ASSERT_EQUAL(*(int*)elem, 8);

  release(list);
}

void test_list_apply() {
  list_t *list = list_new(NULL, NULL, NULL);
  retain(list);
  for (int i = 0; i < 10; i++) {
    list_append(list, new_integer(i));
  }

  CU_ASSERT_TRUE(list_apply(list, apply_fun, "Hello World!"));

  release(list);
}

void test_list_contains() {
  int index;
  list_t *list = list_new(NULL, NULL, comp_fun);
  retain(list);
  for (int i = 0; i < 10; i++) {
    list_append(list, new_integer(i));
  }

  int *five = new_integer(15);
  retain(five);
  list_append(list, five);

  index = list_contains(list, five);
  CU_ASSERT_EQUAL(index, 10);

  release(five);

  int *fifteen = new_integer(99);
  retain(fifteen);

  index = list_contains(list, fifteen);
  CU_ASSERT_EQUAL(index, -1);

  release(fifteen);

  release(list);
}

int main(int argc, char *argv[]) {

  CU_initialize_registry();

  // Set up suites and tests
  CU_pSuite creation = CU_add_suite("Test creation and length", NULL, teardown);
  CU_add_test(creation, "Creation", test_list_creation);
  CU_add_test(creation, "Length", test_list_length);

  CU_pSuite manipulation = CU_add_suite("Test insertion, prepend, append and remove", NULL, teardown);
  CU_add_test(manipulation, "Insertion", test_list_insertion);
  CU_add_test(manipulation, "Append", test_list_append);
  CU_add_test(manipulation, "Prepend", test_list_prepend);
  CU_add_test(manipulation, "Remove", test_list_remove);

  CU_pSuite retrieval = CU_add_suite("Test retrieval", NULL, teardown);
  CU_add_test(retrieval, "Get", test_list_get);
  CU_add_test(retrieval, "First", test_list_first);
  CU_add_test(retrieval, "Last", test_list_last);

  CU_pSuite misc = CU_add_suite("Test apply and contains", NULL, teardown);
  CU_add_test(misc, "Apply", test_list_apply);
  CU_add_test(misc, "Contains", test_list_contains);

  CU_basic_run_tests();

  // Tear down
  CU_cleanup_registry();

  shutdown();

  return CU_get_error();
}
