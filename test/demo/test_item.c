#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Automated.h>
#include <stdlib.h>
#include "../../demo/utils.h"
#include "../../demo/item.h"
#include "../../demo/list.h"
#include "../../src/refmem.h"

// -------------------------------
// Tests
// -------------------------------

bool test_item_shelf_apply_fun(char *shelf, int amount, void *data) {
  int *test_data = data;
  (*test_data)++;

  return (strcmp(shelf, "A1") == 0 && amount == 1);
}

void test_item_new() {
  item_t *item = item_new();
  retain(item);

  CU_ASSERT_PTR_NOT_NULL(item);

  CU_ASSERT_PTR_NULL(item_get_name(item));
  CU_ASSERT_PTR_NULL(item_get_desc(item));
  CU_ASSERT_EQUAL(item_get_cost(item), 0);

  release(item);
}

void test_item_copy() {
  item_t *item = item_new();
  retain(item);

  char *name = "Foo";
  char *desc = "Bar";
  int cost = 100;
  char *shelf = "F1";
  int amount = 10000;

  item_set_name(item, name);
  item_set_desc(item, desc);
  item_set_cost(item, cost);
  item_set_shelves(item, shelf, amount, false);

  item_t *copied_item = item_copy(item);
  retain(copied_item);

  CU_ASSERT_STRING_EQUAL(item_get_name(copied_item), name);
  CU_ASSERT_STRING_EQUAL(item_get_desc(copied_item), desc);
  CU_ASSERT_EQUAL(item_get_cost(copied_item), cost);
  CU_ASSERT_TRUE(item_has_shelf(copied_item, shelf));

  CU_ASSERT_PTR_NOT_EQUAL(item, copied_item);

  // Testing changing properties of the original item
  name = "Hello";
  desc = "World";
  cost = 99999;
  shelf = "F2";
  amount = 200;

  item_set_name(item, name);
  item_set_desc(item, desc);
  item_set_cost(item, cost);
  item_set_shelves(item, shelf, amount, false);

  CU_ASSERT_STRING_EQUAL(item_get_name(item), name);
  CU_ASSERT_STRING_EQUAL(item_get_desc(item), desc);
  CU_ASSERT_EQUAL(item_get_cost(item), cost);
  CU_ASSERT_TRUE(item_has_shelf(item, shelf));

  CU_ASSERT_STRING_NOT_EQUAL(item_get_name(copied_item), name);
  CU_ASSERT_STRING_NOT_EQUAL(item_get_desc(copied_item), desc);
  CU_ASSERT_NOT_EQUAL(item_get_cost(copied_item), cost);
  CU_ASSERT_FALSE(item_has_shelf(copied_item, shelf));

  // Testing deleting the old item
  release(item);

  name = "Foo";
  desc = "Bar";
  cost = 100;
  shelf = "F1";
  amount = 10000;

  CU_ASSERT_STRING_EQUAL(item_get_name(copied_item), name);
  CU_ASSERT_STRING_EQUAL(item_get_desc(copied_item), desc);
  CU_ASSERT_EQUAL(item_get_cost(copied_item), cost);
  CU_ASSERT_TRUE(item_has_shelf(copied_item, shelf));

  release(copied_item);
}

void test_item_total() {
  item_t *item = item_new();
  retain(item);

  int A1 = 10;
  int A2 = 20;
  int A3 = 30;
  int A4 = 40;

  item_set_shelves(item, "A1", A1, false);
  item_set_shelves(item, "A2", A2, false);
  item_set_shelves(item, "A3", A3, false);
  item_set_shelves(item, "A4", A4, false);
  CU_ASSERT_EQUAL(item_total(item), A1 + A2 + A3 + A4);

  item_set_shelves(item, "A1", 0, true);
  CU_ASSERT_EQUAL(item_total(item), A2 + A3 + A4);

  item_set_shelves(item, "A2", 0, true);
  item_set_shelves(item, "A3", 0, true);
  item_set_shelves(item, "A4", 0, true);
  CU_ASSERT_EQUAL(item_total(item), 0);

  item_set_shelves(item, "A5", 50, false);
  CU_ASSERT_EQUAL(item_total(item), 50);

  release(item);
}

void test_item_set_name() {
  item_t *item = item_new();
  retain(item);

  CU_ASSERT_FALSE(item_set_name(item, ""));

  char *name = calloc(3, sizeof(char*));
  name = "Foo";

  CU_ASSERT_TRUE(item_set_name(item, name));
  CU_ASSERT_STRING_EQUAL(item_get_name(item), name);

  name = "Bar";
  CU_ASSERT_STRING_NOT_EQUAL(item_get_name(item), name);

  name = "Baz";
  CU_ASSERT_TRUE(item_set_name(item, name));
  CU_ASSERT_STRING_EQUAL(item_get_name(item), name);

  release(item);
}

void test_item_get_name() {
  item_t *item = item_new();
  retain(item);

  CU_ASSERT_PTR_NULL(item_get_name(item));

  item_set_name(item, "Hello");
  CU_ASSERT_STRING_EQUAL(item_get_name(item), "Hello");

  release(item);
}

void test_item_set_desc() {
  item_t *item = item_new();
  retain(item);

  CU_ASSERT_FALSE(item_set_desc(item, ""));

  char *desc = calloc(3, sizeof(char*));
  desc = "Foo";

  CU_ASSERT_TRUE(item_set_desc(item, desc));
  CU_ASSERT_STRING_EQUAL(item_get_desc(item), desc);

  desc = "Bar";
  CU_ASSERT_STRING_NOT_EQUAL(item_get_desc(item), desc);

  desc = "Baz";
  CU_ASSERT_TRUE(item_set_desc(item, desc));
  CU_ASSERT_STRING_EQUAL(item_get_desc(item), desc);

  release(item);
}

