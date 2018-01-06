#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Automated.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../demo/utils.h"
#include "../../demo/action.h"
#include "../../src/refmem.h"

int *new_integer(int number) {
  int *integer = allocate(sizeof(int), NULL);
  *integer = number;
  return integer;
}

void test_action_new() {
  action_t *action = action_new(REMOVE);

  CU_ASSERT_PTR_NOT_NULL(action);
  CU_ASSERT_EQUAL(action_get_type(action), REMOVE);

  deallocate(action);
}

void test_action_new_add() {
  int *foo = new_integer(5);

  action_t *action = action_new_add(foo);
  retain(action);
  CU_ASSERT_EQUAL(action_get_type(action), ADD);

  int *result = action_get_saved(action);
  CU_ASSERT_EQUAL(*result, *foo);

  release(action);
}

void test_action_new_edit() {
  int *orig_elem = new_integer(15);
  int *edit_elem = new_integer(16);

  action_t *action = action_new_edit(orig_elem, edit_elem);
  retain(action);

  CU_ASSERT_EQUAL(action_get_type(action), EDIT);

  int *orig_result = action_get_original(action);
  int *edit_result = action_get_edited(action);
  CU_ASSERT_EQUAL(*orig_result, *orig_elem);
  CU_ASSERT_EQUAL(*edit_result, *edit_elem);

  release(action);
}

void test_action_new_remove() {
  int *elem = new_integer(0);

  action_t *action = action_new_remove(elem);
  retain(action);
  CU_ASSERT_EQUAL(action_get_type(action), REMOVE);

  int *result = action_get_saved(action);
  CU_ASSERT_EQUAL(*result, *elem);

  release(action);
}

void test_action_get_type() {
  action_t *action = action_new(NOTHING);
  CU_ASSERT_EQUAL(action_get_type(action), NOTHING);
  CU_ASSERT_NOT_EQUAL(action_get_type(action), ADD);
  deallocate(action);

  char *string = string_duplicate("Hello World");
  retain(string); // Must retain this, as it will be released by action

  action = action_new_add(string);
  CU_ASSERT_EQUAL(action_get_type(action), ADD);
  CU_ASSERT_NOT_EQUAL(action_get_type(action), EDIT);
  deallocate(action);

  action = action_new_edit(string, string);
  CU_ASSERT_EQUAL(action_get_type(action), EDIT);
  CU_ASSERT_NOT_EQUAL(action_get_type(action), NOTHING);
  deallocate(action);

  action = action_new_remove(string);
  CU_ASSERT_EQUAL(action_get_type(action), REMOVE);
  CU_ASSERT_NOT_EQUAL(action_get_type(action), ADD);
  deallocate(action);

  release(string); // Fixes a bug
}

void test_action_get_saved() {
  char *string = string_duplicate("Hello World");
  retain(string); // Must retain this, as it will be released by action

  action_t *action = action_new_add(string);
  char *result = action_get_saved(action);
  CU_ASSERT_STRING_EQUAL(result, string);
  deallocate(action);

  release(string);

  string = string_duplicate("Foo bar!");
  retain(string); // Must retain this, as it will be released by action

  action = action_new_remove(string);
  result = action_get_saved(action);
  CU_ASSERT_STRING_EQUAL(result, string);
  deallocate(action);

  release(string);

  char *orig_string = string_duplicate("Original string!");
  char *edit_string = string_duplicate("Edited string!");

  action = action_new_edit(orig_string, edit_string);
  result = action_get_saved(action);
  CU_ASSERT_STRING_NOT_EQUAL(result, orig_string);

  deallocate(action);
}

void test_action_get_edited() {
  char *orig_string = string_duplicate("Original string!");
  char *edit_string = string_duplicate("Edited string!");

  action_t *action = action_new_edit(orig_string, edit_string);
  char *result = action_get_edited(action);

  CU_ASSERT_STRING_EQUAL(result, edit_string);
  CU_ASSERT_STRING_NOT_EQUAL(result, orig_string);

  deallocate(action);
}

void test_action_get_original() {
  char *orig_string = string_duplicate("Original string!");
  char *edit_string = string_duplicate("Edited string!");

  action_t *action = action_new_edit(orig_string, edit_string);
  char *result      = action_get_original(action);

  CU_ASSERT_STRING_EQUAL(result, orig_string);
  CU_ASSERT_STRING_NOT_EQUAL(result, edit_string);

  deallocate(action);
}

int main(int argc, char *argv[]) {
  CU_initialize_registry();

  CU_pSuite action = CU_add_suite("Testing action", NULL, NULL);
  CU_add_test(action, "New", test_action_new);
  CU_add_test(action, "New Add", test_action_new_add);
  CU_add_test(action, "New Edit", test_action_new_edit);
  CU_add_test(action, "New Remove", test_action_new_remove);
  CU_add_test(action, "Get Type", test_action_get_type);
  CU_add_test(action, "Get Saved", test_action_get_saved);
  CU_add_test(action, "Get Edited", test_action_get_edited);
  CU_add_test(action, "Get Original", test_action_get_original);

  CU_basic_run_tests();

  CU_cleanup_registry();

  shutdown();

  return CU_get_error();

}
