#ifndef __list_h__
#define __list_h__

#include <stdbool.h>

typedef struct list list_t;

union element
{
  void          *p;
  size_t         s;
  void(*f)(void *);
} typedef element_t;


typedef bool(*element_comp_fun)(element_t, element_t);

/**
 * @brief           Creates a new list
 * @return          Pointer to list
 */
list_t *list_new();

/**
 * @brief           Return lists length.
 * @return          length
 */
unsigned short list_length(list_t *list);

/**
 * @brief           Deletes list and free all its links and
 *                  their element.
 * @param list      Pointer to list
 * @return          void
 */
void list_delete(list_t *list);

/**
 * @brief           Check if list contains elem. If true return its index, if not
 *                  append and return its new index.
 * @param list      Pointer to list
 * @param elem      Object to insert
 * @param cmp       compare function to check if list contains elem
 * @return          index of inserted elem or the placement of the elem
 */
unsigned short list_expand(list_t *list, element_t elem, element_comp_fun cmp);

/**
 * @brief           Return element at given index
 * @param           Pointer to list
 * @param           index
 * @return          element at index id
 * 
 */
element_t list_get(list_t *list, unsigned short id);

#endif
