#include <stdlib.h>
#include "queue.h"
#include <stdio.h>

// -------------------------------
// Structs
// -------------------------------

struct item {
  void *content;
  struct item* next;
} typedef item_t;

struct queue {
  item_t* first;
  item_t* last;
} typedef queue_t;

// -------------------------------
// Public
// -------------------------------

queue_t *create_queue() {
  queue_t *queue = malloc(sizeof(queue_t));
  *queue = (queue_t) {
    .first = NULL,
     .last = NULL
  };

  return queue;

}

void enqueue(queue_t *queue, void *input) {
  item_t *item = malloc(sizeof(item));
  item->content = input;
  item->next = NULL;

  if (queue->first == NULL) {
    queue->first = item;
    queue->last = item;
  } else {
    queue->last->next = item;
    queue->last = item;
  }
}

void *dequeue(queue_t *queue) {
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

void *peak_next(queue_t *queue) {
  return queue->first->content;
}

bool is_empty(queue_t *input) {
  return (input->first == NULL) ? true : false;
}

void apply(queue_t *queue, queue_apply_func func, void *data) {
  item_t *cursor = queue->first;

  while (cursor) {
    func(cursor->content, data);
    cursor = cursor->next;
  }

}

