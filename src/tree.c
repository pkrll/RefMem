#include <stdlib.h>
#include <stdio.h>
#include "tree2.h"

struct node {
  T element;
  int height;

  struct node *left;
  struct node *right;

} typedef node_t;

struct tree {
  node_t *root;
  tree_free_func free;
} typedef tree_t;

// -------------------------------
// Declarations
// -------------------------------

static int max(int a, int b);
static int compare(T a, T b);
static node_t *node_new(T elem);
static node_t **node_get(node_t **parent, T elem);
static node_t **node_minimum(node_t **root);
static node_t *node_rotate_right(node_t *y);
static node_t *node_rotate_left(node_t *x);
static bool node_insert(node_t **node, T elem);
static int node_height(node_t *node);
static int node_get_balance(node_t *node);
static int node_size(node_t *node, int acc);
static bool node_remove(node_t **node, T elem);
static void node_delete(node_t *node, tree_free_func free_func, bool delete_elems);
static void node_apply(node_t *node, tree_apply_func function, void *data);
// -------------------------------
// Public functions
// -------------------------------

tree_t *tree_new(tree_free_func free_func) {
  tree_t *tree = calloc(1, sizeof(tree_t));

  if (tree) {
    tree->free = free_func;
  }

  return tree;
}

bool tree_insert(tree_t *tree, T elem) {
  if (tree->root == NULL) {
    tree->root = node_new(elem);
    return true;
  }

  return node_insert(&(tree->root), elem);
}

bool tree_has_key(tree_t *tree, T elem) {
  if (tree != NULL) {
    node_t *head  = tree->root;
    node_t **node = node_get(&head, elem);

    return (*node != NULL);
  }

  return false;
}

bool tree_remove(tree_t *tree, T elem) {
  return node_remove(&(tree->root), elem);
}

int tree_size(tree_t *tree) {
  if (tree) {
    return node_size(tree->root, 0);
  }

  return 0;
}

int tree_height(tree_t *tree) {
  if (tree->root == NULL) return 0;
  return tree->root->height;
}

void tree_delete(tree_t *tree, bool delete_elems) {
  if (tree) {
    node_delete(tree->root, tree->free, delete_elems);
    free(tree);
  }
}

void tree_apply(tree_t *tree, tree_apply_func function, void *data) {
  if (tree) {
    node_apply(tree->root, function, data);
  }
}

// -------------------------------
// Private functions
// -------------------------------

static int max(int a, int b) {
  return (a > b) ? a : b;
}

static int compare(T a, T b) {
  if (a < b) {
    return -1;
  } else if (a > b) {
    return 1;
  }

  return 0;
}

static node_t *node_new(T elem) {
  node_t *node = calloc(1, sizeof(node_t));

  if (node != NULL) {
    node->element = elem;
  }

  return node;
}

static node_t **node_get(node_t **parent, T elem) {
  node_t **current = parent;

  while (*current) {
    int cmp = compare((*current)->element, elem);

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

static node_t **node_minimum(node_t **root) {
  node_t **node = root;

  while (*node && (*node)->left) {
    node = &(*node)->left;
  }

  return node;
}

static node_t *node_rotate_right(node_t *y) {
  node_t *x  = y->left;
  node_t *z = x->right;

  x->right = y;
  y->left = z;

  y->height = max(node_height(y->left), node_height(y->right))+1;
  x->height = max(node_height(x->left), node_height(x->right))+1;

  return x;
}

static node_t *node_rotate_left(node_t *x) {
  node_t *y = x->right;
  node_t *z = y->left;

  y->left = x;
  x->right = z;

  x->height = max(node_height(x->left), node_height(x->right))+1;
  y->height = max(node_height(y->left), node_height(y->right))+1;

  return y;
}

static bool node_insert(node_t **node, T elem) {
  if (*node == NULL) {
    *node = node_new(elem);
    return true;
  }

  int cmp = compare(elem, (*node)->element);

  if (cmp < 0) {
    node_insert(&((*node)->left), elem);
  } else if (cmp > 0) {
    node_insert(&((*node)->right), elem);
  } else {
    // Same keys not allowed!
    return false;
  }

  int height_left  = node_height((*node)->left);
  int height_right = node_height((*node)->right);
  (*node)->height  = 1 + max(height_left, height_right);

  int balance = node_get_balance(*node);

  // Left-left
  if (balance > 1 && elem < (*node)->left->element) {
    *node = node_rotate_right(*node);
    return true;
  }
  // Right-right
  if (balance < -1 && elem > (*node)->right->element) {
    *node = node_rotate_left(*node);
    return true;
  }
  // Left-right
  if (balance > 1 && elem > (*node)->left->element) {
    (*node)->left = node_rotate_left((*node)->left);
    *node = node_rotate_right(*node);
    return true;
  }
  // Right-right
  if (balance < -1 && elem < (*node)->right->element) {
    (*node)->right = node_rotate_right((*node)->right);
    *node = node_rotate_left(*node);
    return true;
  }

  return true;
}

static bool node_remove(node_t **node, T elem) {
  if (*node == NULL) {
    return false;
  }

  if (elem < (*node)->element) {
    node_remove(&(*node)->left, elem);
  } else if (elem > (*node)->element) {
    node_remove(&(*node)->right, elem);
  } else {
    if ((*node)->left == NULL || (*node)->right == NULL) {
      node_t *temp = ((*node)->left != NULL) ? (*node)->left : (*node)->right;
      free(*node);

      if (temp == NULL) {
        *node = NULL;
      } else {
        *node = temp;
      }
    } else {
      node_t *temp = *node_minimum(&((*node)->right));
      (*node)->element = temp->element;
      node_remove(&((*node)->right), temp->element);
    }
  }

  if (*node == NULL) {
    return false;
  }

  (*node)->height = 1 + max(node_height((*node)->left), node_height((*node)->right));
  int balance = node_get_balance(*node);

  // Left-left
  if (balance > 1 && node_get_balance((*node)->left) >= 0) {
    *node = node_rotate_right(*node);
    return true;
  }

  // Left-right
  if (balance > 1 && node_get_balance((*node)->left) < 0) {
    (*node)->left =  node_rotate_left((*node)->left);
    *node = node_rotate_right(*node);
    return true;
  }

  // Right-right
  if (balance < -1 && node_get_balance((*node)->right) <= 0) {
    *node = node_rotate_left(*node);
    return true;
  }

  // Right-left
  if (balance < -1 && node_get_balance((*node)->right) > 0) {
    (*node)->right = node_rotate_right((*node)->right);
    *node = node_rotate_left(*node);
    return true;
  }

  return true;
}

static int node_height(node_t *node) {
  if (node == NULL) return 0;
  return node->height;
}

static int node_get_balance(node_t *node) {
  if (node == NULL) return 0;
  return node_height(node->left) - node_height(node->right);
}

static int node_size(node_t *node, int acc) {
  if (node) {
    return node_size(node->right, node_size(node->left, 1 + acc));
  }

  return acc;
}

static void node_delete(node_t *node, tree_free_func free_func, bool delete_elems) {
  if (node) {
    if (node->left) {
      node_delete(node->left, free_func, delete_elems);
    }

    if (node->right) {
      node_delete(node->right, free_func, delete_elems);
    }

    if (delete_elems) {
      free_func(node->element);
    }

    free(node);
  }
}

static void node_apply(node_t *node, tree_apply_func function, void *data) {
  if (node) {
    node_apply(node->left, function, data);
    function(node->element, data);
    node_apply(node->right, function, data);
  }
}
