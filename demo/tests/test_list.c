#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Automated.h>
#include <stdlib.h>
#include <stdio.h>
#include "../list.h"
#include "../common.h"
#include "../utils.h"

bool apply_fun(obj object, void *data) {
  char *string = data;
  int element  = *(int *)object;

  return (element == 1 && strcmp(string, "Hello World!") == 0);
}

bool apply_fun2(obj object, void *data) {
  int *x = (int*)object;
  printf("ELEM: %d\n", *x);
  return true;
}

int comp_fun(obj a, obj b) {
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
  CU_ASSERT_EQUAL(list_length(list), 0);

  char *object = string_duplicate("Hello World!");
  list_append(list, object);

  CU_ASSERT_EQUAL(list_length(list), 1);

  list_remove(list, 0, false);
  CU_ASSERT_EQUAL(list_length(list), 0);

  for (int i = 0; i < 1500; i++) {
    list_append(list, &i);
  }

  CU_ASSERT_EQUAL(list_length(list), 1500);

  list_remove(list, 1, false);
  CU_ASSERT_EQUAL(list_length(list), 1499);

  list_remove(list, 0, false);
  CU_ASSERT_EQUAL(list_length(list), 1498);

  int x = 5;

  list_append(list, &x);
  CU_ASSERT_EQUAL(list_length(list), 1499);

  deallocate(list);
}

void test_list_insertion() {
  obj fst;
  obj lst;
  list_t *list = list_new(NULL, NULL, NULL);
  retain(list);

  int *firstElement = allocate(sizeof(int), NULL);
  int *secondElement = allocate(sizeof(int), NULL);
  int *thirdElement = allocate(sizeof(int), NULL);

  retain(firstElement);
  retain(secondElement);
  retain(thirdElement);

  *firstElement = 10;
  *secondElement = 42;
  *thirdElement = 99;

  list_insert(list, 0, firstElement);
  list_insert(list, 1, secondElement);
  list_insert(list, 2, thirdElement);

  CU_ASSERT_EQUAL(list_length(list), 3);

  list_first(list, &fst);
  CU_ASSERT_EQUAL(*(int*)fst, 10);

  list_last(list, &lst);
  CU_ASSERT_EQUAL(*(int*)lst, 99);

  char *data1 = string_duplicate("Hello World");
  char *data2 = string_duplicate("Hello Foo");
  char *data3 = string_duplicate("Hello Bar");

  retain(data1);
  retain(data2);
  retain(data3);

  list_insert(list, 0, data1);
  CU_ASSERT_EQUAL(list_length(list), 4);

  list_insert(list, 0, data2);
  CU_ASSERT_EQUAL(list_length(list), 5);

  list_insert(list, -1, data3);
  CU_ASSERT_EQUAL(list_length(list), 6);

  list_first(list, &fst);
  CU_ASSERT_STRING_EQUAL(fst, "Hello Foo");

  list_last(list, &lst);
  CU_ASSERT_STRING_EQUAL(lst, "Hello Bar");

  // Testing negative indicies some more
  int neg = 88;
  obj test = NULL;

  list_insert(list, -14, &neg);
  list_get(list, 0, &test);
  CU_ASSERT_EQUAL(*(int *)test, 88);

  for (int i = 0; i < 14; i++) {
    list_remove(list, i, false);
  }

  release(list);
  release(firstElement);
  release(secondElement);
  release(thirdElement);
  release(data1);
  release(data2);
  release(data3);
}

void test_list_append() {
  obj elem;

  list_t *list = list_new(NULL, NULL, NULL);
  retain(list);

  int *firstElement = allocate(sizeof(int), NULL);
  int *secondElement = allocate(sizeof(int), NULL);
  int *thirdElement = allocate(sizeof(int), NULL);

  retain(firstElement);
  retain(secondElement);
  retain(thirdElement);

  *firstElement = 10;
  *secondElement = 42;
  *thirdElement = 99;

  list_insert(list, 0, firstElement);
  list_last(list, &elem);
  CU_ASSERT_EQUAL(*(int*)elem, *firstElement);

  list_insert(list, 1, secondElement);
  list_last(list, &elem);
  CU_ASSERT_EQUAL(*(int*)elem, *secondElement);

  list_insert(list, 2, thirdElement);
  list_last(list, &elem);
  CU_ASSERT_EQUAL(*(int*)elem, *thirdElement);

  list_remove(list, 2, false);
  list_last(list, &elem);
  CU_ASSERT_EQUAL(*(int*)elem, *secondElement);
  
  release(list);

  release(firstElement);
  release(secondElement);
  release(thirdElement);
}

