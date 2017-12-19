#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Automated.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "../src/tree.h"

int *ptrs[41];

void test_tree_apply_func(T elem, void *data) {
  int *numbr = (int *)elem;
  int *param = (int *)data;

  CU_ASSERT_PTR_NOT_NULL(numbr);
  CU_ASSERT_PTR_NOT_NULL(param);

  int i = *param;
  printf("%p\n", numbr);
  printf("%p\n\n", ptrs[i]);
  CU_ASSERT_PTR_EQUAL(numbr, ptrs[i]);
  (*param)++;
}

void test_tree_free_func(T elem) {
  int *ptr = (int *)elem;
  free(ptr);
}

void test_tree_new() {
  tree_t *tree = tree_new(NULL);

  CU_ASSERT_PTR_NOT_NULL(tree);
  CU_ASSERT_EQUAL(tree_size(tree), 0);

  tree_delete(tree, false);
}

void test_tree_insert() {
  tree_t *tree = tree_new(test_tree_free_func);
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

  int lower_limit = round(log2(n + 1));
  int upper_limit = round(c * log2(n + 2) + b);
  int h = tree_height(tree);

  CU_ASSERT_TRUE(h >= lower_limit && h <= upper_limit);

  CU_ASSERT_EQUAL(tree_size(tree), n);

  tree_delete(tree, true);
}

void test_tree_remove() {
  tree_t *tree = tree_new(test_tree_free_func);

  int n = 200;
  int *ptr[200];

  for (size_t i = 0; i < n; i++) {
    ptr[i] = calloc(1, sizeof(int));
    *ptr[i] = i;
  }

  for (size_t i = 0; i < n; i++) {
    tree_insert(tree, ptr[i]);
  }

  for (size_t i = 50; i < 120; i++) {
    CU_ASSERT_TRUE(tree_remove(tree, ptr[i]));
    n = n - 1;
    free(ptr[i]);
  }

  CU_ASSERT_EQUAL(tree_size(tree), n);

  double p = (1+sqrt(5)) / 2;
  double c = 1 / log2(p);
  double b = c/2 * log2(5) - 2;

  double lower_limit = log2(n + 1);
  double upper_limit = c * log2(n + 2) + b;
  double h = tree_height(tree);

  printf("%f\n", lower_limit);
  printf("%d\n", tree_height(tree));
  printf("%f\n", upper_limit);

  CU_ASSERT_TRUE(h >= lower_limit && h < upper_limit);

  tree_delete(tree, true);
}

void test_tree_apply() {
  tree_t *tree = tree_new(test_tree_free_func);

  for (size_t i = 0; i < 41; i++) {
    int *ptr = calloc(1, sizeof(int));
    *ptr = i;
    ptrs[i] = ptr;
    tree_insert(tree, ptr);
  }

  int param = 0;
  tree_apply(tree, test_tree_apply_func, &param);

  tree_delete(tree, true);
}

int main(int argc, char *argv[]) {
  CU_initialize_registry();

  CU_pSuite creation = CU_add_suite("Test creation", NULL, NULL);
  CU_add_test(creation, "Tree new", test_tree_new);
  CU_add_test(creation, "Tree insert", test_tree_insert);
  CU_add_test(creation, "Tree remove", test_tree_remove);
  CU_add_test(creation, "Tree apply", test_tree_apply);

  CU_basic_run_tests();
  CU_cleanup_registry();

  return CU_get_error();
}
