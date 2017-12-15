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

  int n = 150000;

  for (size_t i = 2; i < n; i++) {
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
}

int main(int argc, char *argv[]) {

  CU_initialize_registry();

  // Set up suites and tests
  CU_pSuite creation = CU_add_suite("Test creation", NULL, NULL);
  CU_add_test(creation, "Tree new", test_tree_new);
  CU_add_test(creation, "Tree insert", test_tree_insert);

  CU_basic_run_tests();

  // Tear down
  CU_cleanup_registry();
  return CU_get_error();
}
