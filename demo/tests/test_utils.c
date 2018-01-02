#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Automated.h>
#include <stdlib.h>
#include "../utils.h"

void test_is_digit() {
  char c = 'A';
  CU_ASSERT_FALSE(is_digit(c));

  c = '5';
  CU_ASSERT_TRUE(is_digit(c));
}

void test_is_number() {
  char *str = "1234";
  CU_ASSERT_TRUE(is_number(str));

  str = "123a4";
  CU_ASSERT_FALSE(is_number(str));

  str = "1234a";
  CU_ASSERT_FALSE(is_number(str));
  
  str = "ABCDEF";
  CU_ASSERT_FALSE(is_number(str));

  str = "";
  CU_ASSERT_FALSE(is_number(str));
}

void test_is_char() {
  char *str = "A";
  CU_ASSERT_TRUE(is_char(str));

  str = "AB";
  CU_ASSERT_FALSE(is_char(str));

  str = "2";
  CU_ASSERT_FALSE(is_char(str));
}

void test_not_empty() {
  char *str = "ABCDEF";
  CU_ASSERT_TRUE(not_empty(str));

  str = "";
  CU_ASSERT_FALSE(not_empty(str));
}

void test_toupperchar() {
  char *str = "cunit";
  CU_ASSERT_EQUAL(toupperchar(str), 'C');

  str = "123";
  CU_ASSERT_EQUAL(toupperchar(str), '1');

  str = "Abcdef";
  CU_ASSERT_EQUAL(toupperchar(str), 'A');
}

int main(int argc, char *argv[]) {
  
  CU_initialize_registry();

  // Set up suites and tests
  CU_pSuite utils = CU_add_suite("Test utils", NULL, NULL);
  CU_add_test(utils, "is digit", test_is_digit);
  CU_add_test(utils, "is number", test_is_number);
  CU_add_test(utils, "is char", test_is_char);
  CU_add_test(utils, "not empty", test_not_empty);
  CU_add_test(utils, "to upper char", test_toupperchar);
  
  CU_basic_run_tests();
  
  // Tear down
  CU_cleanup_registry();
  
  return CU_get_error();

}
