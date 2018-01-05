#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "database.h"
#include "action.h"
#include "stack.h"
#include "tree.h"
#include "file.h"
#include "utils_goods.h"
#include "utils.h"
#include "common.h"
// -------------------------------
// Macros
// -------------------------------

// -------------------------------
// Structs & Enums
// -------------------------------

struct database {
  tree_t *goods;
  stack_s *actions;

  char *file_path;
};

struct database_iterator {
  int index;
  int size;

  item_t **elements;
};

// -------------------------------
// Declarations
// -------------------------------

static bool database_insert(database_t *database, item_t *item);
static bool database_remove(database_t *database, item_t *item, bool delete);
static void key_free(tree_key_t key);
static void element_free(obj elem);
static int element_compare(obj a, obj b);
static obj element_copy(obj elem);
static void add_undo(database_t *database, action_t *action);
// static void actions_free(void *action);
static bool database_write_to_file(FILE *file, void *data);
static void write_item_to_file(FILE *file, item_t *item);
static bool write_shelf_to_file(char *shelf, int amount, void *data);
static bool database_read_from_file(FILE *file, void *data);
static item_t *load_item_from_file(FILE *file);
static void load_shelves_from_file(FILE *file, item_t *item);

static void database_destructor(obj object);

// -------------------------------
// Public
// -------------------------------

static void database_destructor(obj object) {
  database_t *database = (database_t *)object;

  if (database->file_path) {
    save_file(database->file_path, database_write_to_file, database);
    release(database->file_path);
  }

  release(database->actions);
  release(database->goods);
}

database_t *database_new(char *file_path) {
  database_t *database = allocate(sizeof(database_t), database_destructor);

  if (database) {
    database->actions = stack_new(NULL);
    database->goods   = tree_new(element_copy, NULL, NULL, element_compare);

    retain(database->actions);
    retain(database->goods);

    if (file_path && strlen(file_path) > 0) {
      database->file_path = string_duplicate(file_path);
      retain(database->file_path);
      read_file(file_path, database_read_from_file, database);
    }
  }

  return database;
}

bool database_insert_item(database_t *database, item_t *item) {
  bool result = false;
  retain(item);

  if (database_insert(database, item)) {
    action_t *action = action_new_add(item);
    retain(action);
    add_undo(database, action);
    release(action);

    result = true;
  }

  release(item);
  return result;
}

bool database_update_item(database_t *database, item_t *old_item, item_t *new_item) {
  bool result = false;

  if (database && old_item && new_item) {
    retain(old_item);
    retain(new_item);

    tree_key_t old_name = item_get_name(old_item);
    retain(old_name);

    if (tree_remove(database->goods, old_name, NULL)) {
      if (database_insert(database, new_item)) {

        action_t *action = action_new_edit(old_item, new_item);
        add_undo(database, action);

        result = true;
      }
    }

    release(old_name);
    release(old_item);
    release(new_item);
  }

  return result;
}

bool database_remove_item(database_t *database, item_t *item) {
  bool result = false;
  retain(item);

  if (database_remove(database, item, false)) {
    action_t *action = action_new_remove(item);
    retain(action);
    add_undo(database, action);
    release(action);

    result = true;
  }

  release(item);

  return result;
}

bool database_has_item(database_t *database, char *name) {
  bool result = false;

  if (database && strlen(name) > 0) {
    retain(name);
    result = tree_has_key(database->goods, name);
    release(name);
  }

  return result;
}

item_t *database_get_item(database_t *database, char *name) {
  if (database && strlen(name) > 0) {
    retain(name);
    obj elem;

    if (tree_get(database->goods, name, &elem)) {
      release(name);
      return (item_t *)elem;
    }

    release(name);
  }

  return NULL;
}

