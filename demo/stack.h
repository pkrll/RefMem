/**
 * @file
 * @brief Stack data structure
 */

#ifndef __stack_h__
#define __stack_h__

#include "common.h"

typedef struct stack stack_s;

typedef void(*stack_element_free_fun)(void *);

/**
 * @brief  Creates and returns a new, empty stack.
 *
 * @return A pointer to a new and empty stack.
 */
stack_s *stack_new(stack_element_free_fun free_fun);
/**
 * @brief  Pushes an element to the stack.
 *
 * @param  stack   The stack to push the element to.
 * @param  element The element to add to the stack.
 *
 * @return void
 */
void stack_push(stack_s *stack, void *element);
/**
 * @brief Remove an element from the stack and return the result.
 *
 * @note   If the stack parameter is NULL, this function will return
 *         an elem_t type with a NULL pointer.
 *
 * @see    ::stack_top
 *
 * @param  stack A pointer to a stack.
 *
 * @return The element that was removed from the stack.
 */
void *stack_pop(stack_s *stack);
/**
 * @brief  Returns the top element of the stack.
 *
 * @note   This will not remove the element. For that, use ::stack_pop.
 *
 * @param  stack A pointer to a stack.
 *
 * @return The element at the top of the stack.
 */
void *stack_top(stack_s *stack);
/**
 * @brief  Returns the size of a stack.
 *
 * @param  stack A pointer to a stack.
 *
 * @return The size of the stack.
 */
int stack_size(stack_s *stack);

#endif
