/**
 * @file
 * @brief Binary search tree data structure
 */

#ifndef __treeset_h__
#define __treeset_h__

#include <stdbool.h>

typedef void* T;
typedef struct treeset treeset_t;
typedef void(*treeset_apply_func)(T elem, void *data);
typedef void(*treeset_free_func)(T elem);

/// Creates a new tree set
///
/// \param free_func (may be NULL) used to free elements in treeset_delete
/// \returns: empty tree set
treeset_t *treeset_new(treeset_free_func free_func);

/// Insert element into the tree set. Returns false if the key is already used.
///
/// \param tree set pointer to the tree
/// \param key the key of element to be appended
/// \param elem the element
/// \returns: true if successful, else false
bool treeset_insert(treeset_t *tree, T elem);

/// Checks whether a key is used in a tree
///
/// \param tree pointer to the tree
/// \param elem the elem to check for inclusion in the tree
bool treeset_has_key(treeset_t *tree, T elem);

/// Removes the specified element from the tree
///
/// \param tree pointer to the tree
/// \param elem the elem of elem to be removed
/// \returns: the removed element
bool treeset_remove(treeset_t *tree, T elem);

/// Get the size of the tree
///
/// \returns: the number of nodes in the tree
int treeset_size(treeset_t *tree);

/// Get the height of the tree
///
/// \returns: the height of the tree
int treeset_height(treeset_t *tree);

/// Remove a tree along with all T elements.
///
/// \param tree the tree
/// \param delete_elems if true, run tree's free_func function on all elements
void treeset_delete(treeset_t *tree, bool delete_elems);

/// Applies a function to all elements in the tree inorder.
///
/// \param tree the tree set
/// \param func the function to apply to all elements
/// \param data an extra argument passed to each call to fun (may be NULL)
void treeset_apply(treeset_t *tree, treeset_apply_func function, void *data);

/// Returns an array with all the elements.
///
/// \param tree the tree set
/// \returns: an array with all the elements
T *to_array(treeset_t *tree);

#endif
