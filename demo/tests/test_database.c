#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Automated.h>
#include <stdlib.h>
#include "../database.h"
#include "../item.h"

item_t *create_item(char *name, char *desc, int cost, char *shelf, int amount) {
  item_t *item = item_new();

  item_set_name(item, name);
  item_set_desc(item, desc);
  item_set_cost(item, cost);
  item_set_shelves(item, shelf, amount, false);

  return item;
}

void test_database_new() {
  database_t *database = database_new(NULL);
  retain(database);

  CU_ASSERT_PTR_NOT_NULL(database);

  release(database);
}

void test_database_insert_item() {
  database_t *database = database_new(NULL);
  retain(database);

  item_t *item_add = create_item("Test Name", "Test Desc", 1, "A1", 2);

  retain(item_add);

  CU_ASSERT_TRUE(database_insert_item(database, item_add));
  CU_ASSERT_FALSE(database_insert_item(database, item_add));

  item_t *item_get = database_get_item(database, "Test Name");
  CU_ASSERT_STRING_EQUAL(item_get_name(item_get), "Test Name");

  CU_ASSERT_FALSE(database_insert_item(database, NULL));

  database_remove_item(database, item_get);
  item_get = database_get_item(database, "Test Name");
  CU_ASSERT_PTR_NULL(item_get);

  release(item_add);

  item_add = create_item("Test Name 2", "Test Desc 2", 2, "A2", 4);
  CU_ASSERT_TRUE(database_insert_item(database, item_add));
  item_add = create_item("Test Name 3", "Test Desc 2", 2, "A3", 4);
  CU_ASSERT_TRUE(database_insert_item(database, item_add));
  item_add = create_item("Test Name 4", "Test Desc 2", 2, "A4", 4);
  CU_ASSERT_TRUE(database_insert_item(database, item_add));
  item_add = create_item("Test Name 5", "Test Desc 2", 2, "A5", 4);
  CU_ASSERT_TRUE(database_insert_item(database, item_add));

  item_get = database_get_item(database, "Test Name 2");
  CU_ASSERT_STRING_EQUAL(item_get_name(item_get), "Test Name 2");
  item_get = database_get_item(database, "Test Name 3");
  CU_ASSERT_STRING_EQUAL(item_get_name(item_get), "Test Name 3");
  item_get = database_get_item(database, "Test Name 4");
  CU_ASSERT_STRING_EQUAL(item_get_name(item_get), "Test Name 4");
  item_get = database_get_item(database, "Test Name 5");
  CU_ASSERT_STRING_EQUAL(item_get_name(item_get), "Test Name 5");

  release(database);
}

void test_database_update_item() {
  database_t *database = database_new(NULL);
  retain(database);

  item_t *item = create_item("Test Item", "Test Desc", 15, "A5", 1);
  database_insert_item(database, item);

  item = database_get_item(database, "Test Item");
  item_t *edit = item_copy(item);

  item_set_name(edit, "Edited Item");
  CU_ASSERT_TRUE(database_update_item(database, item, edit));

  item = database_get_item(database, "Edited Item");
  CU_ASSERT_STRING_EQUAL(item_get_name(item), "Edited Item");

  item = database_get_item(database, "Test Item");
  CU_ASSERT_PTR_NULL(item);

  // Trying to update a non existent item
  item = create_item("Foo", "Bar", 1, "F5", 5);
  edit = item_copy(item);

  CU_ASSERT_FALSE(database_update_item(database, item, edit));

  database_insert_item(database, item);
  item = database_get_item(database, "Foo");
  CU_ASSERT_TRUE(database_update_item(database, item, edit));

  release(database);
}

void test_database_remove_item() {
  database_t *database = database_new(NULL);
  retain(database);

  item_t *item_1 = create_item("Item 1", "Desc 1", 2, "A1", 1);
  item_t *item_2 = create_item("Item 2", "Desc 2", 4, "A2", 2);
  item_t *item_3 = create_item("Item 3", "Desc 3", 8, "A3", 3);
  item_t *item_4 = create_item("Item 4", "Desc 4", 16, "A4", 4);
  item_t *item_5 = create_item("Item 5", "Desc 5", 32, "A5", 5);

  database_insert_item(database, item_1);
  database_insert_item(database, item_2);
  database_insert_item(database, item_3);
  database_insert_item(database, item_4);
  database_insert_item(database, item_5);

  item_1 = database_get_item(database, "Item 1");
  item_2 = database_get_item(database, "Item 2");
  item_3 = database_get_item(database, "Item 3");
  item_4 = database_get_item(database, "Item 4");
  item_5 = database_get_item(database, "Item 5");

  CU_ASSERT_TRUE(database_remove_item(database, item_1));
  CU_ASSERT_PTR_NULL(database_get_item(database, item_get_name(item_1)));
  CU_ASSERT_TRUE(database_remove_item(database, item_2));
  CU_ASSERT_PTR_NULL(database_get_item(database, item_get_name(item_2)));
  CU_ASSERT_TRUE(database_remove_item(database, item_3));
  CU_ASSERT_PTR_NULL(database_get_item(database, item_get_name(item_3)));
  CU_ASSERT_TRUE(database_remove_item(database, item_4));
  CU_ASSERT_PTR_NULL(database_get_item(database, item_get_name(item_4)));
  CU_ASSERT_TRUE(database_remove_item(database, item_5));
  CU_ASSERT_PTR_NULL(database_get_item(database, item_get_name(item_5)));

  database_insert_item(database, item_copy(item_1));
  item_1 = database_get_item(database, item_get_name(item_1));
  CU_ASSERT_STRING_EQUAL(item_get_name(item_1), "Item 1");

  CU_ASSERT_FALSE(database_remove_item(database, item_2));

  release(database);
}

