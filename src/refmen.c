#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "refmen.h"

#define COUNT_SIZE 8
#define DESTR_SIZE 8
#define HEADER_SIZE (COUNT_SIZE + DESTR_SIZE)

/**  
* @brief CASCADE_LIMIT represent the amount of free's
         that's the program is allowed to do in a 
         sequence.
*/
static size_t CASCADE_LIMIT = 1000;

struct record {
  size_t reference_count;
  function1_t destructor;

} typedef record_t;

static record_t *convert_to_record(obj object) {
  record_t *record = object;
  record--;

  return record;
}

void retain(obj object) {
  if (object != NULL) {
    record_t *record = convert_to_record(object);
    record->reference_count += 1;
  }
}

void release(obj object) {
  if (object != NULL) {
    record_t *record = convert_to_record(object);

    if (record->reference_count > 1) {
      record->reference_count -= 1;
    } else {
      // deallocate(record)
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
  record_t *record = malloc(HEADER_SIZE + bytes);

  record->reference_count = 0;
  record->destructor = destructor;

  record++;

  return (obj)record;
}

/**  
* @brief Sets the cascade limit
* @param limit new cascade limit
* @return void.
*/
void set_cascade_limit(size_t limit) {
  CASCADE_LIMIT = limit;
}

/**  
* @brief Returns the cascade limit
* @return cascade limit.
*/
size_t get_cascade_limit(){
  return CASCADE_LIMIT;
}

obj allocate_array(size_t elements, size_t elem_size, function1_t destructor) {
      record_t *record = calloc(1, ( elem_size * elements + HEADER_SIZE));
      

      record->reference_count = 0;
      record->destructor = destructor;

      record++;

      return (obj)record;
}

void deallocate(obj object) {

  assert(rc(object) == 0);
  
  record_t *record = convert_to_record(object);

  if (record->destructor != NULL) {
    (*record->destructor)(object);
  }
    
  free(record);
}

void set_cascade_limit(size_t);
size_t get_cascade_limit();
void cleanup();
void shutdown();
