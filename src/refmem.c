#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "refmem.h"
#include "queue.h"

/**
* @brief cascade_limit represent the amount of free's
         that's the program is allowed to do in a
         sequence.
*/
static size_t cascade_limit = 1000;
static size_t cascade_counter = 0;
static queue_t *mem_register = NULL;

// -------------------------------
// Structs
// -------------------------------

struct record {
  size_t reference_count;
  function1_t destructor;

} typedef record_t;


// -------------------------------
// Declarations
// -------------------------------

/**
 * @brief                 Redirect object pointer to its record
 * @param object          Pointer to object
 * @return                The object's record
 */
static record_t *convert_to_record(obj object);
/**
 * @brief                 Redirect record pointer to its object
 * @param object          Pointer to record
 * @return                The record's object
 */
static obj convert_from_record(record_t *record);
/**
 * @brief         Puts an object in the mem register tree.
 * @param object  Pointer to the object.
 */
static void save_object(obj object);

/**
 * @brief         Free cascade_limit amount of objects in mem_register
 * @return        void
 */
static void clear_mem_register();

// -------------------------------
// Public
// -------------------------------

void retain(obj object) {
  if (object != NULL) {
    record_t *record = convert_to_record(object);
    record->reference_count += 1;
  }
}

void release(obj object) {
  if (object != NULL) {
    record_t *record = convert_to_record(object);

    record->reference_count -= 1;

    if (record->reference_count == 0) {
      obj object = convert_from_record(record);

      deallocate(object);
    }
  }
}

size_t rc(obj object) {
  // Should we assert object?
  if (object != NULL) {
    record_t *record = convert_to_record(object);
    return record->reference_count;
  }

  return 0;
}

obj allocate(size_t bytes, function1_t destructor) {
  record_t *record = calloc(1, sizeof(record_t) + bytes);

  record->reference_count = 0;
  record->destructor = destructor;

  clear_mem_register();

  record++;

  return (obj)record;
}

void set_cascade_limit(size_t limit) {
  cascade_limit = limit;
}

size_t get_cascade_limit() {
  return cascade_limit;
}

obj allocate_array(size_t elements, size_t elem_size, function1_t destructor) {
  record_t *record = calloc(1, ( elem_size * elements + sizeof(record_t)));

  record->reference_count = 0;
  record->destructor = destructor;

  clear_mem_register();

  record++;

  return (obj)record;
}

void deallocate(obj object) {

  assert(rc(object) == 0);

  record_t *record = convert_to_record(object);

  if (record->destructor != NULL) {
    (*record->destructor)(object);
  }

  if (cascade_counter >= cascade_limit) {
    save_object(record);
  } else {

    free(record);

    cascade_counter += 1;
  }
}

bool mem_register_is_empty() {
  return queue_is_empty(mem_register);
}

void cleanup();
void shutdown();

// -------------------------------
// Private
// -------------------------------

static record_t *convert_to_record(obj object) {
  record_t *record = object;
  record--;

  return record;
}

static obj convert_from_record(record_t *record) {
  record++;
  obj object = (obj) record;

  return object;
}

static void save_object(obj object) {
  if (mem_register == NULL) {
    mem_register = queue_create();
  }

  queue_enqueue(mem_register, object);
}

void clear_mem_register() {
  cascade_counter = 0;

  if (mem_register == NULL) {
    mem_register = queue_create();
  }

  for (size_t i = 0; i < cascade_limit; ++i) {
    free(queue_dequeue(mem_register));
  }
}
