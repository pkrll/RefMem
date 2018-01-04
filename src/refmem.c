#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "refmem.h"
#include "treeset.h"
#include "list.h"

/**
* @brief cascade_limit represent the amount of free's
         that's the program is allowed to do in a
         sequence.
*/
static size_t cascade_limit = 1000;

static list_t *destr_register = NULL;

static list_t *size_register = NULL;

static treeset_t *mem_register = NULL;

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

static void tree_free(obj input);

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
  record_t *record = calloc(1, sizeof(record_t) + bytes);

  record->reference_count = 0;

  element_t elem = {.f = destructor};
  if (destr_register == NULL) destr_register = list_new();

  element_t size = {.s = bytes};
  if (size_register == NULL) size_register = list_new();

  record->id = list_expand(destr_register, elem, compare_destructor);
  record->size_index = list_expand(size_register, size, compare_size);

  record++;

  if (mem_register == NULL) {
    mem_register = treeset_new(NULL);
  }

  treeset_insert(mem_register, (obj)record);

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
  record_t *record = calloc(1, ( allocated_size + sizeof(record_t)));

  record->reference_count = 0;

  element_t elem = {.f = destructor};
  if (destr_register == NULL) destr_register = list_new();

  element_t size = {.s = allocated_size};
  if (size_register == NULL) size_register = list_new();

  record->id = list_expand(destr_register, elem, compare_destructor);
  record->size_index = list_expand(size_register, size, compare_size);

  record++;

  if (mem_register == NULL) {
    mem_register = treeset_new(tree_free);
  }

  treeset_insert(mem_register, (obj)record);

  return (obj)record;
}

void deallocate(obj object) {

  assert(rc(object) == 0);

  record_t *record = convert_to_record(object);
  element_t elem = list_get(destr_register,record->id);
  function1_t destr = NULL;
  destr = elem.f;

  if (destr != NULL) {

    (*destr)(object);
  }

  record++;
  treeset_remove(mem_register, (obj)record);
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

static void tree_free(obj input) {
  record_t* record = convert_to_record(input);
  free(record);
}

static bool compare_destructor(element_t elem1, element_t elem2) {
  return elem1.f == elem2.f;
}

static bool compare_size(element_t elem1, element_t elem2) {
  return elem1.s == elem2.s;
}
