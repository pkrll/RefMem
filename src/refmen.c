#include <stdlib.h>
#include <stdio.h>
#include "refmen.h"

#define COUNT_SIZE 8
#define DESTR_SIZE 8
#define HEADER_SIZE (COUNT_SIZE + DESTR_SIZE)

struct object {
  size_t refcount;
  function1_t destructor;

} typedef object_t;

void retain(obj object) {
  if (object != NULL) {
    object = (char *)object - HEADER_SIZE;

    ((object_t *)object)->refcount += 1;
  }
}

void release(obj object) {
  if (object != NULL) {
    object = (char *)object - HEADER_SIZE;

    if (((object_t *)object)->refcount > 1) {
      ((object_t *)object)->refcount -= 1;
    } else {
      // deallocate(object);
    }
  }
}

size_t rc(obj param) {
  param = (char *)param - HEADER_SIZE;
  object_t *object = (object_t*)param;

  return object->refcount;
}

obj allocate(size_t bytes, function1_t destructor) {
  void *object = malloc(HEADER_SIZE + bytes);

  ((object_t *)object)->refcount = 0;
  ((object_t *)object)->destructor = destructor;

  object = (char *)object + HEADER_SIZE;

  return (obj)object;
}


obj *allocate_array(size_t elements, size_t elem_size, function1_t destructor);

void deallocate(obj);
void set_cascade_limit(size_t);
size_t get_cascade_limit();
void cleanup();
void shutdown();
