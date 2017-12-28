#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include "../src/refmem.h"

// -------------------------------
// Declarations
// -------------------------------

static int read_string(char *buf, int buf_siz);
static void clear_input_buffer();

// -------------------------------
// Public
// -------------------------------

char *string_duplicate(char *str) {
  int length = strlen(str);
  char *duplicate = allocate(length + 1, NULL);

  strcpy(duplicate, str);

  return duplicate;
}

bool is_digit(char str) {
  return ('0' <= str && str <= '9');
}

bool is_number(char *str) {
  int len = strlen(str);

  for (int i = 0; i < len; i++) {
    if (is_digit(str[i]) == false) {
      return false;
    }
  }

  return (strlen(str) > 0) ? true : false;
}

bool is_char(char *str) {
  return (isalpha(*str) && strlen(str) == 1);
}

bool not_empty(char *str) {
  return strlen(str) > 0;
}

char toupperchar(char *str) {
  return toupper(*str);
}

int ask_question_int(const char *question) {
  return ask_question(question, is_number, (convert_func) atoi).i;
}

char ask_question_char(const char *question) {
  return ask_question(question, (check_func) is_char, (convert_func) toupperchar).c;
}

char *ask_question_string(const char *question) {
  return ask_question(question, not_empty, (convert_func) string_duplicate).s;
}

answer_t ask_question(const char *question, check_func check, convert_func convert) {
  int buf_size = 255;
  char buf[buf_size];
  bool loop = true;

  while (loop) {
    printf("%s", question);
    read_string(buf, buf_size);

    if (check(buf)) {
      loop = false;
    } else {
      printf("\nFelaktig inmatning: '%s'!\n\n", buf);
    }
  }

  return convert(buf);
}

// -------------------------------
// Private
// -------------------------------

static int read_string(char *buf, int buf_siz) {
  int i;
  bool clear = true;

  for (i = 0; i < buf_siz - 1; i++) {
    int tmp = getchar();

    if (tmp == '\n' || tmp == EOF) {
      clear = false;
      break;
    } else {
      buf[i] = tmp;
    }
  }

  buf[i] = '\0';

  if (clear) clear_input_buffer();

  return i;
}

static void clear_input_buffer() {
  int c;
  do {
    c = getchar();
  } while (c != '\n' && c != EOF);
}
