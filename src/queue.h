#include <stdbool.h>
#include <stdlib.h>

typedef struct queue queue_t;
typedef void(*queue_apply_func)(void *item, void *data);

/**
 * @brief           Creates a new queue.
 *
 * @return          New queue.
 */
queue_t *queue_create();
/**
 * @brief           Places an element at the end of the queue.
 *
 * @param queue     Pointer to the queue.
 * @param input     Object to enqueue.
 */
void queue_enqueue(queue_t *queue, void *input);
/**
 * @brief           Return the first object in the queue.
 *
 * @param queue     Pointer to queue.
 * @return          Content of first object.
 */
void *queue_first(queue_t *queue);
/**
 * @brief           Counts the number of elements in the queue.
 *
 * @param queue     Pointer to queue.
 * @return          The length of the current queue.
 */
size_t queue_length(queue_t *queue);
/**
 * @brief           Dequeues and returns the first object of the queue.
 *
 * @param queue     Pointer to queue.
 * @return          The dequeued object (or NULL if queue is empty).
 */
void *queue_dequeue(queue_t *queue);

/**
 * @brief           Checks if a queue is empty.
 *
 * @param queue     Pointer to queue.
 * @return          True if the queue is empty, otherwise false.
 */
bool queue_is_empty(queue_t *queue);

/**
 * @brief           Applies func on all objects of the queue.
 *
 * @param queue     Pointer to queue.
 * @param func      Pointer to function
 * @param data      Param for the function which applies on the queues object.
 */
void queue_apply(queue_t *queue, queue_apply_func func, void *data);
/**
 * @brief           Deletes the queue.
 *
 * @param queue     Pointer to a queue.
 */
void queue_delete(queue_t *queue);
