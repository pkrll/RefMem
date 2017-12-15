/**
 * @file
 * @brief Binary search tree data structure
 */

#ifndef __tree_h__
#define __tree_h__

#include <stdbool.h>

typedef struct tree tree_t;

typedef void* T;

/// Creates a new tree
///
/// \returns: empty tree
tree_t *tree_new();

/// Remove a tree along with all T elements.
///
/// \param tree the tree
/// \param cleanup a function that takes a key and element as
///        argument, to be used to free memory. If this param is
///        NULL, no cleanup of keys or elements will happen.
void tree_delete(tree_t *tree);

/// Get the size of the tree
///
/// \returns: the number of nodes in the tree
int tree_size(tree_t *tree);

int tree_height(tree_t *node);

/// Get the depth of the tree
///
/// \returns: the depth of the deepest subtree
int tree_depth(tree_t *tree);

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

///
/// \param tree pointer to the tree
/// \param elem the elem of elem to be removed
/// \returns: the removed element
bool tree_remove(tree_t *tree, T elem);

#endif
