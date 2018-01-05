#pragma once
#include <stddef.h>
#include <stdbool.h>

typedef void *obj;
typedef void(*function1_t)(obj);

/**
 * @brief               Increase an object reference count
 *                      by one. If param is NULL make an
 *                      assertion.
 * @param object        Pointer to which object count to increase
 * @return              void
 */
void retain(obj object);

/**
 * @brief               Decrease an object reference count
 *                      by one.
 * @param object        Pointer to which object count to decrease
 * @return              void
 */
void release(obj object);

/**
 * @brief               Returns object refrence count, the
 *                      number of objects that depends of the
 *                      param.
 * @param object        Pointer to which object to extract count from
 * @return              number of reference
 */
unsigned short rc(obj object);
/**
 * @brief               Allocates memory for an object an its record
 *                      that contains the reference counter and the
 *                      function pointer to its destructor.
 * @param bytes         Number of bytes to allocate
 * @param destructor    (may be NULL) Pointer to destructor function
 * @return              Pointer to allocated object
 */
obj allocate(size_t bytes, function1_t destructor);

/**
 * @brief               Allocates memory for a set number of object,
 *                      an its record that contains the reference
 *                      counter and the function pointer to its destructor.
 * @param elements      Number of elements to allocate
 * @param elem_size     Size of the object to allocate, in byte
 * @param destructor    (may be NULL) Pointer to destructor function
 * @return              Pointer to allocated object
 */
obj allocate_array(size_t elements, size_t elem_size, function1_t destructor);

/**
 * @brief               Free allocated object and its header.
 * @param               Pointer to which object to free
 * @return              void
 */
void deallocate(obj object);

/**
* @brief                Sets new cascade limit.
* @param                limit new cascde value
* @return               void
*/
void set_cascade_limit(size_t limit);

/**
* @brief                Returns the cascade limit.
* @return               cascade limit
*/
size_t get_cascade_limit();

/**
 * brief
 * @return              void
 */
void cleanup();

/**
 * @brief
 * @return              void
 */
void shutdown();
/**
 * @brief checks if mem register is empty.
 * @return true if empty, else false
 */
bool mem_register_is_empty();
