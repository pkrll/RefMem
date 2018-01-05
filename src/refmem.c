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

static queue_t *mem_register = NULL;

static size_t cascade_counter = 0;

// -------------------------------
// Structs
// -------------------------------

struct record {
  unsigned short reference_count;
  unsigned short id;
  unsigned short size_index;

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
 * @brief         Free cascade_limit amount of objects in mem_register or clear until limit is reached
 * @param request Upper limit of number of bytes to clear.
 * @return        void
 */
static void clear_mem_register(size_t request);

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

unsigned short rc(obj object) {
  assert(object != NULL);

  record_t *record = convert_to_record(object);
  return record->reference_count;
}

obj allocate(size_t bytes, function1_t destructor) {

  clear_mem_register(bytes);

  record_t *record = calloc(1, sizeof(record_t) + bytes);

  record->reference_count = 0;

  element_t elem = {.f = destructor};
  if (destr_register == NULL) destr_register = listset_new();

  element_t size = {.s = bytes};
  if (size_register == NULL) size_register = listset_new();

  record->id = listset_expand(destr_register, elem, compare_destructor);
  record->size_index = listset_expand(size_register, size, compare_size);

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
  size_t allocated_size = elem_size * elements;
  clear_mem_register(allocated_size);

  record_t *record = calloc(1, ( allocated_size + sizeof(record_t)));

  record->reference_count = 0;

  element_t elem = {.f = destructor};
  if (destr_register == NULL) destr_register = listset_new();

  element_t size = {.s = allocated_size};
  if (size_register == NULL) size_register = listset_new();

  record->id = listset_expand(destr_register, elem, compare_destructor);
  record->size_index = listset_expand(size_register, size, compare_size);

  record++;

  return (obj)record;
}

void deallocate(obj object) {

  assert(rc(object) == 0);

  record_t *record = convert_to_record(object);
  element_t elem = listset_get(destr_register,record->id);
  function1_t destr = NULL;
  destr = elem.f;

  if (destr != NULL) {
    (*destr)(object);
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

void cleanup() {
  if (mem_register != NULL) {
    while (mem_register_is_empty() == false) {
      record_t *object = queue_dequeue(mem_register);
      free(object);
    }
  }
}

void shutdown() {
  cleanup();
  queue_delete(mem_register);
  listset_delete(destr_register);
  listset_delete(size_register);
}

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

size_t free_record(record_t *record) {
  element_t size = listset_get(size_register, record->size_index);
  free(record);
  return size.s;
}

void clear_mem_register(size_t request) {
  cascade_counter = 0;

  if (mem_register == NULL) {
    mem_register = queue_create();
  } else {

    size_t size_sum = 0;

    for (unsigned short i = 0; i < cascade_limit && queue_is_empty(mem_register) == false; i++) {
      size_sum += free_record(queue_dequeue(mem_register));
    }

    while (size_sum < request && queue_is_empty(mem_register) == false) {
      size_sum += free_record(queue_dequeue(mem_register));
    }
  }
}

static bool compare_destructor(element_t elem1, element_t elem2) {
  return elem1.f == elem2.f;
}

static bool compare_size(element_t elem1, element_t elem2) {
  return elem1.s == elem2.s;
}
