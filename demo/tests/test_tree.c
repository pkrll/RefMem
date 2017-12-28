#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Automated.h>
#include <stdlib.h>
#include <stdio.h>
#include "../tree.h"
#include "../common.h"

#define Elem_int(a) (elem_t) { .i = a }
#define Elem_ptr(a) (elem_t) { .p = a }
#define Elem_str(a) (elem_t) { .p = strdup(a) } // Så vi får en sträng på heapen

int comp_fun(elem_t a, elem_t b) {
  if (a.i == b.i) return 0;
  if (a.i >= 2 * b.i) return -1;

  return 1;
}

elem_t copy_fun(elem_t e) {
  CU_PASS("Copy");
  return e;
}

void free_fun(elem_t e) {
  CU_PASS("Free");
  free(e.p);
}

bool apply_fun(elem_t key, elem_t elem, void *data) {
  CU_ASSERT_EQUAL(data, "Hello World");
  return (elem.i > 5);
}

// -------------------------------
// Tests
// -------------------------------

void test_tree_creation() {
  tree_t *tree = tree_new(NULL, NULL, NULL, NULL);

  CU_ASSERT_PTR_NOT_NULL(tree);
  CU_ASSERT_EQUAL(tree_size(tree), 0);
  
  tree_delete(tree, false, false);
}

void test_tree_delete() {
  tree_t *tree = tree_new(NULL, free_fun, free_fun, NULL);

  tree_insert(tree, Elem_str("Foo"), Elem_str("Foo"));
  tree_insert(tree, Elem_str("Bar"), Elem_str("Bar"));
  tree_insert(tree, Elem_str("Baz"), Elem_str("Baz"));
  tree_insert(tree, Elem_str("Bay"), Elem_str("Bay"));

  tree_delete(tree, true, true);
  // As there seems to be no way to test free, we use CU_PASS
  // our own "free" functions above.
}

void test_tree_size() {
  tree_t *tree = tree_new(NULL, NULL, NULL, NULL);
  // A pretty unbalanced tree
  for (int i = 0; i < 10; i++) {
    tree_insert(tree, Elem_int(i), Elem_int(i));
    CU_ASSERT_EQUAL(tree_size(tree), i + 1);
  }
  
  elem_t elem;
  
  tree_remove(tree, Elem_int(9), &elem);
  CU_ASSERT_EQUAL(tree_size(tree), 9);

  tree_remove(tree, Elem_int(8), &elem);
  CU_ASSERT_EQUAL(tree_size(tree), 8);
  
  tree_insert(tree, Elem_int(8), Elem_int(8));
  CU_ASSERT_EQUAL(tree_size(tree), 9);

  for (int i = 0; i < 9; i++) {
    tree_remove(tree, Elem_int(i), &elem);
  }

  CU_ASSERT_EQUAL(tree_size(tree), 0);
  // Somewhat balanced
  tree_insert(tree, Elem_int(40), Elem_int(40));
  tree_insert(tree, Elem_int(20), Elem_int(20));
  tree_insert(tree, Elem_int(30), Elem_int(30));
  tree_insert(tree, Elem_int(25), Elem_int(25));
  tree_insert(tree, Elem_int(50), Elem_int(50));
  tree_insert(tree, Elem_int(45), Elem_int(45));
  tree_insert(tree, Elem_int(60), Elem_int(60));
  
  CU_ASSERT_EQUAL(tree_size(tree), 7);
  
  tree_delete(tree, false, false);
}

void test_tree_depth() {
  tree_t *tree = tree_new(NULL, NULL, NULL, NULL);
  // testing right side
  for (int index = 0; index < 10; index++) {
    tree_insert(tree, Elem_int(index), Elem_int(index));
  }

  CU_ASSERT_EQUAL(tree_depth(tree), 10);

  for (int index = 0; index < 10; index++) {
    tree_remove(tree, Elem_int(index), NULL);
  }

  CU_ASSERT_EQUAL(tree_depth(tree), 0);
  
  // testing left side
  for (int index = 10; index > 0; index--) {
    tree_insert(tree, Elem_int(index), Elem_int(index));
  }

  CU_ASSERT_EQUAL(tree_depth(tree), 10);

  for (int index = 10; index > 0; index--) {
    tree_remove(tree, Elem_int(index), NULL);
  }

  CU_ASSERT_EQUAL(tree_depth(tree), 0);
  
  // More normal tree
  tree_insert(tree, Elem_int(40), Elem_int(40));
  tree_insert(tree, Elem_int(20), Elem_int(20));
  tree_insert(tree, Elem_int(32), Elem_int(32));
  tree_insert(tree, Elem_int(75), Elem_int(75));
  tree_insert(tree, Elem_int(50), Elem_int(50));
  tree_insert(tree, Elem_int(45), Elem_int(45));
  tree_insert(tree, Elem_int(60), Elem_int(60));

  CU_ASSERT_EQUAL(tree_depth(tree), 4);

  tree_delete(tree, false, false);
}

