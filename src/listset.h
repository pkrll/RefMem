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
 * @brief           Creates a new list
 * @return          Pointer to list
 */
listset_t *listset_new();

/**
 * @brief           Return lists length.
 * @return          length
 */
unsigned short listset_length(listset_t *list);

/**
 * @brief           Deletes list and free all its links and
 *                  their element.
 * @param list      Pointer to list
 * @return          void
 */
void listset_delete(listset_t *list);

/**
 * @brief           Check if list contains elem. If true return its index, if not
 *                  append and return its new index.
 * @param list      Pointer to list
 * @param elem      Object to insert
 * @param cmp       compare function to check if list contains elem
 * @return          index of inserted elem or the placement of the elem
 */
unsigned short listset_expand(listset_t *list, element_t elem, element_comp_fun cmp);

/**
 * @brief           Return element at given index
 * @param           Pointer to list
 * @param           index
 * @return          element at index id
 *
 */
element_t listset_get(listset_t *list, unsigned short id);

#endif
