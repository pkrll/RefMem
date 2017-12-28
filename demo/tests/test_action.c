#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Automated.h>
#include <stdlib.h>
#include <stdio.h>
#include "../action.h"

void test_action_new() {
  action_t *action = action_new(REMOVE);
  
  CU_ASSERT_PTR_NOT_NULL(action);
  CU_ASSERT_EQUAL(action_get_type(action), REMOVE);
  
  action_delete(action);
}

void test_action_new_add() {
  elem_t elem = (elem_t) { .i = 5 };
  
  action_t *action = action_new_add(elem);
  CU_ASSERT_EQUAL(action_get_type(action), ADD);

  elem_t result = action_get_saved(action);
  CU_ASSERT_EQUAL(result.i, elem.i);

  action_delete(action);
}

void test_action_new_edit() {
  elem_t orig_elem = (elem_t) { .i = 15 };
  elem_t edit_elem = (elem_t) { .i = 16 };
  
  action_t *action = action_new_edit(orig_elem, edit_elem);
  CU_ASSERT_EQUAL(action_get_type(action), EDIT);

  elem_t orig_result = action_get_original(action);
  elem_t edit_result = action_get_edited(action);
  CU_ASSERT_EQUAL(orig_result.i, orig_result.i);
  CU_ASSERT_EQUAL(edit_result.i, edit_result.i);
  
  action_delete(action);
}

void test_action_new_remove() {
  elem_t elem = (elem_t) { .i = 0 };
  
  action_t *action = action_new_remove(elem);
  CU_ASSERT_EQUAL(action_get_type(action), REMOVE);

  elem_t result = action_get_saved(action);
  CU_ASSERT_EQUAL(result.i, elem.i);

  action_delete(action);
}

void test_action_get_type() {  
  action_t *action = action_new(NOTHING);
  CU_ASSERT_EQUAL(action_get_type(action), NOTHING);
  CU_ASSERT_NOT_EQUAL(action_get_type(action), ADD);
  action_delete(action);

  elem_t elem = { .i = 5 };
  action = action_new_add(elem);
  CU_ASSERT_EQUAL(action_get_type(action), ADD);
  CU_ASSERT_NOT_EQUAL(action_get_type(action), EDIT);
  action_delete(action);

  action = action_new_edit(elem, elem);
  CU_ASSERT_EQUAL(action_get_type(action), EDIT);
  CU_ASSERT_NOT_EQUAL(action_get_type(action), NOTHING);
  action_delete(action);

  action = action_new_remove(elem);
  CU_ASSERT_EQUAL(action_get_type(action), REMOVE);
  CU_ASSERT_NOT_EQUAL(action_get_type(action), ADD);
  action_delete(action);
}

void test_action_get_saved() {
  elem_t elem = (elem_t) { .i = 5 };
  action_t *action = action_new_add(elem);
  elem_t result = action_get_saved(action);
  CU_ASSERT_EQUAL(result.i, elem.i);
  action_delete(action);

  elem = (elem_t) { .i = 15 };
  action = action_new_remove(elem);
  result = action_get_saved(action);
  CU_ASSERT_EQUAL(result.i, elem.i);
  action_delete(action);

  elem_t orig_elem = (elem_t) { .i = 11 };
  elem_t edit_elem = (elem_t) { .i = 99 };
  action = action_new_edit(orig_elem, edit_elem);
  result = action_get_saved(action);
  CU_ASSERT_NOT_EQUAL(result.i, orig_elem.i);
  action_delete(action);
}

void test_action_get_edited() {
  elem_t orig_elem = (elem_t) { .i = 11 };
  elem_t edit_elem = (elem_t) { .i = 99 };
  action_t *action = action_new_edit(orig_elem, edit_elem);
  elem_t result    = action_get_edited(action);
  CU_ASSERT_EQUAL(result.i, edit_elem.i);
  CU_ASSERT_NOT_EQUAL(result.i, orig_elem.i);
  action_delete(action);
}

void test_action_get_original() {
  elem_t orig_elem = (elem_t) { .i = 11 };
  elem_t edit_elem = (elem_t) { .i = 99 };
  action_t *action = action_new_edit(orig_elem, edit_elem);
  elem_t result    = action_get_original(action);
  CU_ASSERT_EQUAL(result.i, orig_elem.i);
  CU_ASSERT_NOT_EQUAL(result.i, edit_elem.i);
  action_delete(action);
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
  
  return CU_get_error();

}
