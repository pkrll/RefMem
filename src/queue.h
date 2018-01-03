#include <stdbool.h>
#include <stdlib.h>

typedef struct queue queue_t;
typedef void(*queue_apply_func)(void *item, void *data);

/**
 * @brief           Creates a new queue.
 * @return          New queue
 */
queue_t *queue_create();

/**
 * @brief           Place inout at the end of queue, if
 *                  queue is empty place input first.
 * @param queue     Pointer to queue
 * @param input     Object to enqueue
 * @return          void
 */
void queue_enqueue(queue_t *queue, void *input);

/**
 * @brief           Return the content of the first object
 * @param queue     Pointer to queue
 * @return          Content of first object
 */
void *queue_first(queue_t *queue);

/**
 * @brief           Counts the number of elements in the queue
 * @param queue     Pointer to queue
 * @return          The length of the current queue
 */
size_t queue_length(queue_t *queue);

/**
 * @brief           Removes first object of the queue and return
 *                  its content. If queue is empty return NULL.
 * @param queue     Pointer to queue
 * @return          Content of removed object
 */
void *queue_dequeue(queue_t *queue);

/**
 * @brief           Checks if a queue is empty.
 * @param queue     Pointer to queue
 * @return          true if queue is empty, else false
 */
bool queue_is_empty(queue_t *queue);

/**
 * @brief           Applies func on all objects of the queue.
 * @param queue     Pointer to queue
 * @param func      Pointer to function
 * @param data      Param for the function which applies on the queues object
 * @return          void
 */
void queue_apply(queue_t *queue, queue_apply_func func, void *data);

void queue_delete(queue_t *queue);
