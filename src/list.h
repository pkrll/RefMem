#ifndef __list_h__
#define __list_h__

#include <stdbool.h>

/// Define struct list in your .c file not here! (why?)
typedef struct list list_t;

/// Change this definition and replace int with the appropriate type
/// in your program. 

/// This function is used in list_delete() to allow the lists which are
/// the sole owners of their data to free the data on delete.
typedef void(*list_action)(void *elem);

short list_length(list_t *list);

void list_delete(list_t *list, list_action cleanup);
/**
 * @brief Check if list contains elem. If true return its index, if not
 *        append and return its new index.
 * @param list Pointer to list
 * @param elem Object to insert
 * @return index
 */
short list_expand(list_t *list, void *elem);

#endif
