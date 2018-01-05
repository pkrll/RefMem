#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "tree.h"

typedef struct node node_t;

struct node {
  tree_key_t key;
  obj elem;

  node_t *right;
  node_t *left;
};

struct tree {
  node_t *root;

  element_copy_fun copy;
  key_free_fun     key_free;
  element_free_fun elem_free;
  element_comp_fun compare;
};

// -------------------------------
// Declarations
// -------------------------------

static obj id(obj e);
static int comp(obj a, obj b);
static void no_free(obj e);
static node_t *node_new();
static int node_size(node_t *node, int acc);
static int node_depth(node_t *node);
/**
 * @brief Retrieves the node with the smallest key, which is
 *        the left most node in a given sub tree.
 *
 * @note  The `root` parameter passed along to the function
 *        will be updated to point to the pointer that points
 *        to the parent of the node that is returned.
 *
 *        Example:
 *
 *          node_t *rt  = tree->root;
 *          node_t **n = node_minimum(&rt);
 *
 *        The above example will change the `rt` variable to
 *        point to the parent node of `n`.
 *
 * @param  root A pointer to a pointer to the starting node.
 *
 * @return A pointer to the pointer that is the left most node.
 */
static node_t **node_minimum(node_t **root);
static void node_keys(node_t *node, tree_key_t *keys, int *index);
static void node_elements(node_t *node, obj *elems, int *index);
static bool node_apply(node_t *node, enum tree_order order, key_elem_apply_fun fun, void *data);
/**
 * @brief Traverses a binary search tree with regards to the
 *        inputed key until the node with the same key or a
 *        leaf has been reached.
 *
 *        The `parent` parameter passed along to the function
 *        will be updated to point to the pointer that points
 *        to the parent of the node to be returned.
 *
 *        Example:
 *
 *          node_t *head = tree->root;
 *          node_t **node = find_node_at_key(&head, k, c);
 *
 *        will update the `head` pointer to be the parent of
 *        the node with the provided key k. Note that if the
 *        tree is empty, or if the node  'node' will hold the
 *        same adress as head does.
 *
 * @param  head A pointer to the pointer to the starting node.
 * @param  key  The key of the node.
 * @param  comp The function used to compare keys.
 *
 * @return A pointer to the pointer that should hold the node.
 */
static node_t **find_node_at_key(node_t **parent, tree_key_t key, element_comp_fun compare);
static void node_delete(node_t *node, element_free_fun elem_free, key_free_fun key_free);
static void construct_balanced_tree(tree_t *tree, tree_key_t *keys, obj *elements, int start, int end);
static bool node_is_balanced(node_t *node, int *depth);
static void empty_tree(tree_t *tree, bool delete_keys, bool delete_elements);

static void tree_destructor(obj object);
static void node_destructor(obj object);

// -------------------------------
// Public
// -------------------------------

tree_t *tree_new(element_copy_fun element_copy, key_free_fun key_free, element_free_fun elem_free, element_comp_fun compare) {
  tree_t *tree = allocate(sizeof(tree_t), tree_destructor);

  if (tree) {
    tree->copy = element_copy ? element_copy : id;
    tree->key_free = key_free ? key_free : no_free;
    tree->elem_free = elem_free ? elem_free : no_free;
    tree->compare = compare ? compare : comp;
  }

  return tree;
}

void tree_delete(tree_t *tree, bool delete_keys, bool delete_elements) {
  if (tree) {
    empty_tree(tree, delete_keys, delete_elements);
    release(tree);
  }
}

int tree_size(tree_t *tree) {
  if (tree) {
    return node_size(tree->root, 0);
  }

  return 0;
}

int tree_depth(tree_t *tree) {
  if (tree) {
    return node_depth(tree->root);
  }

  return 0;
}

bool tree_insert(tree_t *tree, tree_key_t key, obj elem) {
  assert(tree);

  node_t *head  = tree->root;
  node_t **node = find_node_at_key(&head, key, tree->compare);

  if (*node == NULL) {
    *node = node_new();
    (*node)->elem = tree->copy(elem);
    (*node)->key = key;

    retain((*node)->key);
    retain((*node)->elem);
    retain(*node);

    if (tree->root == NULL) tree->root = *node;
  } else {
    return false;
  }

  return true;
}

