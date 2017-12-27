#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "refmem.h"
#include "tree.h"

/**
* @brief cascade_limit represent the amount of free's
         that's the program is allowed to do in a
         sequence.
*/
static size_t cascade_limit = 1000;

tree_t *mem_register = NULL;

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

static void tree_free(obj input);

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
  record_t *record = malloc(sizeof(record_t) + bytes);

  record->reference_count = 0;
  record->destructor = destructor;

  record++;

  if (mem_register == NULL) 
    mem_register = tree_new(tree_free);

  tree_insert(mem_register, (obj)record);

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

  record++;

  if (mem_register == NULL) 
    mem_register = tree_new(tree_free);

  tree_insert(mem_register, (obj)record);
  
  return (obj)record;
}

void deallocate(obj object) {

  assert(rc(object) == 0);

  record_t *record = convert_to_record(object);

  if (record->destructor != NULL) {
    (*record->destructor)(object);
  }

  record++;
  tree_remove(mem_register, (obj)record);
}

void set_cascade_limit(size_t);
size_t get_cascade_limit();
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

static void tree_free(obj input) {
  record_t* record = convert_to_record(input);
  free(record);
}
