/**
 * @file
 * @brief Binary search tree data structure
 */

#ifndef __tree_h__
#define __tree_h__

#include <stdbool.h>

typedef void* T;
typedef struct tree tree_t;
typedef void(*tree_apply_func)(T elem, void *data);
typedef void(*tree_free_func)(T elem);

/// Creates a new tree
///
/// \param free_func (may be NULL) used to free elements in tree_delete
/// \returns: empty tree
tree_t *tree_new(tree_free_func free_func);

/// Insert element into the tree. Returns false if the key is already used.
///
/// \param tree pointer to the tree
/// \param key the key of element to be appended
/// \param elem the element
/// \returns: true if successful, else false
bool tree_insert(tree_t *tree, T elem);

/// Checks whether a key is used in a tree
///
/// \param tree pointer to the tree
/// \param elem the elem to check for inclusion in the tree
bool tree_has_key(tree_t *tree, T elem);

/// Removes the specified element from the tree
///
/// \param tree pointer to the tree
/// \param elem the elem of elem to be removed
/// \returns: the removed element
bool tree_remove(tree_t *tree, T elem);

/// Get the size of the tree
///
/// \returns: the number of nodes in the tree
int tree_size(tree_t *tree);

/// Get the height of the tree
///
/// \returns: the height of the tree
int tree_height(tree_t *tree);

/// Remove a tree along with all T elements.
///
/// \param tree the tree
/// \param delete_elems if true, run tree's free_func function on all elements
void tree_delete(tree_t *tree, bool delete_elems);

/// Applies a function to all elements in the tree inorder.
///
/// \param tree the tree
/// \param func the function to apply to all elements
/// \param data an extra argument passed to each call to fun (may be NULL)
void tree_apply(tree_t *tree, tree_apply_func function, void *data);

/// Returns an array with all the elements.
///
/// \param tree the tree
/// \returns: an array with all the elements
T *to_array(tree_t *tree);

#endif