void test_tree_insert() {  
  // Without compare, copy functions
  tree_t *tree = tree_new(NULL, NULL, NULL, NULL);
  // Testing inserting a lot of stuff
  for (int index = 0; index < 1000; index++) {
    CU_ASSERT_TRUE(tree_insert(tree, Elem_int(index), Elem_int(index)));
    CU_ASSERT_EQUAL(tree_size(tree), index + 1);
  }

  CU_ASSERT_TRUE(tree_has_key(tree, Elem_int(0)));
  CU_ASSERT_TRUE(tree_has_key(tree, Elem_int(200)));
  
  tree_remove(tree, Elem_int(10), NULL);
  // Insert after remove
  CU_ASSERT_TRUE(tree_insert(tree, Elem_int(1000), Elem_int(1000)));
  CU_ASSERT_EQUAL(tree_size(tree), 1000);
  // Insert existing key
  CU_ASSERT_FALSE(tree_insert(tree, Elem_int(600), Elem_int(600)));
  CU_ASSERT_EQUAL(tree_size(tree), 1000);
  CU_ASSERT_TRUE(tree_has_key(tree, Elem_int(600)));
  
  tree_delete(tree, false, false);

  // With copy, compare functions
  tree = tree_new(copy_fun, NULL, NULL, comp_fun);

  CU_ASSERT_TRUE(tree_insert(tree, Elem_int(10), Elem_int(10)));
  CU_ASSERT_TRUE(tree_insert(tree, Elem_int(5), Elem_int(5)));
  CU_ASSERT_TRUE(tree_insert(tree, Elem_int(15), Elem_int(15)));
  CU_ASSERT_TRUE(tree_insert(tree, Elem_int(8), Elem_int(8)));
  
  CU_ASSERT_EQUAL(tree_size(tree), 4);

  tree_remove(tree, Elem_int(10), NULL);
  tree_remove(tree, Elem_int(5), NULL);
  tree_remove(tree, Elem_int(15), NULL);
  tree_remove(tree, Elem_int(8), NULL);

  CU_ASSERT_TRUE(tree_insert(tree, Elem_int(10), Elem_int(10)));
  CU_ASSERT_EQUAL(tree_size(tree), 1);
  
  tree_delete(tree, false, false);
}

void test_tree_has_key() {
  tree_t *tree = tree_new(NULL, NULL, NULL, NULL);

  tree_insert(tree, Elem_int(150), Elem_int(150));
  CU_ASSERT_TRUE(tree_has_key(tree, Elem_int(150)));

  tree_remove(tree, Elem_int(150), NULL);
  CU_ASSERT_FALSE(tree_has_key(tree, Elem_int(150)));
  
  tree_delete(tree, false, false);
}

void test_tree_get() {
  tree_t *tree = tree_new(NULL, NULL, NULL, NULL);
  elem_t elem = Elem_int(0);
  
  tree_insert(tree, Elem_int(50), Elem_int(50));
  
  CU_ASSERT_TRUE(tree_get(tree, Elem_int(50), &elem));
  CU_ASSERT_EQUAL(elem.i, 50);
  
  tree_insert(tree, Elem_int(40), Elem_int(40));
  tree_insert(tree, Elem_int(20), Elem_int(20));
  tree_insert(tree, Elem_int(30), Elem_int(30));
  tree_insert(tree, Elem_int(25), Elem_int(25));
  tree_insert(tree, Elem_int(50), Elem_int(50));
  tree_insert(tree, Elem_int(45), Elem_int(45));
  tree_insert(tree, Elem_int(60), Elem_int(60));

  CU_ASSERT_TRUE(tree_get(tree, Elem_int(30), &elem));
  CU_ASSERT_EQUAL(elem.i, 30);

  CU_ASSERT_FALSE(tree_get(tree, Elem_int(150), &elem));
  CU_ASSERT_EQUAL(elem.i, 30);

  tree_remove(tree, Elem_int(30), NULL);
  CU_ASSERT_FALSE(tree_get(tree, Elem_int(30), &elem));

  tree_delete(tree, false, false);
}

