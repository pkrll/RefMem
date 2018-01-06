#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"

typedef struct link link_t;

// -------------------------------
// Structs & Enums
// -------------------------------

struct link {
  obj elem;
  link_t *next;
};

struct list {
  link_t *first;
  link_t *last;
  size_t size;

  element_copy_fun copy;
  element_free_fun free;
  element_comp_fun comp;
};

// -------------------------------
// Declarations
// -------------------------------

/**
 * @brief Finds and returns a pointer to the pointer that
 *        points to the link with the given index.
 *
 * @note   The `link` pointer passed along will be updated
 *         to point to the link before the link with the
 *         wanted index. If the index is 0, it will point
 *         to the same pointer.
 *
 * @param  link  A pointer to a pointer to a link.
 * @param  index The wanted index.
 *
 * @return A pointer to the pointer pointing to the link.
 */
static link_t **find_link_by_index(link_t **link, int index);
static link_t *link_new(obj elem, link_t *next, element_copy_fun copy);
static int adjust_index(int index, int size);
static obj id(obj elem);
static int comp(obj a, obj b);

static void list_destructor(obj object);
static void link_destructor(obj object);

list_t *list_new(element_copy_fun copy, element_free_fun free, element_comp_fun compare) {
  list_t *list = allocate(sizeof(list_t), list_destructor);

  if (list) {
    list->size = 0;
    list->copy = (copy != NULL) ? copy : id;
    list->free = free;
    list->comp = (compare != NULL) ? compare : comp;
  }

  return list;
}

void list_insert(list_t *list, int index, obj elem) {
  if (list) {
    int size = list_length(list);
    index = adjust_index(index, size);

    link_t *head  = list->first;
    link_t **next = find_link_by_index(&head, index);

    *next = link_new(elem, *next, list->copy);

    if (index == 0) {
      release(list->first); // Prevents mem leak
      list->first = *next;
      retain(list->first);
    } else {
      retain(*next);
    }

    if (index == size) {
      release(list->last);
      list->last = *next;
      retain(list->last);
    }

    list->size = list->size + 1;
  }
}

void list_append(list_t *list, obj elem) {
  list_insert(list, -1, elem);
}

void list_prepend(list_t *list, obj elem) {
  list_insert(list, 0, elem);
}

void list_remove(list_t *list, int index, bool delete) {
  if (list) {
    int size = list_length(list);
    index = adjust_index(index, size);
    // As adjust_index may return size, we need to adjust our adjustment because to live is to adjust
    if (index == size) index = index - 1;

    link_t *head  = list->first;
    link_t **link = find_link_by_index(&head, index);

    if (*link) {
      link_t *link_to_remove = *link;
      *link = (*link)->next;

      if (index == 0) {
        list->first = *link;
        if (list->first) {
          retain(list->first);
        }
      } else {
        if (link_to_remove->next) {
          retain(link_to_remove->next); // fixes issue
        }
      }

      release(link_to_remove);

      if (index + 1 == size) {
        release(list->last);
        list->last = head;

        if (list->last) {
          retain(list->last);
        }
      }

      list->size = list->size - 1;
    }
  }
}

bool list_get(list_t *list, int index, obj *result) {
  if (list == NULL || index < 0) return false;

  link_t *head  = list->first;
  link_t **link = find_link_by_index(&head, index);

  if (*link) {
    *result = (*link)->elem;
  }

  return (*link != NULL);
}

bool list_first(list_t *list, obj *result) {
  if (list && list->first) {
    *result = list->first->elem;

    return true;
  }

  return false;
}

bool list_last(list_t *list, obj *result) {
  if (list && list->last) {
    *result = list->last->elem;

    return true;
  }

  return false;
}

int list_length(list_t *list) {
  if (list) {
    return list->size;
  }

  return 0;
}

bool list_apply(list_t *list, elem_apply_fun fun, void *data) {
  bool result = false;

  if (list) {
    link_t *link = list->first;

    while (link) {
      if (fun(link->elem, data)) {
        result = true;
      }

      link = link->next;
    }
  }

  return result;
}

int list_contains(list_t *list, obj elem) {
  if (list) {
    link_t *link = list->first;
    int i = 0;

    while (link != NULL) {
      obj cmp_elem = link->elem;

      if (list->comp(cmp_elem, elem) == 0) {
        return i;
      }

      link = link->next;
      i++;
    }
  }

  return -1;
}

list_t *list_copy(list_t *list_from) {
  list_t *copied_list = list_new(list_from->copy,
                                 list_from->free,
                                 list_from->comp);
  return copied_list;
}


// -------------------------------
// Private
// -------------------------------

static obj id(obj elem) {
  return elem;
}
// FIXME: HOW TO DO THIS COMPARISION?
static int comp(obj a, obj b) {
  if (a > b) return 1;
  if (a < b) return -1;

  return 0;
}

static link_t *link_new(obj elem, link_t *next, element_copy_fun copy) {
  link_t *link = allocate(sizeof(link_t), link_destructor);

  if (link) {
    link->elem = (copy) ? copy(elem) : elem;
    link->next = next;

    if (link->next) {
      retain(link->next);
    }

    retain(link->elem);
  }

  return link;
}

static int adjust_index(int index, int size) {
  if (index < 0) {
    index = index + size + 1;

    if (index > size) {
      return size;
    } else if (index < 0) {
      return 0;
    }

    return index;
  }

  if (index > size) {
    return size;
  }

  return index;
}

static link_t **find_link_by_index(link_t **link, int index) {
  int i = 0;
  link_t **current = link;

  while (i < index && *current) {
    *link = *current;
    current = &(*current)->next;
    i++;
  }

  return current;
}

static void list_destructor(obj object) {
  list_t *list = (list_t *)object;

  release(list->first);
  release(list->last);
}

static void link_destructor(obj object) {
  link_t *link = (link_t *)object;

  release(link->elem);
  release(link->next);
}
