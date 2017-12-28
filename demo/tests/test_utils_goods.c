#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Automated.h>
#include <stdlib.h>
#include "../utils_goods.h"
#include "../utils.h"
#include <string.h>

void test_is_menu() {
  char *choices = "123ABC";
  CU_ASSERT_TRUE(is_menu("1", choices));
  CU_ASSERT_TRUE(is_menu("2", choices));
  CU_ASSERT_TRUE(is_menu("3", choices));
  CU_ASSERT_TRUE(is_menu("A", choices));
  CU_ASSERT_TRUE(is_menu("B", choices));
  CU_ASSERT_TRUE(is_menu("C", choices));
  CU_ASSERT_FALSE(is_menu("9", choices));
  CU_ASSERT_FALSE(is_menu("123", choices));
}

void test_is_valid_shelf() {
  CU_ASSERT_TRUE(is_valid_shelf("A2"));
  CU_ASSERT_TRUE(is_valid_shelf("a345"));
  CU_ASSERT_FALSE(is_valid_shelf("23A2"));
  CU_ASSERT_FALSE(is_valid_shelf("ABC123"));
  CU_ASSERT_FALSE(is_valid_shelf(""));
  CU_ASSERT_FALSE(is_valid_shelf("23"));
}

int main(int argc, char *argv[]) {
  
  CU_initialize_registry();

  // Set up suites and tests
  CU_pSuite utils = CU_add_suite("Test utils_goods", NULL, NULL);
  CU_add_test(utils, "is menu", test_is_menu);
  CU_add_test(utils, "is valid shelf", test_is_valid_shelf);
  
  CU_basic_run_tests();
  
  // Tear down
  CU_cleanup_registry();
  
  return CU_get_error();

}