void test_tree_remove() {
  tree_t *tree = tree_new(NULL, free_fun, NULL, NULL);
  elem_t elem = Elem_str("Foo");

  tree_insert(tree, elem, Elem_int(0));
  CU_ASSERT_TRUE(tree_remove(tree, elem, NULL)); // Testing remove and free key
  CU_ASSERT_EQUAL(tree_size(tree), 0);

  tree_delete(tree, false, false);
  
  tree = tree_new(NULL, NULL, NULL, NULL);
  
  for (int i = 0; i < 100; i++) {
    tree_insert(tree, Elem_int(i), Elem_str("Some string"));
  }

  for (int i = 99; i >= 0; i--) {
    CU_ASSERT_TRUE(tree_remove(tree, Elem_int(i), &elem));
    CU_ASSERT_STRING_EQUAL(elem.p, "Some string");
    /* free(elem.p); */
  }

  CU_ASSERT_FALSE(tree_remove(tree, Elem_int(0), NULL));

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
  tree_insert(tree, Elem_int(40), Elem_int(20));
  tree_insert(tree, Elem_int(20), Elem_int(20));
  tree_insert(tree, Elem_int(30), Elem_int(20));
  tree_insert(tree, Elem_int(45), Elem_int(20));
  tree_insert(tree, Elem_int(50), Elem_int(20));
  tree_insert(tree, Elem_int(47), Elem_int(20));
  tree_insert(tree, Elem_int(60), Elem_int(20));
  
  tree_remove(tree, Elem_int(40), NULL);
  // TODO: KOlla om elementet också försvunnit med tree_get
  CU_ASSERT_EQUAL(tree_size(tree), 6);
  CU_ASSERT_EQUAL(tree_depth(tree), 3);

  tree_remove(tree, Elem_int(20), NULL);
  tree_remove(tree, Elem_int(50), NULL);
  tree_remove(tree, Elem_int(60), NULL);

  CU_ASSERT_EQUAL(tree_size(tree), 3);
  CU_ASSERT_EQUAL(tree_depth(tree), 2);  
  
  tree_delete(tree, false, false);
  
}

void test_tree_keys() {
  tree_t *tree = tree_new(NULL, NULL, NULL, NULL);

  for (int i = 0; i < 100; i++) tree_insert(tree, Elem_int(i), Elem_int(i));

  tree_key_t *keys = tree_keys(tree);

  for (int i = 0; i < tree_size(tree); i++) CU_ASSERT_EQUAL(keys[i].i, i);
  
  tree_delete(tree, false, false);
}

void test_tree_elements() {
  tree_t *tree = tree_new(NULL, NULL, NULL, NULL);

  for (int i = 0; i < 100; i++) tree_insert(tree, Elem_int(i), Elem_int(i));

  elem_t *elems = tree_elements(tree);

  for (int i = 0; i < 100; i++) CU_ASSERT_EQUAL(elems[i].i, i);

  tree_delete(tree, false, false);
}

void test_tree_apply() {
  tree_t *tree = tree_new(NULL, NULL, NULL, NULL);

  for (int i = 0; i < 100; i++) tree_insert(tree, Elem_int(i), Elem_int(i));

  CU_ASSERT_TRUE(tree_apply(tree, inorder, apply_fun, "Hello World"));

  tree_delete(tree, false, false);
}

void test_tree_balance() {
  tree_t *tree = tree_new(NULL, NULL, NULL, NULL);
  
  for (int i = 0; i < 10; i++) {
    tree_insert(tree, Elem_int(i), Elem_int(i));
  }
  
  CU_ASSERT_TRUE(tree_balance(tree));
  CU_ASSERT_FALSE(tree_balance(tree));

  tree_delete(tree, false, false);

  tree = tree_new(NULL, NULL, NULL, NULL);

  tree_insert(tree, Elem_int(5), Elem_int(5));
  tree_insert(tree, Elem_int(1), Elem_int(1));
  tree_insert(tree, Elem_int(2), Elem_int(2));
  tree_insert(tree, Elem_int(15), Elem_int(15));
  tree_insert(tree, Elem_int(6), Elem_int(6));
  tree_insert(tree, Elem_int(7), Elem_int(7));
  tree_insert(tree, Elem_int(8), Elem_int(8));
  tree_insert(tree, Elem_int(14), Elem_int(14));

  CU_ASSERT_TRUE(tree_balance(tree));

  tree_delete(tree, false, false);

  tree = tree_new(NULL, free_fun, free_fun, NULL);

  tree_insert(tree, Elem_int(15), Elem_str("bb"));
  tree_insert(tree, Elem_int(17), Elem_str("cc"));
  tree_insert(tree, Elem_int(34), Elem_str("dd"));
  tree_insert(tree, Elem_int(1), Elem_str("e"));
  tree_insert(tree, Elem_int(6), Elem_str("f"));
  tree_insert(tree, Elem_int(7), Elem_str("g"));
  tree_insert(tree, Elem_int(83), Elem_str("h"));
  tree_insert(tree, Elem_int(19), Elem_str("i"));
  tree_insert(tree, Elem_int(88), Elem_str("j"));
  tree_insert(tree, Elem_int(89), Elem_str("k"));
  tree_insert(tree, Elem_int(5), Elem_str("l"));
  tree_insert(tree, Elem_int(4), Elem_str("m"));
  
  CU_ASSERT_TRUE(tree_balance(tree));

  tree_delete(tree, false, true);
}

void test_tree_is_balanced() {
  tree_t *tree = tree_new(NULL, NULL, NULL, NULL);
  int depth = 0;
  
  CU_ASSERT_TRUE(tree_is_balanced(tree, &depth));
  CU_ASSERT_TRUE(depth <= 1);
  
  tree_delete(tree, false, false);
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
