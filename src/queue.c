#include <stdlib.h>
#include "queue.h"
#include <stdio.h>

// -------------------------------
// Structs
// -------------------------------

typedef struct item item_t;

struct item {
  void *content;
  item_t *next;
};

struct queue {
  item_t* first;
  item_t* last;
};

// -------------------------------
// Public
// -------------------------------

queue_t *queue_create() {
  queue_t *queue = malloc(sizeof(queue_t));
  *queue = (queue_t) {
    .first = NULL,
     .last = NULL
  };

  return queue;
}

void queue_enqueue(queue_t *queue, void *input) {
  item_t *item = calloc(1, sizeof(item_t));
  item->content = input;

  if (queue->first == NULL) {
    queue->first = item;
    queue->last = item;
  } else {
    queue->last->next = item;
    queue->last = item;
  }
}

void *queue_dequeue(queue_t *queue) {
  item_t *next_item;

  if (queue->first != NULL) {
    next_item = queue->first;

    if (queue->first->next != NULL) {
      queue->first = queue->first->next;
    } else {
      queue->first = NULL;
      queue->last = NULL;
    }

    void *content = next_item->content;
    free(next_item);

    return content;
  }

  return NULL;
}

size_t queue_length(queue_t *queue) {
  size_t counter = 0;
  item_t *cursor = queue->first;

  while (cursor) {
    cursor = cursor->next;
    counter++;
  }

  return counter;
}

void *queue_first(queue_t *queue) {
  return queue->first->content;
}

bool queue_is_empty(queue_t *input) {
  return (input->first == NULL) ? true : false;
}

void queue_apply(queue_t *queue, queue_apply_func func, void *data) {
  item_t *cursor = queue->first;

  while (cursor) {
    func(cursor->content, data);
    cursor = cursor->next;
  }
}

static void delete_aux(item_t *content) {
  if (content->next) {
    delete_aux(content->next);
  }

  free(content);
}

void queue_delete(queue_t *queue) {
  if (queue->first) {
    delete_aux(queue->first);
  }

  free(queue);
}
