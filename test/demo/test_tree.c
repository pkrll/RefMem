#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Automated.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../demo/utils.h"
#include "../../demo/tree.h"
#include "../../demo/common.h"

int *new_integer(int number) {
  int *integer = allocate(sizeof(int), NULL);
  *integer = number;
  return integer;
}

int comp_fun(obj a, obj b) {
  char *str_a = (char *)a;
  char *str_b = (char *)b;

  return strcmp(str_a, str_b);
}

int comp_fun_int(obj a, obj b) {
  int *int_a = (int *)a;
  int *int_b = (int *)b;

  if (*int_a == *int_b) return 0;
  if (*int_a > *int_b) return 1;

  return -1;
}

obj copy_fun(obj e) {
  CU_PASS("Copy");
  return e;
}

bool apply_fun(obj key, obj elem, void *data) {
  CU_ASSERT_EQUAL(data, "Hello World");
  return true;
}

// -------------------------------
// Tests
// -------------------------------

void test_tree_creation() {
  tree_t *tree = tree_new(NULL, NULL, NULL, NULL);
  retain(tree);

  CU_ASSERT_PTR_NOT_NULL(tree);
  CU_ASSERT_EQUAL(tree_size(tree), 0);

  release(tree);
}

void test_tree_delete() {
  tree_t *tree = tree_new(NULL, NULL, NULL, NULL);
  retain(tree);

  char *element_1 = string_duplicate("Foo");
  char *element_2 = string_duplicate("Bar");
  char *element_3 = string_duplicate("Baz");
  char *element_4 = string_duplicate("Bae");

  tree_insert(tree, element_1, element_1);
  tree_insert(tree, element_2, element_2);
  tree_insert(tree, element_3, element_3);
  tree_insert(tree, element_4, element_4);

  release(tree);
}

void test_tree_size() {
  tree_t *tree = tree_new(NULL, NULL, NULL, comp_fun);
  retain(tree);

  char *strings[6] = { "Hello", "World", "foo", "bar", "42", "whatup" };

  for (int i = 0; i < 6; i++) {
    tree_insert(tree, string_duplicate(strings[i]), string_duplicate(strings[i]));
    CU_ASSERT_EQUAL(tree_size(tree), i + 1);
  }

  obj element;
  tree_remove(tree, strings[5], &element);
  CU_ASSERT_EQUAL(tree_size(tree), 5);

  tree_remove(tree, strings[1], &element);
  CU_ASSERT_EQUAL(tree_size(tree), 4);

  tree_insert(tree, string_duplicate(strings[1]), string_duplicate(strings[1]));
  CU_ASSERT_EQUAL(tree_size(tree), 5);

  for (int i = 0; i < 5; i++) {
    tree_remove(tree, strings[i], &element);
  }

  CU_ASSERT_EQUAL(tree_size(tree), 0);

  release(tree);
}

void test_tree_depth() {
  tree_t *tree = tree_new(NULL, NULL, NULL, comp_fun);
  retain(tree);

  char *strings[10] = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j" };

  // testing right side
  for (int i = 0; i < 10; i++) {
    tree_insert(tree, string_duplicate(strings[i]), string_duplicate(strings[i]));
  }

  CU_ASSERT_EQUAL(tree_depth(tree), 10);
  obj elem;
  for (int i = 0; i < 10; i++) {
    tree_remove(tree, strings[i], &elem);
  }

  CU_ASSERT_EQUAL(tree_depth(tree), 0);

  // testing left side
  for (int i = 9; i >= 0; i--) {
    tree_insert(tree, string_duplicate(strings[i]), string_duplicate(strings[i]));
  }

  CU_ASSERT_EQUAL(tree_depth(tree), 10);

  for (int i = 9; i >= 0; i--) {
    tree_remove(tree, strings[i], &elem);
  }

  CU_ASSERT_EQUAL(tree_depth(tree), 0);

  // More normal tree
  tree_insert(tree, string_duplicate("40"), string_duplicate("40"));
  tree_insert(tree, string_duplicate("20"), string_duplicate("20"));
  tree_insert(tree, string_duplicate("32"), string_duplicate("32"));
  tree_insert(tree, string_duplicate("75"), string_duplicate("75"));
  tree_insert(tree, string_duplicate("50"), string_duplicate("50"));
  tree_insert(tree, string_duplicate("45"), string_duplicate("45"));
  tree_insert(tree, string_duplicate("60"), string_duplicate("60"));

  CU_ASSERT_EQUAL(tree_depth(tree), 4);

  release(tree);
}

