#include <stdbool.h>

typedef struct queue queue_t;
typedef void(*queue_apply_func)(void *item, void *data);

queue_t *create_queue();

void enqueue(queue_t *queue, void *input);

void *peak_next(queue_t *queue);

void *dequeue(queue_t *queue);

bool is_empty(queue_t *queue);

void apply(queue_t *queue, queue_apply_func func, void *data);
