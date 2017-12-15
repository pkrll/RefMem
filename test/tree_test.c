#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Automated.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "../src/tree.h"

void test_tree_new() {
  tree_t *tree = tree_new();
  CU_ASSERT_PTR_NOT_NULL(tree);
}

void test_tree_insert() {
  tree_t *tree = tree_new();
  int *ptr = calloc(1, sizeof(int *));

  tree_insert(tree, ptr);

  CU_ASSERT_EQUAL(tree_size(tree), 1);

  int n = 150001;

  for (size_t i = 1; i < n; i++) {
    int *ptr = calloc(1, sizeof(int *));
    tree_insert(tree, ptr);
  }

  double p = (1+sqrt(5)) / 2;
  double c = 1 / log2(p);
  double b = c/2 * log2(5) - 2;

  double lower_limit = log2(n + 1);
  double upper_limit = c * log2(n + 2) + b;
  double h = tree_height(tree);

  CU_ASSERT_TRUE(h >= lower_limit && h <= upper_limit);

  CU_ASSERT_EQUAL(tree_size(tree), n);
}

void test_tree_remove() {
  tree_t *tree = tree_new();

  int n = 200;
  int *ptr[200];

  for (size_t i = 0; i < n; i++) {
    ptr[i] = calloc(1, sizeof(int));
    *ptr[i] = i;
  }

  for (size_t i = 0; i < n; i++) {
    tree_insert(tree, ptr[i]);
  }
  size_t s = 50;
  size_t e = 120;
  for (size_t i = s; i < e; i++) {
    CU_ASSERT_TRUE(tree_remove(tree, ptr[i]));
    n = n - 1;
  }

  CU_ASSERT_EQUAL(tree_size(tree), n);

  double p = (1+sqrt(5)) / 2;
  double c = 1 / log2(p);
  double b = c/2 * log2(5) - 2;

  double lower_limit = log2(n + 1);
  double upper_limit = c * log2(n + 2) + b;
  double h = tree_height(tree);

  CU_ASSERT_TRUE(h >= lower_limit && h < upper_limit);

}

int main(int argc, char *argv[]) {

  CU_initialize_registry();

  // Set up suites and tests
  CU_pSuite creation = CU_add_suite("Test creation", NULL, NULL);
  CU_add_test(creation, "Tree new", test_tree_new);
  CU_add_test(creation, "Tree insert", test_tree_insert);
  CU_add_test(creation, "Tree remove", test_tree_remove);

  CU_basic_run_tests();

  // Tear down
  CU_cleanup_registry();
  return CU_get_error();
}
