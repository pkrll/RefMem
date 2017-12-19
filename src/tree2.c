#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "tree.h"


typedef struct node node_t;

struct node {
  T element;
  int height;

  struct node *left;
  struct node *right;

} typedef node_t;

struct tree {
  node_t *root;
  int size;
  tree_free_func free;
} typedef tree_t;

//==================================== HJÄLPFUNKTIONER =========================================//


int tree_height(tree_t *tree) {
  return 0;
}

int compare(T a, T b) {
  if (a < b) {
    return -1;
  } else if (a > b) {
    return 1;
  }

  return 0;

}

int max(int x, int y){
  return (x > y)? x : y;
}

void fix_height(node_t *node){
  if(!node){
    return;
  }
  int left =0;
  int right =0;
  if(node->left){
    left = node->left->height;
  }
  if(node->right){
    right = node->right->height;
  }
  node->height = max(left, right) + 1;
}

int node_depth(node_t *node) {
  if (!node){
    return 0;
  }
  else{
    return node->height;
  }
}

void rotate_right(node_t **root){
  node_t* tmp = (*root)->left;
  (*root)->left = tmp->right;
  tmp->right = *root;
  *root = tmp;
}

void rotate_left(node_t **root){
  node_t *tmp = (*root)->right;
  (*root)->right = tmp->left;
  tmp->left = *root;
  *root = tmp;
}

void rebalance_node(node_t **root){
  int right = node_depth((*root)->right);
  int left = node_depth((*root)->left);
  int rightright = -1;
  int rightleft = -1;
  int leftright = -1;
  int leftleft = -1;

  if(right){
    rightright = node_depth((*root)->right->right);
    rightleft = node_depth((*root)->right->left);
  }
  if(left){
    leftright = node_depth((*root)->left->right);
    leftleft = node_depth((*root)->left->left);
  }

  int balance = right-left;


  if (balance > 1) {
    if (rightright >= rightleft) {
      rotate_left(root);
    }
    else {
      rotate_right(&(*root)->right);
      rotate_left(root);

    }
    fix_height((*root)->left);
  }

  else if (balance < -1) {
    if (leftleft >= leftright) {
      rotate_right(root);
    }
    else {
      rotate_left(&(*root)->left);
      rotate_right(root);
    }
    fix_height((*root)->right);
  }
  else {
    return;
  }

}


bool tree_insert_aux (node_t **subtree, T elem){
  if (*subtree == NULL) {
    *subtree = calloc(1, sizeof(node_t));
    (*subtree)->element = elem;
    (*subtree)->height = 1;
    (*subtree)->left = NULL;
    (*subtree)->right = NULL;
    return true;
  }
  else if (compare((*subtree)->element, elem) == 1){
    bool returnvalue = tree_insert_aux(&(*subtree)->left, elem);
    fix_height(*subtree);
    rebalance_node(subtree);
    fix_height(*subtree);
    return returnvalue;
  }
  else if (compare((*subtree)->element, elem) == -1){
    bool returnvalue = tree_insert_aux(&(*subtree)->right, elem);
    fix_height(*subtree);
    rebalance_node(subtree);
    fix_height(*subtree);
    return returnvalue;
  }
  else{
    return false;
  }
}

void tree_delete_aux(node_t *node, tree_free_func elem_free, bool delete_elements){
  if (node) {
    tree_delete_aux(node->left, elem_free, delete_elements);
    tree_delete_aux(node->right, elem_free, delete_elements);
    if (delete_elements) {elem_free(node->element);}
    free(node);
  }
}

//================================ HUVUDFUNKTIONER ========================================

tree_t *tree_new(tree_free_func free_func){
  tree_t *newtree = calloc(1, sizeof(tree_t));
  if (newtree){
    newtree->root = NULL;
    newtree->size = 0;
    newtree->free = free_func;
  }
  return newtree;
}

bool tree_insert(tree_t *tree, T elem){
  bool returnvalue = tree_insert_aux(&tree->root, elem);
  if (returnvalue) {++ (tree->size);}
  return returnvalue;
}

