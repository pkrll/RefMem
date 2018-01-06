
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "macros.h"
#include "database.h"
#include "utils.h"
#include "utils_goods.h"

// -------------------------------
// Structs & Enums
// -------------------------------

// -------------------------------
// Declarations
// -------------------------------

// Handler for list_goods used for edit_goods and show_goods
typedef void(* list_goods_handler)(database_t *database, item_t *item);

static bool list_shelves(char *name, int amount, void *data);
static void set_item_details(enum ItemDetails details, database_t *database, item_t *item);
static void add_goods(database_t *database);
static void remove_goods(database_t *database, item_t *item);
static bool edit_item_details(database_t *database, item_t *item);
static void edit_goods(database_t *database, item_t *item);
static void undo(database_t *database);
static void list_goods(database_t *database, list_goods_handler handler);
static void print_goods(database_iter_t *iter, int limit);
static char *ask_for_goods_index(int index, int size);
static void check_if_database_is_sorted(database_t *database);
static void sort_database(database_t *database);
static void exit_program(database_t *database);
static void event_loop(database_t *database);

// -------------------------------
// Functions
// -------------------------------

static void set_item_details(enum ItemDetails details, database_t *database, item_t *item) {
  if (details & NAME) {
    char *name = Ask_name();
    retain(name);
    item_set_name(item, name);
    release(name);
  }

  if (details & DESC) {
    char *desc = Ask_desc();
    retain(desc);
    item_set_desc(item, desc);
    release(desc);
  }

  if (details & COST) {
    int cost = Ask_cost();
    item_set_cost(item, cost);
  }

  if (details & SHELF) {
    char *shelf = NULL;
    bool cont = true;

    do {
      char *name = item_get_name(item);
      shelf = Ask_shelf();
      retain(shelf);
      if (database_location_is_valid(database, name, shelf)) {
        int amount = Ask_amount();
        item_set_shelves(item, shelf, amount, true);
        cont = false;
      } else {
        Print_shelf_occupied(shelf);
      }

      release(shelf);
    } while (cont);
  }
}

static void add_goods(database_t *database) {
  Print_add_goods_title();

  char *name = Ask_name();
  retain(name);

  if (database_has_item(database, name)) {
    Print_item_exists(name);

    item_t *item = database_get_item(database, name);
    item_t *copy = item_copy(item);

    retain(item);
    retain(copy);

    set_item_details(SHELF, database, copy);

    database_update_item(database, item, copy);

    release(item);
    release(copy);

  } else {
    item_t *item = item_new();
    retain(item);
    item_set_name(item, name);

    set_item_details(DESC | COST | SHELF, database, item);
    do {
      Print_delimiter();
      Print_name(name);
      Print_desc(item_get_desc(item));
      Print_cost(item_get_cost(item));

      item_apply_on_shelves(item, list_shelves, NULL);

      char answer = Ask_to_save();

      if (answer == 'J') {
        if (database_insert_item(database, item)) {
          Print_did_insert(name);
        } else {
          Print_did_not_insert(name);
        }
        break;
      } else if (answer == 'R') {
        edit_item_details(database, item);
      } else if (answer == 'N') {
        break;
      }
    } while (true);

    release(item);
  }

  release(name);

}

static void remove_shelf(database_t *database, item_t *item, char *shelf) {
  if (item && shelf) {
    item_t *copy = item_copy(item);
    retain(copy);
    item_set_shelves(copy, shelf, 0, true);

    if (item_has_shelves(copy)) {
      database_update_item(database, item, copy);
    } else {
      if (database_remove_item(database, item)) {
        Print_did_remove();
      }
    }

    release(copy);
  }
}

static void remove_goods(database_t *database, item_t *item) {
  if (database && item) {
    char **shelves = item_get_shelves(item);
    char *name = item_get_name(item);
    retain(shelves);

    Print_item_exists_on_shelves(name);

    int index = 1;
    item_apply_on_shelves(item, list_shelves, &index);

    do {
      int answer = Ask_for_shelf_to_remove();

      if (answer < 0) {
        break;
      } else if (answer >= index) { // fixade bugg
        Print_wrong_choice();
      } else {
        char *shelf = string_duplicate(shelves[answer]);
        retain(shelf);
        remove_shelf(database, item, shelf);
        release(shelf);
        break;
      }

    } while (true);

    release(shelves);
  }
}

static bool list_shelves(char *name, int amount, void *data) {
  if (data) {
    int *index = data;
    printf("%d. ", *index);
    (*index)++;
  }

  printf("Hylla: %s (%d stycken)\n", name, amount);

  return true;
}

static bool edit_item_details(database_t *database, item_t *item) {
  bool cont = true;

  do {
    char option = Show_edit_menu();

    switch (option) {
    case 'N':
      printf("\nNuvarande namn: %s\n", item_get_name(item));
      set_item_details(NAME, database, item);
      cont = false;
      break;
    case 'B':
      printf("\nNuvarande beskrivning: %s\n", item_get_desc(item));
      set_item_details(DESC, database, item);
      cont = false;
      break;
    case 'P':
      printf("\nNuvarande pris: %d\n", item_get_cost(item));
      set_item_details(COST, database, item);
      cont = false;
      break;
    case 'L':
      printf("\nNuvarande lagerhyllor och antal (Ange lagerhylla för att redigera dess antal eller lägg till en ny hylla):\n");
      item_apply_on_shelves(item, list_shelves, NULL);
      Print_delimiter();
      set_item_details(SHELF, database, item);
      cont = false;
      break;
    case 'A':
      return false;
      break;
    default:
      Print_wrong_choice();
    }

  } while (cont);

  return true;
}

