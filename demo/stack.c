#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

typedef struct stack_node stack_node_t;

// -------------------------------
// Structs & Enums
// -------------------------------

struct stack_node {
  void *element;
  stack_node_t *next;
};

struct stack {
  int size;
  stack_node_t *top;

  stack_element_free_fun free_fun;
};

// -------------------------------
// Declarations
// -------------------------------

static stack_node_t* stack_node_new();
static void stack_destructor(obj object);

// -------------------------------
// Public
// -------------------------------

stack_s *stack_new(stack_element_free_fun free_fun) {
  stack_s *stack = allocate(sizeof(stack_s), stack_destructor);

  if (stack) {
    stack->free_fun = free_fun;
    stack->size = 0;
  }

  return stack;
}

void stack_push(stack_s *stack, void *element) {
  stack_node_t* node_new = stack_node_new();

  if (stack && node_new) {
    node_new->element = element;
    node_new->next = stack->top;
    stack->top = node_new;

    stack->size += 1;

    retain(node_new);
    retain(element);
  }
}

void *stack_pop(stack_s *stack) {
  void *elem = NULL;

  if (stack && stack->top) {
    stack_node_t *node = stack->top;
    stack->top = stack->top->next;

    elem = node->element;

    release(node);
    release(elem);

    stack->size -= 1;
  }

  return elem;
}

void *stack_top(stack_s *stack) {
  if (stack && stack->top) return stack->top->element;

  return NULL;
}

int stack_size(stack_s *stack) {
  if (stack) {
    return stack->size;
  }

  return 0;
}

// -------------------------------
// Private
// -------------------------------

static stack_node_t* stack_node_new() {
  return allocate(sizeof(stack_node_t), NULL);
}

static void stack_destructor(obj object) {
  stack_s *stack = (stack_s *)object;

  stack_node_t *node = stack->top;

  while (node) {
    stack_node_t *prev = node;
    node = node->next;
    
    release(prev->element);
    release(prev);
  }
}