void test_database_has_item() {
  database_t *database = database_new(NULL);
  retain(database);

  item_t *item_1 = create_item("Item 1", "Desc 1", 4, "A1", 1);
  item_t *item_2 = create_item("Item 2", "Desc 2", 4, "A2", 1);
  item_t *item_3 = create_item("Item 3", "Desc 3", 4, "A3", 1);
  item_t *item_4 = create_item("Item 4", "Desc 4", 4, "A4", 1);

  database_insert_item(database, item_1);
  database_insert_item(database, item_2);
  database_insert_item(database, item_3);
  database_insert_item(database, item_4);

  CU_ASSERT_TRUE(database_has_item(database, "Item 1"));
  CU_ASSERT_TRUE(database_has_item(database, "Item 2"));
  CU_ASSERT_TRUE(database_has_item(database, "Item 3"));
  CU_ASSERT_TRUE(database_has_item(database, "Item 4"));
  CU_ASSERT_FALSE(database_has_item(database, "Item 5"));

  item_1 = database_get_item(database, "Item 1");
  CU_ASSERT_TRUE(database_has_item(database, "Item 1"));

  database_remove_item(database, item_1);
  CU_ASSERT_FALSE(database_has_item(database, "Item 1"));

  release(database);
}

void test_database_get_item() {
  database_t *database = database_new(NULL);
  retain(database);

  item_t *item_1 = create_item("Item 1", "Desc 1", 4, "A1", 1);
  item_t *item_2 = create_item("Item 2", "Desc 2", 4, "A2", 1);
  item_t *item_3 = create_item("Item 3", "Desc 3", 4, "A3", 1);
  item_t *item_4 = create_item("Item 4", "Desc 4", 4, "A4", 1);

  database_insert_item(database, item_1);
  database_insert_item(database, item_2);
  database_insert_item(database, item_3);
  database_insert_item(database, item_4);

  item_t *item_get_1 = database_get_item(database, "Item 1");
  item_t *item_get_2 = database_get_item(database, "Item 2");
  item_t *item_get_3 = database_get_item(database, "Item 3");
  item_t *item_get_4 = database_get_item(database, "Item 4");

  CU_ASSERT_STRING_EQUAL(item_get_name(item_get_1), "Item 1");
  CU_ASSERT_STRING_EQUAL(item_get_name(item_get_2), "Item 2");
  CU_ASSERT_STRING_EQUAL(item_get_name(item_get_3), "Item 3");
  CU_ASSERT_STRING_EQUAL(item_get_name(item_get_4), "Item 4");

  database_remove_item(database, item_get_1);
  item_get_1 = database_get_item(database, "Item 1");
  CU_ASSERT_PTR_NULL(item_get_1);

  release(database);
}

void test_database_location_is_valid() {
  database_t *database = database_new(NULL);
  retain(database);

  item_t *item_1 = create_item("Item 1", "Desc 1", 42, "A1", 12);
  item_t *item_2 = create_item("Item 2", "Desc 2", 42, "A2", 13);

  database_insert_item(database, item_1);
  database_insert_item(database, item_2);

  CU_ASSERT_TRUE(database_location_is_valid(database, "Item 1", "A1"));
  CU_ASSERT_TRUE(database_location_is_valid(database, "Item 1", "A3"));
  CU_ASSERT_FALSE(database_location_is_valid(database, "Item 1", "A2"));

  release(database);
}