void test_item_get_desc() {
  item_t *item = item_new();
  retain(item);

  CU_ASSERT_PTR_NULL(item_get_desc(item));

  item_set_desc(item, "Hello");
  CU_ASSERT_STRING_EQUAL(item_get_desc(item), "Hello");

  release(item);
}

void test_item_set_cost() {
  item_t *item = item_new();
  retain(item);

  CU_ASSERT_FALSE(item_set_cost(item, -15));

  CU_ASSERT_TRUE(item_set_cost(item, 1500));
  CU_ASSERT_EQUAL(item_get_cost(item), 1500);

  CU_ASSERT_TRUE(item_set_cost(item, 0));
  CU_ASSERT_EQUAL(item_get_cost(item), 0);

  CU_ASSERT_FALSE(item_set_cost(item, -150));
  CU_ASSERT_EQUAL(item_get_cost(item), 0);

  CU_ASSERT_TRUE(item_set_cost(item, 10000000));
  CU_ASSERT_EQUAL(item_get_cost(item), 10000000);

  release(item);
}

void test_item_get_cost() {
  item_t *item = item_new();
  retain(item);

  CU_ASSERT_EQUAL(item_get_cost(item), 0);

  item_set_cost(item, 100);
  CU_ASSERT_EQUAL(item_get_cost(item), 100);

  item_set_cost(item, 150);
  CU_ASSERT_NOT_EQUAL(item_get_cost(item), 100);

  release(item);
}

void test_item_set_shelves() {
  item_t *item = item_new();
  retain(item);

  CU_ASSERT_FALSE(item_has_shelf(item, "A1"));

  char *shelf = calloc(2, sizeof(char*));
  shelf = "A1";

  item_set_shelves(item, shelf, 150, false);
  CU_ASSERT_TRUE(item_has_shelf(item, "A1"));

  shelf = "A2";
  CU_ASSERT_TRUE(item_has_shelf(item, "A1"));
  CU_ASSERT_FALSE(item_has_shelf(item, "A2"));

  item_set_shelves(item, "A1", 0, false);
  CU_ASSERT_FALSE(item_has_shelf(item, "A1"));

  item_set_shelves(item, "J1", -5, false);
  CU_ASSERT_FALSE(item_has_shelf(item, "J1"));

  item_set_shelves(item, "A1", 100, false);
  item_set_shelves(item, "J5", 150, false);
  CU_ASSERT_EQUAL(item_total(item), 250);

  item_set_shelves(item, "A1", 50, false);
  CU_ASSERT_EQUAL(item_total(item), 300);

  item_set_shelves(item, "A1", 50, true);
  CU_ASSERT_EQUAL(item_total(item), 200);

  item_set_shelves(item, "", 150, false);
  CU_ASSERT_EQUAL(item_total(item), 200);

  item_set_shelves(item, "A1", 0, false);
  item_set_shelves(item, "J5",  0, false);
  CU_ASSERT_EQUAL(item_total(item), 0);

  item_set_shelves(item, "I8", 15, true);
  CU_ASSERT_EQUAL(item_total(item), 15);

  release(item);
}

void test_item_has_shelf() {
  item_t *item = item_new();
  retain(item);

  char *shelf = string_duplicate("A1");
  retain(shelf);

  item_set_shelves(item, shelf, 5, false);
  CU_ASSERT_TRUE(item_has_shelf(item, shelf));

  char *shelf2 = string_duplicate("A2");
  retain(shelf);
  CU_ASSERT_TRUE(item_has_shelf(item, "A1"));
  CU_ASSERT_FALSE(item_has_shelf(item, shelf2));

  item_set_shelves(item, "A1", 0, false);
  CU_ASSERT_FALSE(item_has_shelf(item, "A1"));

  release(item);
  release(shelf);
  release(shelf2);
}

void test_item_apply_on_shelves() {
  item_t *item = item_new();
  retain(item);

  item_set_shelves(item, "A1", 1, false);
  item_set_shelves(item, "A2", 2, false);
  item_set_shelves(item, "A3", 3, false);
  item_set_shelves(item, "A4", 4, false);

  int index = 0;
  CU_ASSERT_TRUE(item_apply_on_shelves(item, test_item_shelf_apply_fun, &index));
  CU_ASSERT_EQUAL(index, 4);

  item_set_shelves(item, "A1", 0, false);
  index = 0;
  CU_ASSERT_FALSE(item_apply_on_shelves(item, test_item_shelf_apply_fun, &index));
  CU_ASSERT_EQUAL(index, 3);

  release(item);
}

int main(int argc, char *argv[]) {

  CU_initialize_registry();

  // Set up suites and tests
  CU_pSuite creation = CU_add_suite("Test creation and delete", NULL, NULL);
  CU_add_test(creation, "Creation", test_item_new);
  CU_add_test(creation, "Copy", test_item_copy);

  CU_pSuite properties = CU_add_suite("Test properties", NULL, NULL);
  CU_add_test(properties, "Total", test_item_total);
  CU_add_test(properties, "Set name", test_item_set_name);
  CU_add_test(properties, "Get name", test_item_get_name);
  CU_add_test(properties, "Set desc", test_item_set_desc);
  CU_add_test(properties, "Get desc", test_item_get_desc);
  CU_add_test(properties, "Set cost", test_item_set_cost);
  CU_add_test(properties, "Get cost", test_item_get_cost);
  CU_add_test(properties, "Set shelves", test_item_set_shelves);
  CU_add_test(properties, "Has shelf", test_item_has_shelf);
  CU_add_test(properties, "Apply", test_item_apply_on_shelves);

  CU_basic_run_tests();

  // Tear down
  CU_cleanup_registry();

  return CU_get_error();
}
