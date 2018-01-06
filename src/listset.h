#ifndef __listset_h__
#define __listset_h__

#include <stdbool.h>

typedef struct listset listset_t;

union element
{
  void          *p;
  size_t         s;
  void(*f)(void *);
} typedef element_t;

typedef bool(*element_comp_fun)(element_t, element_t);

/**
 * @brief           Creates a new list.
 *
 * @return          A new list.
 */
listset_t *listset_new();
/**
 * @brief           Return a list's length.
 *
 * @param list      Pointer to a list
 * @return          Length of the list
 */
unsigned short listset_length(listset_t *list);
/**
 * @brief           Deletes a list.
 *
 * @param list      Pointer to a list
 */
void listset_delete(listset_t *list);
/**
 * @brief           Inserts an element into a list.
 *
 *                  If the element is already present in the list, it will not be inserted.
 *                  The return value will be the index of the element already present.
 *
 * @param list      Pointer to a list.
 * @param elem      Object to insert.
 * @param cmp       A compare function to check if list contains elem.
 * @return          Index of inserted elem or the placement of the elem.
 */
unsigned short listset_expand(listset_t *list, element_t elem, element_comp_fun cmp);
/**
 * @brief           Return element at the given index
 *
 * @param list      Pointer to a list.
 * @param           index of the element to retrieve.
 * @return          The element at the given index (or NULL if index is out of bounds).
 */
element_t listset_get(listset_t *list, unsigned short id);

#endif
