#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Automated.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "../src/list.h"

void test_list_new() {
  list_t *list = NULL;
  CU_ASSERT_PTR_NULL(list);
  list = list_new();
  CU_ASSERT_PTR_NOT_NULL(list);
  CU_ASSERT_EQUAL(list_length(list), 0);
}

static bool compare_p_test(element_t elem1, element_t elem2) {
  return elem1.p == elem2.p;
}

static bool compare_s_test(element_t elem1, element_t elem2) {
  return elem1.s == elem2.s;
}

void test_list_expand() {
  list_t *list = list_new();
  CU_ASSERT_EQUAL(list_length(list), 0);
  element_t elem1 = {.p = NULL};
  element_t elem2 = {.s = 1};
  unsigned short placement1 = list_expand(list, elem1, compare_p_test);
  unsigned short placement2 = list_expand(list, elem1, compare_p_test);
  unsigned short placement3 = list_expand(list, elem2, compare_s_test);
  CU_ASSERT_EQUAL(placement1, 0);
  CU_ASSERT_EQUAL(placement2, 0);
  CU_ASSERT_EQUAL(placement3, 1);
}

void test_list_get() {
  list_t *list = list_new();
  CU_ASSERT_EQUAL(list_length(list), 0);
  element_t elem1 = {.p = NULL};
  element_t elem2 = {.s = 1};
  unsigned short placement1 = list_expand(list, elem1, compare_p_test);
  unsigned short placement2 = list_expand(list, elem1, compare_p_test);
  unsigned short placement3 = list_expand(list, elem2, compare_s_test);
  element_t get_elem1 = list_get(list, placement1);
  element_t get_elem2 = list_get(list, placement2);
  element_t get_elem3 = list_get(list, placement3);
  CU_ASSERT_EQUAL(elem1.p, get_elem1.p);
  CU_ASSERT_EQUAL(elem1.p, get_elem2.p);
  CU_ASSERT_EQUAL(get_elem1.p, get_elem2.p);
  CU_ASSERT_EQUAL(elem2.s, get_elem3.s);
  CU_ASSERT_TRUE(compare_p_test(get_elem1, get_elem2));
}

void test_list_delete() {
  list_t *list = list_new();
  element_t elem = {.p = NULL};
  for(int i = 0; i < 50; i++) {
    list_expand(list, elem, compare_p_test);
  }
  CU_ASSERT_EQUAL(list_length(list), 1);

  list_delete(list);  
}

int main(int argc, char *argv[]) {
  CU_initialize_registry();

  CU_pSuite creation = CU_add_suite("Test creation", NULL, NULL);
  CU_add_test(creation, "List new and length", test_list_new);
  CU_add_test(creation, "List expand", test_list_expand);
  CU_add_test(creation, "List get", test_list_get);
  CU_add_test(creation, "List delete", test_list_delete);

  CU_basic_run_tests();
  CU_cleanup_registry();

  return CU_get_error();
}
