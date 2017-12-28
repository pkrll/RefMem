#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Automated.h>
#include <stdlib.h>
#include <stdio.h>
#include "../stack.h"

// -------------------------------
// Higher order functions
// -------------------------------

void free_fun(void *element) {
  CU_PASS("Free");
}

// -------------------------------
// The tests
// -------------------------------


void test_stack_new() {
  stack_s *stack = stack_new(NULL);
  CU_ASSERT_EQUAL(stack_size(stack), 0);

  stack_delete(stack);
}

void test_stack_size() {
  stack_s *stack = stack_new(NULL);
  CU_ASSERT_EQUAL(stack_size(stack), 0);

  char *elem1 = "Foo";  
  char *elem2 = "Bar";
  char *elem3 = "Baz";

  stack_push(stack, elem1);
  CU_ASSERT_EQUAL(stack_size(stack), 1);

  stack_push(stack, elem2);
  CU_ASSERT_EQUAL(stack_size(stack), 2);

  stack_push(stack, elem3);
  CU_ASSERT_EQUAL(stack_size(stack), 3);

  stack_pop(stack);
  CU_ASSERT_EQUAL(stack_size(stack), 2);

  stack_pop(stack);
  stack_pop(stack);
  CU_ASSERT_EQUAL(stack_size(stack), 0);

  stack_push(stack, elem3);
  CU_ASSERT_EQUAL(stack_size(stack), 1);
  
  stack_delete(stack);
}

void test_stack_push() {
  stack_s *stack = stack_new(NULL);

  char *elem1 = "Foo";  
  char *elem2 = "Bar";
  char *elem3 = "Baz";
  char *elem4 = "Haz";
  
  stack_push(stack, elem1);
  CU_ASSERT_STRING_EQUAL(stack_top(stack), elem1);
  stack_push(stack, elem2);
  CU_ASSERT_STRING_EQUAL(stack_top(stack), elem2); 
  stack_push(stack, elem3);
  CU_ASSERT_STRING_EQUAL(stack_top(stack), elem3); 

  stack_pop(stack);
  CU_ASSERT_STRING_EQUAL(stack_top(stack), elem2);

  stack_pop(stack);
  stack_pop(stack);
  CU_ASSERT_PTR_NULL(stack_top(stack));

  stack_push(stack, elem4);
  CU_ASSERT_STRING_EQUAL(stack_top(stack), elem4);
  
  stack_delete(stack);
}

void test_stack_pop() {
  stack_s *stack = stack_new(NULL);
  CU_ASSERT_PTR_NULL(stack_pop(stack));
  
  char *elem1 = "Foo";  
  char *elem2 = "Bar";
  char *elem3 = "Baz";

  stack_push(stack, elem1);
  stack_push(stack, elem2);

  CU_ASSERT_STRING_EQUAL(stack_pop(stack), elem2);
  CU_ASSERT_EQUAL(stack_size(stack), 1);
  CU_ASSERT_STRING_EQUAL(stack_pop(stack), elem1);
  CU_ASSERT_EQUAL(stack_size(stack), 0);
  
  stack_push(stack, elem3);
  CU_ASSERT_STRING_EQUAL(stack_pop(stack), elem3);
  
  stack_delete(stack);
}

void test_stack_top() {
  stack_s *stack = stack_new(NULL);
  CU_ASSERT_PTR_NULL(stack_top(stack));
  
  char *elem1 = "Foo";  
  char *elem2 = "Bar";
  char *elem3 = "Baz";

  stack_push(stack, elem1);
  CU_ASSERT_STRING_EQUAL(stack_top(stack), elem1);
  CU_ASSERT_EQUAL(stack_size(stack), 1);

  stack_push(stack, elem2);
  stack_push(stack, elem3);
  CU_ASSERT_STRING_EQUAL(stack_top(stack), elem3);
  CU_ASSERT_EQUAL(stack_size(stack), 3);

  stack_pop(stack);
  CU_ASSERT_STRING_EQUAL(stack_top(stack), elem2);
  CU_ASSERT_EQUAL(stack_size(stack), 2);

  stack_delete(stack);
}

void test_stack_delete() {
  stack_s *stack = stack_new(free_fun);

  char *elem1 = "Foo";

  stack_push(stack, elem1);

  stack_delete(stack);
}

int main(int argc, char *argv[]) {

  CU_initialize_registry();

  CU_pSuite stack = CU_add_suite("Testing stack", NULL, NULL);
  CU_add_test(stack, "Creation", test_stack_new);
  CU_add_test(stack, "Size", test_stack_size);
  CU_add_test(stack, "Push", test_stack_push);
  CU_add_test(stack, "Pop", test_stack_pop);
  CU_add_test(stack, "Top", test_stack_top);
  CU_add_test(stack, "Delete", test_stack_delete);

  CU_basic_run_tests();

  CU_cleanup_registry();
  
  return CU_get_error();
}