void test_database_undo_action() {
  database_t *database = database_new(NULL);
  retain(database);

  item_t *item_1 = create_item("Item 1", "Desc 1", 42, "A1", 5);
  item_t *item_2 = create_item("Item 2", "Desc 2", 42, "A2", 5);
  item_t *item_2_copy = item_copy(item_2);

  database_insert_item(database, item_1);
  database_insert_item(database, item_2);

  item_set_desc(item_2_copy, "Edited");

  item_2 = database_get_item(database, "Item 2");
  CU_ASSERT_STRING_EQUAL(item_get_desc(item_2), "Desc 2");
  database_update_item(database, item_2, item_2_copy);

  item_2 = database_get_item(database, "Item 2");
  CU_ASSERT_STRING_EQUAL(item_get_desc(item_2), "Edited");

  CU_ASSERT_TRUE(database_undo_action(database));

  item_2 = database_get_item(database, "Item 2");
  CU_ASSERT_STRING_EQUAL(item_get_desc(item_2), "Desc 2");

  CU_ASSERT_TRUE(database_undo_action(database));
  CU_ASSERT_TRUE(database_undo_action(database));

  item_1 = database_get_item(database, "Item 1");
  item_2 = database_get_item(database, "Item 2");

  CU_ASSERT_PTR_NULL(item_1);
  CU_ASSERT_PTR_NULL(item_2);

  item_1 = create_item("Item 1.1", "Desc 1.1", 42, "A5", 5);
  database_insert_item(database, item_1);

  CU_ASSERT_TRUE(database_undo_action(database));
  CU_ASSERT_FALSE(database_undo_action(database));

  item_t *item_3 = create_item("Item 3", "Desc 3", 42, "A3", 5);
  database_insert_item(database, item_3);
  item_3 = database_get_item(database, "Item 3");
  database_remove_item(database, item_3);

  CU_ASSERT_TRUE(database_undo_action(database));
  item_3 = database_get_item(database, "Item 3");
  CU_ASSERT_STRING_EQUAL(item_get_desc(item_3), "Desc 3");

  release(database);
}

void test_database_is_sorted() {
  database_t *database = database_new(NULL);
  retain(database);

  CU_ASSERT_TRUE(database_is_sorted(database));

  item_t *item_1 = create_item("A", "Desc A", 42, "A1", 2);
  item_t *item_2 = create_item("B", "Desc B", 42, "A2", 2);
  item_t *item_3 = create_item("C", "Desc C", 42, "A3", 2);

  database_insert_item(database, item_1);
  CU_ASSERT_TRUE(database_is_sorted(database));
  database_insert_item(database, item_2);
  CU_ASSERT_TRUE(database_is_sorted(database));
  database_insert_item(database, item_3);
  CU_ASSERT_FALSE(database_is_sorted(database));

  database_sort(database);
  CU_ASSERT_TRUE(database_is_sorted(database));

  database_insert_item(database, create_item("D", "Desc D", 42, "A4", 2));
  database_insert_item(database, create_item("E", "Desc E", 42, "A5", 2));
  database_insert_item(database, create_item("F", "Desc F", 42, "A6", 2));
  database_insert_item(database, create_item("G", "Desc G", 42, "A7", 2));

  CU_ASSERT_FALSE(database_is_sorted(database));

  release(database);
}

void test_database_sort() {
  database_t *database = database_new(NULL);
  retain(database);

  CU_ASSERT_FALSE(database_sort(database));

  database_insert_item(database, create_item("A", "Desc A", 42, "A1", 2));
  database_insert_item(database, create_item("B", "Desc B", 42, "A2", 2));
  database_insert_item(database, create_item("C", "Desc C", 42, "A3", 2));

  CU_ASSERT_TRUE(database_sort(database));

  database_insert_item(database, create_item("G", "Desc G", 42, "A4", 2));
  CU_ASSERT_FALSE(database_sort(database));

  item_t *item_h = create_item("H", "Desc H", 42, "A5", 2);
  database_insert_item(database, item_h);
  item_t *item_i = create_item("I", "Desc I", 42, "A6", 2);
  database_insert_item(database, item_i);
  item_t *item_j = create_item("J", "Desc J", 42, "A7", 2);
  database_insert_item(database, item_j);

  CU_ASSERT_TRUE(database_sort(database));

  item_h = database_get_item(database, "H");
  item_i = database_get_item(database, "I");
  item_j = database_get_item(database, "J");

  database_remove_item(database, item_h);
  CU_ASSERT_FALSE(database_sort(database));
  database_remove_item(database, item_i);
  CU_ASSERT_FALSE(database_sort(database));
  database_remove_item(database, item_j);
  CU_ASSERT_TRUE(database_sort(database));

  release(database);
}

int main(int argc, char *argv[]) {

  CU_initialize_registry();

  // Set up suites and tests
  CU_pSuite database = CU_add_suite("Test database", NULL, NULL);
  CU_add_test(database, "New", test_database_new);
  CU_add_test(database, "Insert", test_database_insert_item);
  CU_add_test(database, "Update", test_database_update_item);
  CU_add_test(database, "Remove", test_database_remove_item);
  CU_add_test(database, "Has item", test_database_has_item);
  CU_add_test(database, "Get item", test_database_get_item);
  CU_add_test(database, "Location valid", test_database_location_is_valid);
  CU_add_test(database, "Undo action", test_database_undo_action);
  CU_add_test(database, "Is sorted", test_database_is_sorted);
  CU_add_test(database, "Sort", test_database_sort);

  CU_basic_run_tests();

  // Tear down
  CU_cleanup_registry();

  return CU_get_error();

}
