#include <stdbool.h>

typedef struct queue queue_t;
typedef void(*queue_apply_func)(void *item, void *data);

queue_t *queue_create();

void queue_enqueue(queue_t *queue, void *input);

void *queue_first(queue_t *queue);

void *queue_dequeue(queue_t *queue);

bool queue_is_empty(queue_t *queue);

void queue_apply(queue_t *queue, queue_apply_func func, void *data);

void queue_delete(queue_t *queue);
