#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "item.h"
#include "utils.h"

typedef struct shelf shelf_t;

// -------------------------------
// Structs & Enums
// -------------------------------

typedef struct shelf {
  char *name;
  int amount;
} shelf_t;

struct item {
  char *name;
  char *desc;
  int cost;

  list_t *shelves;
};

// -------------------------------
// Declarations
// -------------------------------

static shelf_t *shelf_new(char *name, int amount);
// static void shelf_delete(obj elem);
static shelf_t *get_shelf(list_t *shelves, char *shelf_name, int *shelf_index);
static bool count_shelves_amount(obj elem, void *data);

static void item_destructor(obj object);
static void shelf_destructor(obj object);

// -------------------------------
// Public
// -------------------------------

item_t *item_new() {
  item_t *item = allocate(sizeof(item_t), item_destructor);

  if (item) {
    item->shelves = list_new(NULL, NULL, NULL);
    retain(item->shelves);
  }

  return item;
}

item_t *item_copy(item_t *item) {
  item_t *copy = item_new();

  if (item) {
    copy->name = string_duplicate(item->name);
    copy->desc = string_duplicate(item->desc);
    copy->cost = item->cost;

    retain(copy->name);
    retain(copy->desc);

    list_t *shelves_orig = item->shelves;

    for (int i = 0; i < list_length(shelves_orig); i++) {
      obj elem;

      if (list_get(shelves_orig, i, &elem)) {
        shelf_t *shelf_orig = elem;

        char *name = shelf_orig->name;
        int amount = shelf_orig->amount;

        shelf_t *shelf_copy = shelf_new(name, amount);

        list_prepend(copy->shelves, shelf_copy);
      }
    }
  }

  return copy;
}

int item_total(item_t *item) {
  int total = 0;

  if (item) {
    list_apply(item->shelves, count_shelves_amount, &total);
  }

  return total;
}

bool item_set_name(item_t *item, char *name) {
  if (item && strlen(name) > 0) {
    if (item->name) release(item->name);

    item->name = string_duplicate(name);
    retain(item->name);

    return true;
  }

  return false;
}

char *item_get_name(item_t *item) {
  if (item) {
    return item->name;
  }

  return NULL;
}

bool item_set_desc(item_t *item, char *desc) {
  if (strlen(desc) > 0) {
    if (item->desc) release(item->desc);

    item->desc = string_duplicate(desc);
    retain(item->desc);

    return true;
  }

  return false;
}

char *item_get_desc(item_t *item) {
  if (item) {
    return item->desc;
  }

  return NULL;
}

bool item_set_cost(item_t *item, int cost) {
  if (item && cost >= 0) {
    item->cost = cost;

    return true;
  }

  return false;
}

int item_get_cost(item_t *item) {
  if (item && item) {
    return item->cost;
  }

  return 0;
}

void item_set_shelves(item_t *item, char *shelf_name, int amount, bool overwrite) {
  if (item && shelf_name && strlen(shelf_name) > 0) {
    int index = -1;
    list_t *shelves = item->shelves;
    shelf_t *shelf  = get_shelf(shelves, shelf_name, &index);

    if (shelf == NULL) {
      if (amount > 0) {
        shelf = shelf_new(shelf_name, amount);
        list_prepend(shelves, shelf);
      }
    } else {
      if (amount > 0) {
        shelf->amount = (overwrite) ? amount : shelf->amount + amount;
      } else {
        list_remove(shelves, index, true);
      }

    }
  }
}

int item_total_amount(item_t *item) {
  int amount = 0;

  if (item) {
    list_t *shelves = item->shelves;
    int size = list_length(shelves);


    for (int i = 0; i < size; i++) {
      obj elem;
      if (list_get(shelves, i, &elem)) {
        shelf_t *shelf = elem;
        amount = amount + shelf->amount;
      }
    }
  }

  return amount;
}

bool item_has_shelves(item_t *item) {
  if (item) {
    int size = list_length(item->shelves);

    return size > 0;
  }

  return false;
}

bool item_has_shelf(item_t *item, char *shelf) {
  if (item) {
    list_t *shelves = item->shelves;
    if (get_shelf(shelves, shelf, NULL) != NULL) {
      return true;
    }
  }

  return false;
}

bool item_apply_on_shelves(item_t *item, item_shelf_apply_fun fun, void *data) {
  bool result = false;

  if (item) {
    list_t *shelves = item->shelves;
    int size = list_length(shelves);

    for (int index = 0; index < size; index++) {
      obj elem;

      if (list_get(shelves, index, &elem)) {
        shelf_t *shelf = elem;
        char *name = shelf->name;
        int amount = shelf->amount;

        if (shelf && fun(name, amount, data)) {
          result = true;
        }
      }
    }
  }

  return result;
}

int item_number_of_shelves(item_t *item) {
  if (item) {
    return list_length(item->shelves);
  }

  return 0;
}

char **item_get_shelves(item_t *item) {
  if (item) {
    int size = list_length(item->shelves);
    char **shelves = allocate_array(size, sizeof(char *), NULL);

    for (int i = 0; i < size; i++) {
      obj elem;

      if (list_get(item->shelves, i, &elem)) {
        shelf_t *shelf = elem;
        shelves[i] = shelf->name;
      }
    }

    return shelves;
  }

  return NULL;
}

// -------------------------------
// Private
// -------------------------------

static shelf_t *shelf_new(char *name, int amount) {
  shelf_t *shelf = allocate(sizeof(shelf_t), shelf_destructor);

  if (shelf) {
    shelf->name = string_duplicate(name);
    shelf->amount = amount;
    retain(shelf->name);
  }

  return shelf;
}

static shelf_t *get_shelf(list_t *shelves, char *shelf_name, int *shelf_index) {
  if (shelves && strlen(shelf_name) > 0) {
    for (int i = 0; i < list_length(shelves); i++) {
      obj elem;

      if (list_get(shelves, i, &elem)) {
        shelf_t *shelf = elem;

        if (strcmp(shelf->name, shelf_name) == 0) {
          if (shelf_index) {
            *shelf_index = i;
          }

          return shelf;
        }
      }
    }
  }

  return NULL;
}

static bool count_shelves_amount(obj elem, void *data) {
  shelf_t *shelf = elem;

  if (shelf) {
    int *count = data;
    (*count) += shelf->amount;
    return true;
  }

  return false;
}

void item_destructor(obj object) {
  item_t *item = (item_t *)object;
  release(item->shelves);
  release(item->name);
  release(item->desc);
}

void shelf_destructor(obj object) {
  shelf_t *shelf = (shelf_t *)object;
  release(shelf->name);
}
