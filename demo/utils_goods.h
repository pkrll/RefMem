/**
 * @file
 * @brief Specialized utils library
 */

#ifndef __utils_goods_h__
#define __utils_goods_h__

#include <stdbool.h>

typedef enum {
  Main_Menu,
  Edit_Menu,
  Save_Menu
} MenuType;

/**
 * @brief Strips a newline character from the end of a string.
 *
 * @param  str A string.
 *
 * @return The string without a newline character at the end.
 */
char *strip_newline(char *str);
/**
 * @brief Checks if the inputed string is a valid menu choice. 
 *
 * @param  str The string to check.
 *
 * @return True if the string is valid, otherwise false.
 */
bool is_menu(char *str, const char *choices);
/**
 * @brief Checks if a string is a valid shelf number.
 *
 * @param  str The string to check.
 *
 * @return True if the string is valid, otherwise false.
 */
bool is_valid_shelf(char *str);
/**
 * @brief Asks the user for a shelf number.
 *
 * The user will be presented with a question, and the
 * system will wait for an input.
 *
 * The shelf number has to be begin with a single char,
 * and may end with double digits. If the input does not
 * meet the above requirements, the user will be presented
 * with the question again until they give a valid input.
 *
 * @param  question The question to display to the user.
 *
 * @return The shelf number the user entered.
 */
char *ask_question_shelf(const char *question);
/**
 * @brief Asks the user for a menu choice.
 *
 * The choices are: L, T, R, G, H or A. If the input is 
 * anything other than those, the question will be asked 
 * again until a valid input.
 *
 * @param  question The question to display to the user.
 *
 * @return The (valid) character that the user entered.
 */
char ask_question_menu(const char *question, MenuType menu);

#endif
