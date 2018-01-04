#include <stdio.h>
#include <stdlib.h>
#include "list.h"

typedef struct link link_t;

// -------------------------------
// Structs
// -------------------------------

/**
 * @struct link
 * @brief A link in a structured list.
 *
 * @var link::elem The element of the link.
 * @var link::next A pointer to the next link.
 */
struct link {
  element_t pointer;
  link_t *next;
};

/**
 * @struct list
 * @brief A structured list
 *
 * @var list::first The first link in the list.
 * @var list::last  The last link in the list.
 * @var list::size  Size of the list.
 */
struct list {
  unsigned short size;
  link_t *first;
  link_t *last;
};

// -------------------------------
// Public
// -------------------------------

link_t *link_new(element_t elem, link_t *next) {
  link_t *new_link = calloc(1, sizeof(link_t));
  new_link->pointer = elem;
  new_link->next = next;

  return new_link;
}

list_t *list_new() {
  return calloc(1, sizeof(list_t));
}

unsigned short list_length(list_t *list) {
  return list->size;
}

unsigned short list_expand(list_t *list, element_t elem, element_comp_fun cmp) {
  unsigned short index = 0;
  link_t *link = list->first;

  if (link == NULL) {
    link_t *link = link_new(elem, NULL);
    list->first = link;
    list->last = link;
    list->size += 1;
    return index;
  }

  while (link->next != NULL) {
    if (cmp(link->pointer, elem) == true) return index;
    index += 1;
    link = link->next;
  }

  link_t *newlink = link_new(elem, NULL);
  link->next = newlink;
  list->last = newlink;
  list->size += 1;
  return index;
}

element_t list_get(list_t *list, unsigned short id) {

  link_t *link = list->first;
  unsigned short i = 0;
  while (i <= id && link->next != NULL) {
    link = link->next;
    i += 1;
  }

  return link->pointer;
}

void list_delete(list_t *list) {
  link_t *link = list->first;

  while (link != NULL) {
    link_t *tmp = link;
    link = tmp->next;
    free(tmp);
  }

  free(list);
}
