/**
 * @file
 * @brief Utils library
 */

#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdbool.h>

/**
 * @union answer_t
 */
typedef union {
  int i;
  float f;
  char *s;
  char c;
} answer_t;

typedef bool(*check_func)(char *);

typedef answer_t(*convert_func)(char *);


char *string_duplicate(char *str);

/**
 * @brief Checks if a character is a number.
 *
 * A number is anything between 0 and 9.
 *
 * @param  c The character to check.
 *
 * @return True if the character is a digit, otherwise false.
 */
bool is_digit(char str);
/**
 * @brief Checks if a string is made up of numbers.
 *
 * @param  str The string to check.
 *
 * @return True if the string only contains numbers, otherwise false.
 */
bool is_number(char *str);
/**
 * @brief Checks if a string consist of only one letter.
 *
 * @param  str The string to check.
 *
 * @return True if the string only contains a letter, otherwise false.
 */
bool is_char(char *str);
/**
 * @brief Checks if a string is empty.
 *
 * @param  str The string to check.
 *
 * @return True if the string is empty, otherwise False.
 */
bool not_empty(char *str);
/**
 * @brief Returns the first character in a string in upper case.
 *
 * @param  str The string.
 *
 * @return A character in upper case.
 */
char toupperchar(char *str);
/**
 * @brief Asks the user for an integer.
 *
 * If the input is not valid, the question will be asked again.
 *
 * @param  question The question to display to the user.
 *
 * @return The integer that the user entered.
 */
int ask_question_int(const char *question);
/**
 * @brief Asks the user for a single character.
 *
 * If the input is not valid, the question will be asked again.
 *
 * @param  question The question to display to the user.
 *
 * @return The character that the user entered.
 */
char ask_question_char(const char *question);
/**
 * @brief Asks the user for a non-empty string.
 *
 * If the input is not valid, the question will be asked again.
 *
 * @param  question The question to display to the user.
 *
 * @return The string that the user entered.
 */
char *ask_question_string(const char *question);
/**
 * @brief Asks the user a question.
 *
 * Depending on what the convert function does, the return value
 * will differ. The union answer_t returned by this function
 * has multiple fields:
 *
 *    - i for int
 *    - f for float
 *    - c for char
 *    - s for string
 *
 * To prevent getting a nonsensical value, use the correct field.
 *
 * @param  question The question to display to the user.
 * @param  check    A check function that should make sure that
 *                  the input is valid.
 * @param  convert  A convert function that should convert the
 *                  input to a valid output to use.
 *
 * @return Either an integer, a string, a character or a float.
 */
answer_t ask_question(const char *question, check_func check, convert_func convert);

#endif