void tree_delete(tree_t *tree, bool delete_elements){
  tree_delete_aux(tree->root, tree->free, delete_elements);
  free(tree);
}

int tree_size(tree_t *tree){
  return tree->size;
}

int tree_depth(tree_t *tree){
  if(tree->root){
    return tree->root->height;
  }
  else {
    return 0;
  }
}

bool tree_has_key(tree_t *tree, T elem){
  node_t *cursor = tree->root;
  while (cursor) {
    switch (compare(cursor->element, elem)) {
    case 1:
      cursor = cursor->left;
      break;
    case 0:
      return true;
      break;
    case -1:
      cursor = cursor->right;
      break;
    default :
      assert(false);

    }
  }
  return false;
}

// elem_t *tree_get(tree_t *tree, T elem){
//   node_t *cursor = tree->root;
//   while (cursor) {
//     switch (tree->compare(cursor->key,key )) {
//     case 1:
//       cursor = cursor->left;
//       break;
//     case 0:
//       return &(cursor->content);
//       break;
//     case -1:
//       cursor = cursor->right;
//       break;
//     }
//   }
//   return NULL;
// }

//har sideeffects
node_t **least_node(node_t **node){
  if((*node)->left){
    node_t **returnvalue = least_node(& ((*node)->left) );
    fix_height(*node);
    return returnvalue;
  }
  else{
    -- ((*node) -> height);
    return node;
  }
}

void bst_remove(node_t **node){
  if((! (*node)->left) && (! (*node)->right)){
    *node = NULL;
  }
  else if(( (*node)->left) && (! (*node)->right)){
    *node = (*node)->left;
  }
  else if((! (*node)->left) && ( (*node)->right)){
    (*node) = (*node)->right;
  }
  else if( (*node)->left && (*node)->right){
    node_t **least = least_node(&((*node)->right));
    node_t *least_value = * least;
    bst_remove (least);
    least_value ->right =(*node)->right;
    least_value ->left = (*node)->left;
    fix_height(least_value);
    *node = least_value;

  }

}

node_t *tree_remove_aux(node_t **node, T elem){

  if(!node){
    return NULL;
  }
  else if (compare((*node)->element, elem) == 1){
    node_t *returnvalue = tree_remove_aux( &(*node)->left, elem);
    rebalance_node(node);
    fix_height(*node);
    return returnvalue;
  }
  else if (compare((*node)->element, elem) == -1){
    node_t *returnvalue = tree_remove_aux(&(*node)->right, elem);
    rebalance_node(node);
    fix_height(*node);
    return returnvalue;
  }
  else {
    node_t * returnvalue = *node;
    bst_remove(node);
    return returnvalue;
  }
}

bool tree_remove(tree_t *tree, T elem){
  node_t *node = tree_remove_aux(&(tree->root), elem);

  if (node) {
    tree->free(node->element);
    free(node);
    -- tree->size;
    return true;
  }

  return false;

}

bool node_apply(node_t *node, tree_apply_func func, void *data){

  if (node) {
    node_apply(node->left, func, data);
    func(node->element, data);
    node_apply(node->right, func, data);
  }
    return true;
}

void tree_apply(tree_t *tree, tree_apply_func func, void *data){
  node_apply(tree->root, func, data);
}

// bool key_insert(tree_key_t key, elem_t elem, tree_key_t** data){
//   **data = key;
//   (*data) ++;
//   return true;
// }

bool elem_insert(T elem, T **data){
  **data = elem;
  (*data) ++;
  return true;

}

// tree_key_t *tree_keys(tree_t *tree){
//   tree_key_t *key_array = calloc(tree->size+1, sizeof(tree_key_t));
//   tree_key_t *array_start= key_array;
//   tree_apply(tree, inorder, (key_elem_apply_fun) key_insert, &key_array);
//   return array_start;
// }
//
// elem_t *tree_elements(tree_t *tree){
//   elem_t *elem_array = calloc(tree->size+1, sizeof(tree_key_t));
//   elem_t *array_start = elem_array;
//   tree_apply(tree, inorder, (key_elem_apply_fun)elem_insert, &elem_array);
//   return array_start;
// }
