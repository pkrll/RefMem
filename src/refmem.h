#pragma once
#include <stddef.h>
#include <stdbool.h>

typedef void *obj;
typedef void(*function1_t)(obj);

/**
 * @brief               Increase an object reference count by one.
 *
 * @note                The object inputed may not be NULL.
 * @param object        Pointer to which object count to increase
 */
void retain(obj object);

/**
 * @brief               Decrease an object reference count by one.
 *
 * @param object        Pointer to which object count to decrease
 * @return              void
 */
void release(obj object);

/**
 * @brief               Returns an object's reference count.
 *
 * @note                The object inputed may not be NULL.
 * @param object        Pointer to which object to extract count from.
 * @return              number of references to the object.
 */
unsigned short rc(obj object);
/**
 * @brief               Allocates memory for an object.
 *
 * @param bytes         Number of bytes to allocate.
 * @param destructor    Pointer to destructor function (may be NULL).
 * @return              Pointer to allocated object.
 */
obj allocate(size_t bytes, function1_t destructor);
/**
 * @brief               Allocates memory for a set number of objects.
 *
 * @param elements      Number of elements to allocate.
 * @param elem_size     Size of the object to allocate, in byte.
 * @param destructor    Pointer to destructor function (may be NULL).
 * @return              Pointer to allocated object
 */
obj allocate_array(size_t elements, size_t elem_size, function1_t destructor);
/**
 * @brief               Free allocated object and its header.
 *
 * @param               Pointer to which object to free.
 */
void deallocate(obj object);
/**
 * @brief                Sets new cascade limit.
 * @param                limit new cascde value.
 */
void set_cascade_limit(size_t limit);
/**
 * @brief                Returns the cascade limit.
 * @return               cascade limit
 */
size_t get_cascade_limit();
/**
  * @brief               Removes all garbage objects still not deallocated.
  */
void cleanup();
/**
  * @brief               Shutdowns the reference count system.
  */
void shutdown();
