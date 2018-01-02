#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Automated.h>
#include <stdlib.h>
#include <stdio.h>
#include "../action.h"
#include "../../src/refmem.h"

void test_action_new() {
  action_t *action = action_new(REMOVE);

  CU_ASSERT_PTR_NOT_NULL(action);
  CU_ASSERT_EQUAL(action_get_type(action), REMOVE);

  deallocate(action);
}

void test_action_new_add() {
  int foo = 5;

  action_t *action = action_new_add(&foo);
  retain(action);
  CU_ASSERT_EQUAL(action_get_type(action), ADD);

  int *result = action_get_saved(action);
  CU_ASSERT_EQUAL(*result, foo);

  release(action);
}

void test_action_new_edit() {
  int orig_elem = 15;
  int edit_elem = 16;

  action_t *action = action_new_edit(&orig_elem, &edit_elem);
  retain(action);

  CU_ASSERT_EQUAL(action_get_type(action), EDIT);

  int *orig_result = action_get_original(action);
  int *edit_result = action_get_edited(action);
  CU_ASSERT_EQUAL(*orig_result, orig_elem);
  CU_ASSERT_EQUAL(*edit_result, edit_elem);

  release(action);
}

void test_action_new_remove() {
  int elem = 0;

  action_t *action = action_new_remove(&elem);
  retain(action);
  CU_ASSERT_EQUAL(action_get_type(action), REMOVE);

  int *result = action_get_saved(action);
  CU_ASSERT_EQUAL(*result, elem);

  release(action);
}

void test_action_get_type() {
  action_t *action = action_new(NOTHING);
  CU_ASSERT_EQUAL(action_get_type(action), NOTHING);
  CU_ASSERT_NOT_EQUAL(action_get_type(action), ADD);
  deallocate(action);

  int elem = 5;
  action = action_new_add(&elem);
  CU_ASSERT_EQUAL(action_get_type(action), ADD);
  CU_ASSERT_NOT_EQUAL(action_get_type(action), EDIT);
  deallocate(action);

  action = action_new_edit(&elem, &elem);
  CU_ASSERT_EQUAL(action_get_type(action), EDIT);
  CU_ASSERT_NOT_EQUAL(action_get_type(action), NOTHING);
  deallocate(action);

  action = action_new_remove(&elem);
  CU_ASSERT_EQUAL(action_get_type(action), REMOVE);
  CU_ASSERT_NOT_EQUAL(action_get_type(action), ADD);
  deallocate(action);
}

void test_action_get_saved() {
  int elem = 5;
  action_t *action = action_new_add(&elem);
  int *result = action_get_saved(action);
  CU_ASSERT_EQUAL(*result, elem);
  deallocate(action);

  elem = 15;
  action = action_new_remove(&elem);
  result = action_get_saved(action);
  CU_ASSERT_EQUAL(*result, elem);
  deallocate(action);

  int orig_elem = 11;
  int edit_elem = 99;

  action = action_new_edit(&orig_elem, &edit_elem);
  result = action_get_saved(action);
  CU_ASSERT_NOT_EQUAL(*result, orig_elem);
  deallocate(action);
}

void test_action_get_edited() {
  int orig_elem = 11;
  int edit_elem = 99;

  action_t *action = action_new_edit(&orig_elem, &edit_elem);
  int *result      = action_get_edited(action);

  CU_ASSERT_EQUAL(*result, edit_elem);
  CU_ASSERT_NOT_EQUAL(*result, orig_elem);

  deallocate(action);
}

void test_action_get_original() {
  int orig_elem = 11;
  int edit_elem = 99;

  action_t *action = action_new_edit(&orig_elem, &edit_elem);
  int *result      = action_get_original(action);

  CU_ASSERT_EQUAL(*result, orig_elem);
  CU_ASSERT_NOT_EQUAL(*result, edit_elem);

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

  return CU_get_error();

}