void test_tree_insert() {
  // Without compare, copy functions
  tree_t *tree = tree_new(NULL, NULL, NULL, comp_fun_int);
  retain(tree);

  int **numbers = allocate_array(1000, sizeof(int*), NULL);

  // Testing inserting a lot of stuff
  for (int i = 0; i < 1000; i++) {
    numbers[i] = allocate(sizeof(int), NULL);
    retain(numbers[i]);
    *numbers[i] = i;

    CU_ASSERT_TRUE(tree_insert(tree, numbers[i], numbers[i]));
    CU_ASSERT_EQUAL(tree_size(tree), i + 1);
  }

  CU_ASSERT_TRUE(tree_has_key(tree, numbers[0]));
  CU_ASSERT_TRUE(tree_has_key(tree, numbers[200]));


  tree_remove(tree, numbers[10], NULL);
  // Insert after remove
  CU_ASSERT_TRUE(tree_insert(tree, numbers[10], numbers[10]));
  CU_ASSERT_EQUAL(tree_size(tree), 1000);
  // Insert existing key
  CU_ASSERT_FALSE(tree_insert(tree, numbers[600], numbers[600]));
  CU_ASSERT_EQUAL(tree_size(tree), 1000);
  CU_ASSERT_TRUE(tree_has_key(tree, numbers[600]));

  release(tree);

  // With copy, compare functions
  tree = tree_new(copy_fun, NULL, NULL, comp_fun_int);

  CU_ASSERT_TRUE(tree_insert(tree, numbers[10], numbers[10]));
  CU_ASSERT_TRUE(tree_insert(tree, numbers[5], numbers[5]));
  CU_ASSERT_TRUE(tree_insert(tree, numbers[15], numbers[15]));
  CU_ASSERT_TRUE(tree_insert(tree, numbers[8], numbers[8]));

  CU_ASSERT_EQUAL(tree_size(tree), 4);

  tree_remove(tree, numbers[10], NULL);
  tree_remove(tree, numbers[5], NULL);
  tree_remove(tree, numbers[15], NULL);
  tree_remove(tree, numbers[8], NULL);

  CU_ASSERT_TRUE(tree_insert(tree, numbers[10], numbers[10]));
  CU_ASSERT_EQUAL(tree_size(tree), 1);

  release(tree);

  for (int i = 0; i < 1000; i++) {
    release(numbers[i]);
  }

  release(numbers);
}

void test_tree_has_key() {
  tree_t *tree = tree_new(NULL, NULL, NULL, comp_fun);
  retain(tree);

  char *key = string_duplicate("150");
  retain(key);

  tree_insert(tree, key, string_duplicate("150"));
  CU_ASSERT_TRUE(tree_has_key(tree, key));

  tree_remove(tree, key, NULL);
  CU_ASSERT_FALSE(tree_has_key(tree, key));

  release(tree);
  release(key);
}