static void edit_goods(database_t *database, item_t *item) {
  if (database && item) {

    char answer = 'R';

    item_t *copy = item_copy(item);
    retain(copy);

    do {
      if (answer == 'R') {
        if (edit_item_details(database, copy) == false) {
          break;
        }
      } else if (answer == 'J') {
        char *name = item_get_name(item);
        if (database_update_item(database, item, copy)) {
          Print_did_update(name);
        } else {
          Print_did_not_update(name);
        }
        break;
      } else if (answer == 'N') {
        break;
      }

      answer = Ask_to_save();

    } while (true);

    release(copy);

  }

}

static void undo(database_t *database) {
  if (database_undo_action(database)) {
    printf("\nSenaste ändringen har ångrats!\n");
  } else {
    printf("\nKunde inte ångra senaste ändringen!\n");
  }
}

static void list_goods_details(database_t *database, item_t *item) {
  char *name  = item_get_name(item);
  char *desc  = item_get_desc(item);
  int cost    = item_get_cost(item);

  Print_name(name);
  Print_desc(desc);
  Print_cost(cost);
  item_apply_on_shelves(item, list_shelves, NULL);
}

static void list_goods(database_t *database, list_goods_handler handler) {
  // TODO: We should really do something about this mess
  database_iter_t *iter = database_iterator(database);
  retain(iter);

  int size = database_iterator_size(iter);
  int count = 0;
  int limit = MAX_LIST_COUNT;
  bool cont = true;
  bool should_print_goods = true;

  if (database_iterator_has_next(iter) == false) {
    printf("\nDatabasen är tom!\n");
    release(iter);
    return;
  }

  do {
    // Horrible solution, but I don't really have the time
    if (should_print_goods) {
      print_goods(iter, limit);
    }

    int number_of_listed_goods = count * limit + 1;
    char *choice = ask_for_goods_index(number_of_listed_goods, size);
    retain(choice);

    int cur_indx = atoi(choice) + count - 1;

    if (is_number(choice) && cur_indx < size && cur_indx >= 0) {
      item_t *item = database_iterator_get(iter, cur_indx);

      handler(database, item);
      cont = false;
    } else {
      char c = toupperchar(choice);

      if (c == 'A') {
        cont = false;
      } else if (c == 'F' && number_of_listed_goods < size) {
        should_print_goods = true;
        count += limit;
        release(choice);
        continue;
      } else {
        Print_wrong_choice();
        should_print_goods = false;
      }
    }

    release(choice);

  } while(cont);

  release(iter);
}

static void print_goods(database_iter_t *iter, int limit) {
  for (int i = 1; i <= limit && database_iterator_has_next(iter); i++) {
    item_t *item = database_iterator_next(iter);
    printf("%d. %s\n", i, item_get_name(item));
  }
}

static char *ask_for_goods_index(int index, int size) {
  char *question;
  if (index < size) {
    question = "\nAnge varans index för mer information, visa [f]ler varor eller [a]vbryt: ";
  } else {
    question = "\nAnge varans index för mer information, eller [a]vbryt: ";
  }

  return ask_question_string(question);
}

static void check_if_database_is_sorted(database_t *database) {
  if (database_is_sorted(database)) {
    printf("\nDatabasen är sorterad!\n");
  } else {
    printf("\nDatabasen är inte sorterad!\n");
  }
}

static void sort_database(database_t *database) {
  if (database_sort(database)) {
    printf("\nDatabasen har sorterats!\n");
  } else {
    printf("\nDatabasen är redan sorterad!\n");
  }
}

static void exit_program(database_t *database) {
  release(database);
  shutdown();
}

static void event_loop(database_t *database) {
  if (database == NULL) {
    printf("\nEtt fel har uppstått. Databasen existerar inte.\n");
    return;
  }

  bool cont = true;

  do {
    char choice = Show_main_menu();

    switch (choice) {
    case 'L' :
      add_goods(database);
      break;
    case 'T' :
      list_goods(database, remove_goods);
      break;
    case 'R':
      list_goods(database, edit_goods);
      break;
    case 'G':
      undo(database);
      break;
    case 'H':
      list_goods(database, list_goods_details);
      break;
    case 'K':
      check_if_database_is_sorted(database);
      break;
    case 'S':
      sort_database(database);
      break;
    case 'A':
      exit_program(database);
      cont = false;
      break;
    default:
      Print_wrong_choice();
      break;
    }

  } while (cont);

}

int main(int argc, char *argv[]) {
  char *file = NULL;

  if (argc > 1) {
    file = argv[1];
  }

  database_t *database = database_new(file);
  retain(database);
  event_loop(database);

  return 0;
}
