#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "listset.h"

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
struct listset {
  link_t *first;
  link_t *last;
};

// -------------------------------
// Private
// -------------------------------

link_t *link_new(element_t elem, link_t *next) {
  link_t *new_link = calloc(1, sizeof(link_t));

  if (new_link) {
    new_link->pointer = elem;
    new_link->next = next;
  }

  return new_link;
}

// -------------------------------
// Public
// -------------------------------


listset_t *listset_new() {
  listset_t *list = calloc(1, sizeof(listset_t));

  return list;
}

unsigned short listset_length(listset_t *list) {
  unsigned short size = 0;
  link_t *link = list->first;

  while (link) {
    link = link->next;
    size += 1;
  }

  return size;
}

unsigned short listset_expand(listset_t *list, element_t elem, element_comp_fun cmp) {
  assert(cmp != NULL);
  unsigned short index = 0;
  link_t **link = &list->first;

  while (*link != NULL) {
    if (cmp((*link)->pointer, elem) == true) return index;
    index += 1;
    link = &(*link)->next;
  }

  *link = link_new(elem, NULL);
  list->last = *link;

  return index;
}

element_t listset_get(listset_t *list, unsigned short id) {

  link_t *link = list->first;
  unsigned short i = 0;
  
  while (i < id && link->next != NULL) {
    link = link->next;
    i += 1;
  }

  return link->pointer;
}

void listset_delete(listset_t *list) {
  link_t *link = list->first;

  while (link != NULL) {
    link_t *tmp = link;
    link = tmp->next;
    free(tmp);
  }

  free(list);
}