void test_tree_get() {
  tree_t *tree = tree_new(NULL, NULL, NULL, comp_fun_int);
  retain(tree);

  int *number = new_integer(50);

  tree_insert(tree, number, number);

  obj elem;
  CU_ASSERT_TRUE(tree_get(tree, number, &elem));
  CU_ASSERT_EQUAL(*(int *)elem, 50);

  tree_insert(tree, new_integer(40), new_integer(40));
  tree_insert(tree, new_integer(20), new_integer(20));
  tree_insert(tree, new_integer(30), new_integer(30));
  tree_insert(tree, new_integer(25), new_integer(25));
  tree_insert(tree, new_integer(50), new_integer(50));
  tree_insert(tree, new_integer(45), new_integer(45));
  tree_insert(tree, new_integer(60), new_integer(60));

  int *key = new_integer(30);

  CU_ASSERT_TRUE(tree_get(tree, key, &elem));
  CU_ASSERT_EQUAL(*(int *)elem, 30);
  deallocate(key);

  key = new_integer(150);

  CU_ASSERT_FALSE(tree_get(tree, key, &elem));
  CU_ASSERT_EQUAL(*(int *)elem, 30);
  deallocate(key);

  key = new_integer(30);
  tree_remove(tree, key, NULL);
  CU_ASSERT_FALSE(tree_get(tree, key, &elem));

  release(tree);
}

void test_tree_remove() {
  tree_t *tree = tree_new(NULL, NULL, NULL, comp_fun_int);
  retain(tree);

  int **numbers = allocate_array(1000, sizeof(int*), NULL);

  // Testing inserting a lot of stuff
  for (int i = 0; i < 100; i++) {
    numbers[i] = new_integer(i);
    retain(numbers[i]);
    tree_insert(tree, numbers[i], numbers[i]);
  }

  obj elem;
  for (int i = 99; i >= 0; i--) {
    CU_ASSERT_TRUE(tree_remove(tree, numbers[i], &elem));
    CU_ASSERT_EQUAL(*(int*)elem, *numbers[i]);
  }

  CU_ASSERT_FALSE(tree_remove(tree, numbers[0], NULL));

  // Testing:
  /*
              40                       45                   45
            /    \                   /    \                /  \
           20     50      =>        20     50       =>    30  47
            \    /  \                \    /  \
            30  45   60              30  47  60
                 \
                  47

          depth: 4                   depth: 3            depth: 2
          Size: 7                    Size: 6             Size: 3
   */
  tree_insert(tree, numbers[40], numbers[20]);
  tree_insert(tree, numbers[20], numbers[20]);
  tree_insert(tree, numbers[30], numbers[20]);
  tree_insert(tree, numbers[45], numbers[20]);
  tree_insert(tree, numbers[50], numbers[20]);
  tree_insert(tree, numbers[47], numbers[20]);
  tree_insert(tree, numbers[60], numbers[20]);

  tree_remove(tree, numbers[40], NULL);

  CU_ASSERT_EQUAL(tree_size(tree), 6);
  CU_ASSERT_EQUAL(tree_depth(tree), 3);

  tree_remove(tree, numbers[20], NULL);
  tree_remove(tree, numbers[50], NULL);
  tree_remove(tree, numbers[60], NULL);

  CU_ASSERT_EQUAL(tree_size(tree), 3);
  CU_ASSERT_EQUAL(tree_depth(tree), 2);

  release(tree);
}

void test_tree_keys() {
  tree_t *tree = tree_new(NULL, NULL, NULL, comp_fun_int);
  retain(tree);

  for (int i = 0; i < 100; i++) {
    tree_insert(tree, new_integer(i), new_integer(i));
  }

  tree_key_t *keys = tree_keys(tree);

  for (int i = 0; i < tree_size(tree); i++) {
    int *key = (int *)keys[i];
    CU_ASSERT_EQUAL(*key, i);
  }

  release(tree);
}

void test_tree_elements() {
  tree_t *tree = tree_new(NULL, NULL, NULL, comp_fun_int);
  retain(tree);

  for (int i = 0; i < 100; i++) {
    tree_insert(tree, new_integer(i), new_integer(i));
  }

  obj *elems = tree_elements(tree);

  for (int i = 0; i < 100; i++) {
    int *elem = (int *)elems[i];
    CU_ASSERT_EQUAL(*elem, i);
  }

  release(tree);
}

void test_tree_apply() {
  tree_t *tree = tree_new(NULL, NULL, NULL, comp_fun_int);
  retain(tree);

  for (int i = 0; i < 100; i++) tree_insert(tree, new_integer(i), new_integer(i));

  CU_ASSERT_TRUE(tree_apply(tree, inorder, apply_fun, "Hello World"));

  release(tree);
}

