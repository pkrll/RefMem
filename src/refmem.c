#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "refmem.h"
#include "queue.h"
#include "listset.h"

/**
* @brief cascade_limit represent the amount of free's
         that's the program is allowed to do in a
         sequence.
*/
static size_t cascade_limit = 1000;

static listset_t *destr_register = NULL;

static listset_t *size_register = NULL;

static queue_t *obj_register = NULL;

static size_t cascade_counter = 0;

// -------------------------------
// Structs
// -------------------------------

struct header {
  unsigned short reference_count;
  unsigned short destr_index;
  unsigned short size_index;
} typedef header_t;


// -------------------------------
// Declarations
// -------------------------------

/**
 * @brief                 Redirect object pointer to its header
 * @param object          Pointer to object
 * @return                The object's header
 */
static header_t *convert_to_header(obj object);
/**
 * @brief         Puts an object in the object register queue.
 * @param object  Pointer to the object.
 */
static void queue_deallocation(obj object);
/**
 * @brief         Prepares the system for allocation.
 * @param bytes   The number of bytes requested for allocation.
 */
static void prepare_for_allocation(size_t bytes);
/**
 * @brief         Free cascade_limit amount of objects in obj_register or clear until limit is reached
 * @param request Upper limit of number of bytes to clear.
 * @return        void
 */
static void clear_obj_register(size_t request);

/**
 * @brief                 Compare two elements by their destructor function
 * @return                true if they are the same, false if not
 */
static bool compare_destructor(element_t elem1, element_t elem2);

/**
 * @brief                 Compare two elements by their size
 * @return                true if they are the same, false if not
 */
static bool compare_size(element_t elem1, element_t elem2);

// -------------------------------
// Public
// -------------------------------

void retain(obj object) {
  assert(object != NULL);

  if (object != NULL) {
    header_t *header = convert_to_header(object);
    header->reference_count += 1;
  }
}

void release(obj object) {
  if (object != NULL) {
    header_t *header = convert_to_header(object);

    header->reference_count -= 1;

    if (header->reference_count == 0) {
      deallocate(object);
    }
  }
}

unsigned short rc(obj object) {
  assert(object != NULL);

  header_t *header = convert_to_header(object);
  return header->reference_count;
}

obj allocate(size_t bytes, function1_t destructor) {
  prepare_for_allocation(bytes);

  element_t elem = {.f = destructor};
  element_t size = {.s = bytes};

  header_t *header = calloc(1, sizeof(header_t) + bytes);

  if (header) {
    header->reference_count = 0;
    header->destr_index = listset_expand(destr_register, elem, compare_destructor);
    header->size_index  = listset_expand(size_register, size, compare_size);

    header++;
  }

  return (obj)header;
}

void set_cascade_limit(size_t limit) {
  cascade_limit = limit;
}

size_t get_cascade_limit() {
  return cascade_limit;
}

obj allocate_array(size_t elements, size_t elem_size, function1_t destructor) {
  size_t allocated_size = elem_size * elements;
  prepare_for_allocation(allocated_size);

  element_t elem = {.f = destructor};
  element_t size = {.s = allocated_size};

  header_t *header = calloc(1, ( allocated_size + sizeof(header_t)));

  if (header) {
    header->reference_count = 0;
    header->destr_index = listset_expand(destr_register, elem, compare_destructor);
    header->size_index = listset_expand(size_register, size, compare_size);

    header++;
  }

  return (obj)header;
}

void deallocate(obj object) {
  assert(rc(object) == 0);

  header_t *header = convert_to_header(object);
  element_t elem   = listset_get(destr_register,header->destr_index);

  if (elem.f != NULL) {
    function1_t destructor = elem.f;
    (*destructor)(object);
  }

  if (cascade_counter >= cascade_limit) {
    queue_deallocation(header);
  } else {
    free(header);
    cascade_counter += 1;
  }
}

bool mem_register_is_empty() {
  return queue_is_empty(obj_register);
}

void cleanup() {
  if (obj_register != NULL) {
    while (mem_register_is_empty() == false) {
      header_t *object = queue_dequeue(obj_register);
      free(object);
    }
  }
}

void shutdown() {
  cleanup();
  queue_delete(obj_register);
  listset_delete(destr_register);
  listset_delete(size_register);
}

// -------------------------------
// Private
// -------------------------------

static header_t *convert_to_header(obj object) {
  header_t *header = object;
  header--;

  return header;
}

static void queue_deallocation(obj object) {
  if (obj_register == NULL) {
    obj_register = queue_create();
  }

  queue_enqueue(obj_register, object);
}

static size_t free_header(header_t *header) {
  element_t size = listset_get(size_register, header->size_index);
  free(header);
  return size.s;
}

static void prepare_for_allocation(size_t bytes) {
  if (destr_register == NULL) destr_register = listset_new();
  if (size_register == NULL)  size_register  = listset_new();

  clear_obj_register(bytes);
}

static void clear_obj_register(size_t request) {
  cascade_counter = 0;

  if (obj_register == NULL) {
    obj_register = queue_create();
  } else {
    size_t size_sum = 0;

    for (unsigned short i = 0; i < cascade_limit && queue_is_empty(obj_register) == false; i++) {
      size_sum += free_header(queue_dequeue(obj_register));
    }

    while (size_sum < request && queue_is_empty(obj_register) == false) {
      size_sum += free_header(queue_dequeue(obj_register));
    }
  }
}

static bool compare_destructor(element_t elem1, element_t elem2) {
  return elem1.f == elem2.f;
}

static bool compare_size(element_t elem1, element_t elem2) {
  return elem1.s == elem2.s;
}
