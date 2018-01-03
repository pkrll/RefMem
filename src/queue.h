#include <stdbool.h>

typedef struct queue queue_t;
typedef void(*queue_apply_func)(void *item, void *data);
/**
 * @brief           Creates a new queue.
 * @return          New queue
 */
queue_t *create_queue();
/**
 * @brief           Place inout at the end of queue, if
 *                  queue is empty place input first.
 * @param queue     Pointer to queue
 * @param input     Object to enqueue
 * @return          void
 */
void enqueue(queue_t *queue, void *input);
/**
 * @brief           Return the content of the first object
 * @param queue     Pointer to queue
 * @return          Content of first object
 */
void *peak_next(queue_t *queue);
/**
 * @brief           Removes first object of the queue and return
 *                  its content. If queue is empty return NULL.
 * @param queue     Pointer to queue
 * @return          Content of removed object
 */
void *dequeue(queue_t *queue);
/**
 * @brief           Checks if a queue is empty.
 * @param queue     Pointer to queue
 * @return          true if queue is empty, else false
 */
bool is_empty(queue_t *queue);
/**
 * @brief           Applies func on all objects of the queue.
 * @param queue     Pointer to queue
 * @param func      Pointer to function
 * @param data      Param for the function which applies on the queues object
 * @return          void
 */
void apply(queue_t *queue, queue_apply_func func, void *data);