bool tree_has_key(tree_t *tree, tree_key_t key) {
  if (tree) {
    retain(key);

    node_t *head  = tree->root;
    node_t **node = find_node_at_key(&head, key, tree->compare);

    release(key);

    return (*node != NULL);
  }

  return false;
}

bool tree_get(tree_t *tree, tree_key_t key, obj *result) {
  assert(result);

  if (tree) {
    node_t *head  = tree->root;
    node_t **node = find_node_at_key(&head, key, tree->compare);

    if (*node) {
      *result = (*node)->elem;
      return true;
    }
  }

  return false;
}

bool tree_remove(tree_t *tree, tree_key_t key, obj *result) {
  if (tree) {
    node_t **parent = &tree->root;
    node_t **child = find_node_at_key(parent, key, tree->compare);

    if (*child) {
      node_t *node_to_delete = *child;

      if (node_to_delete->left == NULL) {
        *child = node_to_delete->right;
        retain(*child);
      } else if (node_to_delete->right == NULL) {
        *child = node_to_delete->left;
        retain(*child);
      } else {
        node_t *minimum_root = node_to_delete->right;
        node_t *minimum_node = *node_minimum(&minimum_root);

        if (minimum_root != minimum_node) {
          minimum_root->left = minimum_node->right;
          minimum_node->right = node_to_delete->right;
        }

        minimum_node->left = node_to_delete->left;
        *child = minimum_node;

        retain(node_to_delete->left);
        retain(node_to_delete->right);
      }

      if (result) *result = node_to_delete->elem;

      release(node_to_delete);

      return true;
    }
  }

  return false;
}

tree_key_t *tree_keys(tree_t *tree) {
  int size = tree_size(tree);
  tree_key_t *keys = allocate_array(size, sizeof(tree_key_t), NULL);

  if (tree) {
    int i = 0;
    node_keys(tree->root, keys, &i);
  }

  return keys;
}

obj *tree_elements(tree_t *tree) {
  int size = tree_size(tree);
  obj *elems = allocate_array(size, sizeof(obj), NULL);

  if (tree) {
    int i = 0;
    node_elements(tree->root, elems, &i);
  }

  return elems;
}

bool tree_apply(tree_t *tree, enum tree_order order, key_elem_apply_fun fun, void *data) {
  if (tree) {
    return node_apply(tree->root, order, fun, data);
  }

  return false;
}

bool tree_is_balanced(tree_t *tree, int *depth) {
  assert(tree);

  if (tree->root) {
    return node_is_balanced(tree->root, depth);
  }

  return true;
}

bool tree_balance(tree_t *tree) {
  if (tree_is_balanced(tree, NULL)) return false;

  int size = tree_size(tree);
  obj *elements = allocate_array(size, sizeof(obj), NULL);
  tree_key_t *keys = allocate_array(size, sizeof(tree_key_t), NULL);

  int i = 0, x = 0;

  node_elements(tree->root, elements, &i);
  node_keys(tree->root, keys, &x);

  for (int j = 0; j < size; j++) {
    retain(elements[j]);
    retain(keys[j]);
  }

  empty_tree(tree, false, false);

  construct_balanced_tree(tree, keys, elements, 0, size - 1);

  for (int i = 0; i < size; i++) {
    release(elements[i]);
    release(keys[i]);
  }

  deallocate(elements);
  deallocate(keys);

  return true;
}

// -------------------------------
// Private
// -------------------------------

static obj id(obj e) {
  return e;
}

static int comp(obj a, obj b) {
  if (a > b) return 1;
  if (a < b) return -1;

  return 0;
}

static void no_free(obj e) {

}

static node_t *node_new() {
  return allocate(sizeof(node_t), node_destructor);
}

static int node_size(node_t *node, int acc) {
  if (node) {
    return node_size(node->right, node_size(node->left, 1 + acc));
  }

  return acc;
}

static int node_depth(node_t *node) {
  if (node) {
    int left_depth  = node_depth(node->left) + 1;
    int right_depth = node_depth(node->right) + 1;

    return (left_depth > right_depth) ? left_depth : right_depth;
  }

  return 0;
}