void test_tree_balance() {
  tree_t *tree = tree_new(NULL, NULL, NULL, comp_fun_int);
  retain(tree);

  for (int i = 0; i < 10; i++) {
    tree_insert(tree, new_integer(i), new_integer(i));
  }

  CU_ASSERT_TRUE(tree_balance(tree));
  CU_ASSERT_FALSE(tree_balance(tree));

  release(tree);

  tree = tree_new(NULL, NULL, NULL, comp_fun_int);
  retain(tree);

  tree_insert(tree, new_integer(5), new_integer(5));
  tree_insert(tree, new_integer(1), new_integer(1));
  tree_insert(tree, new_integer(2), new_integer(2));
  tree_insert(tree, new_integer(15), new_integer(15));
  tree_insert(tree, new_integer(6), new_integer(6));
  tree_insert(tree, new_integer(7), new_integer(7));
  tree_insert(tree, new_integer(8), new_integer(8));
  tree_insert(tree, new_integer(14), new_integer(14));

  CU_ASSERT_TRUE(tree_balance(tree));

  release(tree);

  tree = tree_new(NULL, NULL, NULL, comp_fun_int);

  tree_insert(tree, new_integer(15), string_duplicate("bb"));
  tree_insert(tree, new_integer(17), string_duplicate("cc"));
  tree_insert(tree, new_integer(34), string_duplicate("dd"));
  tree_insert(tree, new_integer(1), string_duplicate("e"));
  tree_insert(tree, new_integer(6), string_duplicate("f"));
  tree_insert(tree, new_integer(7), string_duplicate("g"));
  tree_insert(tree, new_integer(83), string_duplicate("h"));
  tree_insert(tree, new_integer(19), string_duplicate("i"));
  tree_insert(tree, new_integer(88), string_duplicate("j"));
  tree_insert(tree, new_integer(89), string_duplicate("k"));
  tree_insert(tree, new_integer(5), string_duplicate("l"));
  tree_insert(tree, new_integer(4), string_duplicate("m"));

  CU_ASSERT_TRUE(tree_balance(tree));

  release(tree);
}

void test_tree_is_balanced() {
  tree_t *tree = tree_new(NULL, NULL, NULL, NULL);
  int depth = 0;

  CU_ASSERT_TRUE(tree_is_balanced(tree, &depth));
  CU_ASSERT_TRUE(depth <= 1);

  deallocate(tree);
}

int main(int argc, char *argv[]) {

  CU_initialize_registry();

  // Set up suites and tests
  CU_pSuite creation = CU_add_suite("Test creation, delete, size and depth", NULL, NULL);
  CU_add_test(creation, "Creation", test_tree_creation);
  CU_add_test(creation, "Delete", test_tree_delete);
  CU_add_test(creation, "Size", test_tree_size);
  CU_add_test(creation, "Depth", test_tree_depth);
  CU_add_test(creation, "Has key", test_tree_has_key);

  CU_pSuite manipulation = CU_add_suite("Test manipulation and retrieval", NULL, NULL);
  CU_add_test(manipulation, "Insert", test_tree_insert);
  CU_add_test(manipulation, "Get", test_tree_get);
  CU_add_test(manipulation, "Remove", test_tree_remove);

  CU_pSuite others = CU_add_suite("Test other functions", NULL, NULL);
  CU_add_test(others, "Keys", test_tree_keys);
  CU_add_test(others, "Elements", test_tree_elements);
  CU_add_test(others, "Apply", test_tree_apply);

  CU_pSuite balance = CU_add_suite("Test balancing", NULL, NULL);
  CU_add_test(balance, "Balance", test_tree_balance);
  CU_add_test(balance, "Is balanced", test_tree_is_balanced);

  CU_basic_run_tests();

  // Tear down
  CU_cleanup_registry();
  return CU_get_error();
}