void test_list_prepend() {
  obj elem;
  list_t *list = list_new(NULL, NULL, NULL);

  for (int i = 0; i < 10; i++) {
    list_prepend(list, &i);
    list_first(list, &elem);
    CU_ASSERT_EQUAL(*(int*)elem, i);
  }

  release(list);
}

void test_list_remove() {
  list_t *list = list_new(NULL, NULL, NULL);

  for (int i = 0; i < 10; i++) {
    list_append(list, &i);
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

  deallocate(list);
}

void test_list_get() {
  obj elem;
  list_t *list = list_new(NULL, NULL, NULL);

  for (int i = 0; i < 10; i++) {
    int *x = NULL;
    *x = i;
    list_append(list, x);
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
  for (int i = 0; i < 10; i++) {
    int *x = NULL;
    *x = i;
    list_append(list, x);
  }

  CU_ASSERT_TRUE(list_first(list, &elem));
  CU_ASSERT_EQUAL(*(int*)elem, 0);

  list_remove(list, 0, false);
  CU_ASSERT_TRUE(list_first(list, &elem));
  CU_ASSERT_EQUAL(*(int*)elem, 1);

  deallocate(list);
}

void test_list_last() {
  obj elem;
  list_t *list = list_new(NULL, NULL, NULL);

  for (int i = 0; i < 10; i++) {
    int *x = NULL;
    *x = i;
    list_append(list, x);
  }

  CU_ASSERT_TRUE(list_last(list, &elem));
  CU_ASSERT_EQUAL(*(int*)elem, 9);

  list_remove(list, 9, false);
  CU_ASSERT_TRUE(list_last(list, &elem));
  CU_ASSERT_EQUAL(*(int*)elem, 8);

  deallocate(list);
}

void test_list_apply() {
  list_t *list = list_new(NULL, NULL, NULL);

  for (int i = 0; i < 10; i++) {
    int *x = NULL;
    *x = i;
    list_append(list, x);
  }

  CU_ASSERT_TRUE(list_apply(list, apply_fun, "Hello World!"));

  deallocate(list);
}

void test_list_contains() {
  int index;
  list_t *list = list_new(NULL, NULL, comp_fun);

  for (int i = 0; i < 10; i++) {
    int *x = NULL;
    *x = i;
    list_append(list, x);
  }

  int *five = NULL;
  *five = 5;
  list_append(list, five);
  int *fifteen = NULL;
  *fifteen = 15;

  index = list_contains(list, five);
  CU_ASSERT_EQUAL(index, 0);

  index = list_contains(list, fifteen);
  CU_ASSERT_EQUAL(index, -1);

  deallocate(list);
}

int main(int argc, char *argv[]) {

  CU_initialize_registry();

  // Set up suites and tests
  CU_pSuite creation = CU_add_suite("Test creation and length", NULL, NULL);
  CU_add_test(creation, "Creation", test_list_creation);
  CU_add_test(creation, "Length", test_list_length);

  CU_pSuite manipulation = CU_add_suite("Test insertion, prepend, append and remove", NULL, NULL);
  CU_add_test(manipulation, "Insertion", test_list_insertion);
  CU_add_test(manipulation, "Append", test_list_append);
  // CU_add_test(manipulation, "Prepend", test_list_prepend);
  // CU_add_test(manipulation, "Remove", test_list_remove);

  // CU_pSuite retrieval = CU_add_suite("Test retrieval", NULL, NULL);
  // CU_add_test(retrieval, "Get", test_list_get);
  // CU_add_test(retrieval, "First", test_list_first);
  // CU_add_test(retrieval, "Last", test_list_last);
  //
  // CU_pSuite misc = CU_add_suite("Test apply and contains", NULL, NULL);
  // CU_add_test(misc, "Apply", test_list_apply);
  // CU_add_test(misc, "Contains", test_list_contains);

  CU_basic_run_tests();

  // Tear down
  CU_cleanup_registry();
  return CU_get_error();
}
