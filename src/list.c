#include <stdio.h>
#include <stdlib.h>
#include "list.h"

typedef struct link link_t;

/**
 * @struct link
 * @brief A link in a structured list.
 *
 * @var link::elem The element of the link.
 * @var link::next A pointer to the next link.
 */
struct link {
  void *pointer;
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
  short size;
  link_t *first;
  link_t *last;
};

link_t *link_new(void *elem, link_t *next) {
  link_t *new_link = calloc(1, sizeof(link_t));
  new_link->pointer = elem;
  new_link->next = next;
  
  return new_link;
}

list_t *list_new() {
  return calloc(1, sizeof(list_t));
}

short list_length(list_t *list) {
    return list->size;
}

short list_expand(list_t *list, void *elem) {
    if(list == NULL) list = list_new();
    
    link_t *link = list->first;
    short index = 0;

    if(link == NULL) {
        link = link_new(elem, NULL);
        list->first = link;
        list->last = link;
        return index;
    }

    while(link != NULL) {
        index += 1;
        link = link->next;
    }

    link = link_new(elem, NULL);
    list->last = link;
    list->size += 1;
    return index;
}

void *list_get(list_t *list, short id) {
    if(list == NULL || id >= list->size || id < 0) return NULL;

    link_t *link = list->first;

    for(short i = 0; i < id; i++) link = link->next;

    return link->pointer;
}