bool database_location_is_valid(database_t *database, char *item_name, char *shelf) {
  if (strlen(item_name) == 0 || strlen(shelf) == 0) {
    return false;
  }

  retain(item_name);
  retain(shelf);

  bool result = true;

  if (database) {
    int size = tree_size(database->goods);
    item_t **items = (item_t **)tree_elements(database->goods);
    retain(items);

    for (int i = 0; i < size; i++) {
      item_t *item = items[i];
      if (item_has_shelf(item, shelf) && strcmp(item_name, item_get_name(item))) {
        result = false;
        break;
      }
    }

    release(items);
  }

  release(shelf);
  release(item_name);

  return result;
}

bool database_undo_action(database_t *database) {
  bool result = false;

  if (database) {
    action_t *action = stack_top(database->actions);
    retain(action);

    stack_pop(database->actions); // Fixes memleak as database doesnt have ownership of action before hand.

    if (action == NULL) return false;

    action_type type = action_get_type(action);

    if (type == ADD) {
      item_t *item  = action_get_saved(action);
      retain(item);
      result = database_remove(database, item, true);
      release(item);
    } else if (type == EDIT) {
      item_t *orig_item = action_get_original(action);
      item_t *edit_item = action_get_edited(action);

      retain(orig_item);
      retain(edit_item);

      if (database_remove(database, edit_item, true)) {
        result = database_insert(database, orig_item);
      }

      release(orig_item);
      release(edit_item);

    } else if (type == REMOVE) {
      item_t *item  = action_get_saved(action);
      retain(item);
      result = database_insert(database, item);
      release(item);
    }

    release(action);
  }

  return result;
}

bool database_apply(database_t *database, database_apply_fun fun, void *data) {
  bool result = false;

  if (database && fun) {
    tree_t *tree = database->goods;
    item_t **elem = (item_t **)tree_elements(tree);
    retain(elem);

    for (int i = 0; i < tree_size(tree); i++) {
      item_t *item = elem[i];

      if (item && fun(item, data)) {
        result = true;
      } else {
        break;
      }
    }

    release(elem);
  }

  return result;
}

bool database_is_sorted(database_t *database) {
  if (database) {
    tree_t *tree = database->goods;
    int depth = 0;
    return tree_is_balanced(tree, &depth);
  }

  return false;
}

bool database_sort(database_t *database) {
  if (database) {
    tree_t *tree = database->goods;

    return tree_balance(tree);
  }

  return false;
}

// -------------------------------
// Iterator functions
// -------------------------------

void database_iterator_destructor(obj object) {
  database_iter_t *iter = (database_iter_t *)object;
  release(iter->elements);
}

database_iter_t *database_iterator(database_t *database) {
  database_iter_t *iter = allocate(sizeof(database_iter_t), database_iterator_destructor);

  if (database) {
    iter->size = tree_size(database->goods);
    iter->index = 0;
    iter->elements = (item_t **)tree_elements(database->goods);
    retain(iter->elements);
  }

  return iter;
}

bool database_iterator_has_next(database_iter_t *iter) {
  if (iter) {
    return (iter->size > iter->index);
  }

  return false;
}

item_t *database_iterator_next(database_iter_t *iter) {
  if (iter && database_iterator_has_next(iter)) {
    item_t *item = iter->elements[iter->index];
    iter->index = iter->index + 1;

    return item;
  }

  return NULL;
}

item_t *database_iterator_get(database_iter_t *iter, int index) {
  if (iter && index < iter->size) {
    item_t *item = iter->elements[index];

    return item;
  }

  return NULL;
}

int database_iterator_size(database_iter_t *iter) {
  if (iter) {
    return iter->size;
  }

  return 0;
}

void database_iterator_delete(database_iter_t *iter) {
  if (iter) {
    free(iter->elements);
    free(iter);
  }
}

// -------------------------------
// Private
// -------------------------------

static bool database_insert(database_t *database, item_t *item) {
  bool result = false;

  if (database && item) {
    retain(item);
    char *name = item_get_name(item);
    tree_key_t key = string_duplicate(name);
    retain(key);

    result = tree_insert(database->goods, key, item);

    release(key);
    release(item);
  }

  return result;
}

