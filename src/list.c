#include <stdio.h>
#include <stdlib.h>
#include "list.h"

typedef struct link link_t;

/**
 * @brief Traverses a linked list structure until it
 *        reaches the wanted index.
 *
 * While the function returns a pointer to the link
 * at index + 1 in the list, the pointer "head" that
 * is passed along to the function will be updated 
 * to the link at the actual index.
 *
 * @param  head  A pointer to the starting point of
 *               of the traversal.
 * @param  index The index of the wanted link.
 *
 * @return A pointer to the link at index index + 1.
 */
link_t *list_traverse(link_t **head, int index);

/**
 * @brief Creates a new link.
 *
 * @param  elem The element of the link.
 * @param  next Pointer to next member of the list.
 *
 * @return A pointer to the new link.
 */
link_t *link_new(L elem, link_t *next);
/**
 * @brief Chain two links together.
 *
 * The first supplied link will set as the
 * next member of the list the second link
 * passed along.
 *
 * @param  link A pointer to the link that will
 *         serve as the head of a pair of links.
 * @param  next A pointer to the link that will
 *         serve as the tail of a pair of links.
 *
 * @return True if the first link exists,
 *         otherwise false.
 */
bool link_with(link_t *link, link_t *next);

/**
 * @struct link
 * @brief A link in a structured list.
 *
 * @var link::elem The element of the link.
 * @var link::next A pointer to the next link.
 */
struct link {
  L pointer;
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
  int size;
  link_t *first;
  link_t *last;
};
 
link_t *list_traverse(link_t **head, int index) {
  link_t *link = (*head)->next;
  int i = 0;
  
  while (i < index) {
    *head = link;
    link = link->next;
    i++;
  }
  
  return link;
}

link_t *link_new(L elem, link_t *next) {
  link_t *new_link = malloc(sizeof(link_t));
  new_link->pointer = elem;
  new_link->next = next;
  
  return new_link;
}

bool link_with(link_t *link, link_t *next) {
  if (link != NULL) {
    link->next = next;
    return true;
  }

  return false;
}

list_t *list_new() {
  return calloc(1, sizeof(list_t));
}

void list_append(list_t *list, L elem) {
  link_t *new_link = link_new(elem, NULL);
  
  if (!link_with(list->last, new_link)) {
    list->first = new_link;
  }
  
  list->last = new_link;
  list->size += 1;
}

void list_prepend(list_t *list, L elem) {
  link_t *new_link = link_new(elem, NULL);

  if (!link_with(new_link, list->first)) {
    list->last = new_link;
  }
  
  list->first = new_link;
  list->size += 1;
}

bool list_insert(list_t *list, int index, L elem) {
  int length = list_length(list);

  if (index < 0) {
    index += length + 1;
  }

  if (index == 0) {
    list_prepend(list, elem);
    return true;
  } else if (index > length) {
    return false;
  } else if (index == length) {
    list_append(list, elem);
    return true;
  }

  // Vi skickar index - 1 för att göra det lättare att
  // länka den nya länken (som blir next till head).
  link_t *head = list->first;
  link_t *next = list_traverse(&head, index - 1);

  if (head != NULL) {
    link_t *new_link = link_new(elem, next);
    list->size += 1;
    return link_with(head, new_link);
  }

  return false;
}

bool list_remove(list_t *list, int index, L *elem) {
  int length = list_length(list);
  
  if (index < 0) {
    index += length +1;
  }
  
  if (index > length || length == 0) {
    return false;
  }

  link_t *head = list->first;

  if (index == 0) {
    *elem = head->pointer;
    list->first = head->next;
    
    free(head);

    list->size -= 1;
    
    return true;
  }

  // Vi skickar index - 1 för att vi vill ha vår head pekare på
  // föregående länk, vilket gör det lättare att peka om var
  // dess next ska vara
  link_t *link = list_traverse(&head, index - 1);

  if (link != NULL) {
    *elem = link->pointer;
    head->next = link->next;
    
    if (list->last == link) {
      list->last = head;
    }
    
    free(link);

    list->size -= 1;
    
    return true;
  }
    
  return false;
}

L *list_get(list_t *list, int index) {
  int length = list_length(list);
  
  if (index < 0) {
    index = length + index;
  }
  
  if (index < length) {
    link_t *current = list->first;
    list_traverse(&current, index);

    if (current != NULL) {
      return &(current->pointer);
    }
  }  

  return NULL;
}

L *list_first(list_t *list) {
  return list_get(list, 0);
}

L *list_last(list_t *list) {
  return list_get(list, -1);
}

int list_length(list_t *list) {
  return list->size;
}

void list_delete(list_t *list, list_action cleanup) {
  link_t *link = list->first;

  while (link) {
    if (cleanup) {
      cleanup(link->pointer);
    }
    link_t *tmp = link;
    link = link->next;
    free(tmp);
  }

  free(list);
}

void list_apply(list_t *list, list_action2 fun, void *data) {
  link_t *link = list->first;

  while (link) {
    if (fun) {
      fun(link->pointer, data);
    }

    link = link->next;
  }
}

short list_expand(list_t *list, L elem) {
  link_t *link = list->first;
  short index = 0;

  while(link->next != NULL) {
    if(link->pointer == elem) return index;
    link = link->next;
    index++;
  }
  list_append(list, elem);
  return index;
}
