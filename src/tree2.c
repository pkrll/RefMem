#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "tree.h"


typedef struct node node_t;

struct node {
  tree_key_t  key;
  elem_t      content;
  int         height;
  node_t     *left;
  node_t     *right;
};

struct tree {
  node_t *root;
  int size;
  element_copy_fun  copy;
  key_free_fun      freeKey;
  element_free_fun  freeCont;
  element_comp_fun  compare;
};

//==================================== HJÄLPFUNKTIONER =========================================//

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


bool tree_insert_aux (node_t **subtree, element_comp_fun compare, tree_key_t key, elem_t elem){
  if (*subtree == NULL) {
    *subtree = calloc(1, sizeof(node_t));
    (*subtree)->key = key;
    (*subtree)->content = elem;
    (*subtree)->height = 1;
    (*subtree)->left = NULL;
    (*subtree)->right = NULL;
    return true;
  }
  else if (compare((*subtree)->key, key) == 1){
    bool returnvalue = tree_insert_aux(&(*subtree)->left, compare, key, elem);
    fix_height(*subtree);
    rebalance_node(subtree); 
    fix_height(*subtree);  
    return returnvalue;
  }
  else if (compare((*subtree)->key, key) == -1){
    bool returnvalue = tree_insert_aux(&(*subtree)->right, compare, key, elem);
    fix_height(*subtree);
    rebalance_node(subtree); 
    fix_height(*subtree);
    return returnvalue;
  }
  else{
    return false;
  }
}

void tree_delete_aux(node_t *node, key_free_fun key_free, element_free_fun elem_free, bool delete_keys, bool delete_elements){
  if (node) {
    tree_delete_aux(node->left, key_free, elem_free, delete_keys, delete_elements);
    tree_delete_aux(node->right, key_free, elem_free, delete_keys, delete_elements);
    if (delete_keys) {key_free(node->key);}
    if (delete_elements) {elem_free(node->content);}
    free(node);
  }
}

//================================ HUVUDFUNKTIONER ========================================

tree_t *tree_new(element_copy_fun element_copy, key_free_fun key_free, element_free_fun elem_free, element_comp_fun compare){
  tree_t *newtree = calloc(1, sizeof(tree_t));
  if (newtree){
    newtree->root = NULL;
    newtree->size = 0;
    newtree->compare = compare;
    newtree->copy = element_copy;
    newtree->freeCont = elem_free;
    newtree->freeKey = key_free;
  }
  return newtree;
}

bool tree_insert(tree_t *tree, tree_key_t key, elem_t elem){
  bool returnvalue = tree_insert_aux(&tree->root, tree->compare, key, elem);
  if (returnvalue) {++ (tree->size);}
  return returnvalue;    
}

void tree_delete(tree_t *tree, bool delete_keys, bool delete_elements){
  tree_delete_aux(tree->root, tree->freeKey, tree->freeCont, delete_keys, delete_elements);
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

bool tree_has_key(tree_t *tree, tree_key_t key){
  node_t *cursor = tree->root;
  while (cursor) {
    switch (tree->compare(cursor->key,key )) {
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

elem_t *tree_get(tree_t *tree, tree_key_t key){
  node_t *cursor = tree->root;
  while (cursor) {
    switch (tree->compare(cursor->key,key )) {
    case 1:
      cursor = cursor->left;
      break;      
    case 0:
      return &(cursor->content);       
      break;
    case -1:
      cursor = cursor->right;
      break;      
    }
  }
  return NULL;
}

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

node_t *tree_remove_aux(node_t **node, tree_key_t key, element_comp_fun compare){

  if(!node){
    return NULL;
  }  
  else if (compare((*node)->key, key) == 1){
    node_t *returnvalue = tree_remove_aux( &(*node)->left, key, compare);
    rebalance_node(node);
    fix_height(*node);
    return returnvalue;
  }
  else if (compare((*node)->key, key) == -1){
    node_t *returnvalue = tree_remove_aux(&(*node)->right, key, compare);
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

bool tree_remove(tree_t *tree, tree_key_t key, elem_t *result){
  node_t *node = tree_remove_aux(&(tree->root), key, tree->compare);

  if (node) {
    if(result){
      *result = node->content;
    }
    else {
      tree->freeCont(node->content);
    }
    tree->freeKey(node->key);
    free(node);
    -- tree->size;
    return true;
  }
  
  return false;
  
}

bool node_apply(node_t *node, enum tree_order order, key_elem_apply_fun fun, void *data){
  bool returnvalue=false;
  if (node) {    
    switch (order) {
    case inorder: {
      returnvalue = node_apply(node->left, order, fun, data) || returnvalue;
      returnvalue = fun(node->key, node->content, data) || returnvalue;
      returnvalue = node_apply(node->right, order, fun, data) || returnvalue;
      break;
    }
    case preorder: {
      returnvalue = fun(node->key, node->content, data) || returnvalue;
      returnvalue = node_apply(node->left, order, fun, data) || returnvalue;
      returnvalue = node_apply(node->right, order, fun, data) || returnvalue;
      break;
    }
    case postorder: {
      returnvalue = node_apply(node->left, order, fun, data) || returnvalue;
      returnvalue = node_apply(node->right, order, fun, data) || returnvalue;
      returnvalue = fun(node->key, node->content, data) || returnvalue;
      break;
    }
    }
  }
  return returnvalue;

}

bool tree_apply(tree_t *tree, enum tree_order order, key_elem_apply_fun fun, void *data){
  return node_apply(tree->root, order, fun, data);
}

bool key_insert(tree_key_t key, elem_t elem, tree_key_t** data){  
  **data = key;
  (*data) ++;
  return true;
}

bool elem_insert(tree_key_t key, elem_t elem, elem_t **data){
  **data = elem;
  (*data) ++;
  return true;
  
}

tree_key_t *tree_keys(tree_t *tree){
  tree_key_t *key_array = calloc(tree->size+1, sizeof(tree_key_t));  
  tree_key_t *array_start= key_array;
  tree_apply(tree, inorder, (key_elem_apply_fun) key_insert, &key_array);
  return array_start;
}

elem_t *tree_elements(tree_t *tree){
  elem_t *elem_array = calloc(tree->size+1, sizeof(tree_key_t));
  elem_t *array_start = elem_array;
  tree_apply(tree, inorder, (key_elem_apply_fun)elem_insert, &elem_array);
  return array_start;
}