static bool database_remove(database_t *database, item_t *item, bool delete) {
  bool result = false;

  if (database && item) {
    char *name = item_get_name(item);
    retain(name);

    result = tree_remove(database->goods, name, NULL);

    release(name);
  }

  return result;
}

static void add_undo(database_t *database, action_t *action) {
  if (database && database->actions) {
    retain(action);
    stack_push(database->actions, action);
    release(action);
  }
}

static void key_free(tree_key_t tree_key) {
  release(tree_key);
}

static void element_free(obj elem) {
  release(elem);
}

static obj element_copy(obj elem) {
  if (elem) {
    item_t *item = (item_t *)elem;
    item_t *copy = item_copy(item);

    return copy;
  }

  return elem;
}

static int element_compare(obj a, obj b) {
  char *name_a = a;
  char *name_b = b;

  if (name_a && name_b) {
    return strcmp(name_a, name_b);
  }

  return 1;
}

// -------------------------------
// Private: Read / Write
// -------------------------------

static bool database_write_to_file(FILE *file, void *data) {
  if (file && data) {
    database_t *database  = data;
    database_iter_t *iter = database_iterator(database);
    retain(iter);
    int size = tree_size(database->goods);

    fprintf(file, "%d\n", size);

    while (database_iterator_has_next(iter)) {
      item_t *item = database_iterator_next(iter);
      write_item_to_file(file, item);
    }

    release(iter);

    return true;
  }

  return false;
}

static void write_item_to_file(FILE *file, item_t *item) {
  if (file && item) {
    retain(item);
    char *name = item_get_name(item);
    char *desc = item_get_desc(item);
    int cost   = item_get_cost(item);
    int size   = item_number_of_shelves(item);

    fprintf(file, "%s\n%s\n%d\n%d\n", name, desc, cost, size);

    item_apply_on_shelves(item, write_shelf_to_file, file);
    release(item);
  }
}

static bool write_shelf_to_file(char *shelf, int amount, void *data) {
  if (shelf && data) {
    FILE *file = data;
    fprintf(file, "%s\n%d\n", shelf, amount);

    return true;
  }

  return false;
}

static bool database_read_from_file(FILE *file, void *data) {
  if (file && data) {
    database_t *database = data;

    int size = 0;
    fscanf(file, "%d\n", &size);

    for (int i = 0; i < size; i++) {
      item_t *item = load_item_from_file(file);
      retain(item);
      database_insert(database, item);
      release(item);
    }

    return true;
  }

  return false;
}

static item_t *load_item_from_file(FILE *file) {
  if (file) {
    // By setting bytes to 0, and initializing name,
    // desc, cost to NULL == getline allocates a buffer
    // for storing the line.
    unsigned long bytes  = 0;

    char *name = NULL;
    char *desc = NULL;
    char *cost = NULL;

    getline(&name, &bytes, file);
    getline(&desc, &bytes, file);
    getline(&cost, &bytes, file);

    name = strip_newline(name);
    desc = strip_newline(desc);
    cost = strip_newline(cost);

    item_t *item = item_new();
    item_set_name(item, name);
    item_set_desc(item, desc);
    item_set_cost(item, atoi(cost));

    load_shelves_from_file(file, item);

    free(name);
    free(desc);
    free(cost);

    return item;
  }

  return NULL;
}

static void load_shelves_from_file(FILE *file, item_t *item) {
  unsigned long bytes = 0;
  // Number of shelves
  int shelf_size = 0;
  fscanf(file, "%d\n", &shelf_size);

  while (shelf_size > 0) {
    char *shelf_name   = NULL;
    char *shelf_amount = 0;

    getline(&shelf_name, &bytes, file);
    getline(&shelf_amount, &bytes, file);

    shelf_name   = strip_newline(shelf_name);
    shelf_amount = strip_newline(shelf_amount);

    item_set_shelves(item, shelf_name, atoi(shelf_amount), true);

    free(shelf_name);
    free(shelf_amount);

    shelf_size--;
  }

}