static node_t **node_minimum(node_t **root) {
  node_t **node = root;

  while (*node && (*node)->left) {
    node = &(*node)->left;
  }

  return node;
}

static void node_keys(node_t *node, tree_key_t *keys, int *index) {
  if (node) {
    // Run the function on the left sub tree
    node_keys(node->left, keys, index);
    // Here we actually add it to the the array
    keys[*index] = node->key;
    (*index)++;
    // Run the function on the right subtree
    node_keys(node->right, keys, index);
  }
}

static void node_elements(node_t *node, obj *elems, int *index) {
  if (node) {
    node_elements(node->left, elems, index);
    elems[*index] = node->elem;
    (*index)++;
    node_elements(node->right, elems, index);
  }
}

static bool node_apply(node_t *node, enum tree_order order, key_elem_apply_fun fun, void *data) {
  bool result_left = false;
  bool result_root = false;
  bool result_right = false;
  // Can we pretty up this function somehow?
  if (node) {
    if (order == inorder) {
      result_left  = node_apply(node->left, order, fun, data);
      result_root  = fun(node->key, node->elem, data);
      result_right = node_apply(node->right, order, fun, data);
    } else if (order == preorder) {
      result_root  = fun(node->key, node->elem, data);
      result_left  = node_apply(node->left, order, fun, data);
      result_right = node_apply(node->right, order, fun, data);
    } else if (order == postorder) {
      result_left  = node_apply(node->left, order, fun, data);
      result_right = node_apply(node->right, order, fun, data);
      result_root  = fun(node->key, node->elem, data);
    }
  }

  return (result_left || result_root || result_right);
}


static node_t **find_node_at_key(node_t **parent, tree_key_t key, element_comp_fun compare) {
  node_t **current = parent;

  while (*current) {
    int cmp = compare((*current)->key, key);

    if (cmp > 0) {
      parent = current;
      current = &((*current)->left);
    } else if (cmp < 0) {
      parent = current;
      current = &((*current)->right);
    } else {
      return current;
    }
  }

  return current;
}

static void node_delete(node_t *node, element_free_fun elem_free, key_free_fun key_free) {
  if (node) {
    node_delete(node->left, elem_free, key_free);
    node_delete(node->right, elem_free, key_free);

    // if (elem_free) elem_free(node->elem);
    // if (key_free)  key_free(node->key);

    release(node);
  }
}

static void construct_balanced_tree(tree_t *tree, tree_key_t *keys, obj *elements, int start, int end) {
  if (tree && keys && elements && start <= end) {
    int middle = (start + end) / 2;

    tree_insert(tree, keys[middle], elements[middle]);

    construct_balanced_tree(tree, keys, elements, start, middle - 1);
    construct_balanced_tree(tree, keys, elements, middle + 1, end);
  }
}

static bool node_is_balanced(node_t *node, int *depth) {
  if (node) {
    bool left  = node_is_balanced(node->left, depth);
    bool right = node_is_balanced(node->right, depth);
    int height = abs(node_depth(node->left) - node_depth(node->right));

    if (depth && *depth < height) {
      *depth = height;
    }

    return left && right && height <= 1;
  }

  return true;
}

static void empty_tree(tree_t *tree, bool delete_keys, bool delete_elements) {
  if (tree) {
    element_free_fun elem_free = (delete_elements) ? tree->elem_free : NULL;
    key_free_fun key_free = (delete_keys) ? tree->key_free : NULL;

    node_delete(tree->root, elem_free, key_free);
    tree->root =  NULL;
  }
}

static void tree_destructor(obj object) {
  tree_t *tree = (tree_t *)object;
  release(tree->root);
}

static void node_destructor(obj object) {
  node_t *node = (node_t *)object;
  release(node->key);
  release(node->elem);
  release(node->right);
  release(node->left);
}


void node_print(node_t *node) {
  if (node) {
    printf("Left: \n");
    node_print(node->left);
    printf("- Root: %d\n", *(int*)node->key);
    printf("Right: \n");
    node_print(node->right);
  } else {
    printf("-");
  }
}

void tree_print(tree_t *tree) {
  node_print(tree->root);
}
