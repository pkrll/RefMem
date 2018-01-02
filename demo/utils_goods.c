#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "utils_goods.h"
#include "utils.h"
#include "macros.h"

// -------------------------------
// Declarations
// -------------------------------

static bool is_main_menu(char *str);
static bool is_edit_menu(char *str);
static bool is_save_menu(char *str);

// -------------------------------
// Public
// -------------------------------

char *strip_newline(char *str) {
  if (str) {
    int len = strlen(str);

    if (str[len - 1] == '\n') {
      str[len - 1] = '\0';
    }
  }

  return str;
}

bool is_menu(char *str, const char *choices) {
  while (*choices != '\0' && strlen(str) == 1) {
    if (toupper(*str) == *choices) {
      return true;
    }

    choices++;
  }

  return false;
}

bool is_valid_shelf(char *str) {
  if (isalpha(*str++)) {
    return is_number(str);
  }

  return false;
}

char *ask_question_shelf(const char *question) {
  char * answer = ask_question(question, (check_func) is_valid_shelf, (convert_func) string_duplicate).s;

  answer[0] = toupper(answer[0]);

  return answer;
}

char ask_question_menu(const char *question, MenuType menu) {
  check_func func = is_main_menu;

  if (menu == Edit_Menu) {
    func = is_edit_menu;
  } else if (menu == Save_Menu) {
    func = is_save_menu;
  }

  return ask_question(question, func, (convert_func) toupperchar).c;
}

// -------------------------------
// Private
// -------------------------------

static bool is_main_menu(char *str) {
  return is_menu(str, "LTRGHAKS");
}

static bool is_edit_menu(char *str) {
  return is_menu(str, "NBPLTA");
}

static bool is_save_menu(char *str) {
  return is_menu(str, "JRN");
}
