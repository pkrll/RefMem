#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Automated.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "../src/queue.h"

void test_queue_create() {
  queue_t *queue = queue_create();

  CU_ASSERT_PTR_NOT_NULL(queue);
  CU_ASSERT_TRUE(queue_is_empty(queue));

  queue_delete(queue);
}

void test_queue_enqueue() {
  queue_t *queue = queue_create();
  int numbers[4] = {1,2,3,4};

  CU_ASSERT_TRUE(queue_is_empty(queue));

  queue_enqueue(queue, &numbers[0]);
  queue_enqueue(queue, &numbers[1]);
  queue_enqueue(queue, &numbers[2]);
  queue_enqueue(queue, &numbers[3]);

  CU_ASSERT_FALSE(queue_is_empty(queue));

  CU_ASSERT_EQUAL((*(int *) queue_first(queue)), 1);
  CU_ASSERT_EQUAL((*(int *) queue_dequeue(queue)), 1);
  CU_ASSERT_EQUAL((*(int *) queue_first(queue)), 2);
  CU_ASSERT_EQUAL((*(int *) queue_dequeue(queue)), 2);
  CU_ASSERT_EQUAL((*(int *) queue_first(queue)), 3);
  CU_ASSERT_EQUAL((*(int *) queue_dequeue(queue)), 3);
  CU_ASSERT_EQUAL((*(int *) queue_first(queue)), 4);
  CU_ASSERT_EQUAL((*(int *) queue_dequeue(queue)), 4);

  queue_delete(queue);
}

void test_queue_first() {
  queue_t *queue = queue_create();
  int testobject = 10;

  queue_enqueue(queue, &testobject);

  CU_ASSERT_FALSE(queue_is_empty(queue));
  CU_ASSERT_EQUAL((*(int *) queue_first(queue)), 10);

  queue_delete(queue);
}

void test_queue_length() {
  queue_t *queue = queue_create();
  CU_ASSERT_EQUAL(queue_length(queue), 0);

  int numbers[4] = {1,2,3,4};

  queue_enqueue(queue, &numbers[0]);
  CU_ASSERT_EQUAL(queue_length(queue), 1);
  queue_enqueue(queue, &numbers[1]);
  CU_ASSERT_EQUAL(queue_length(queue), 2);
  queue_enqueue(queue, &numbers[2]);
  CU_ASSERT_EQUAL(queue_length(queue), 3);
  queue_enqueue(queue, &numbers[3]);
  CU_ASSERT_EQUAL(queue_length(queue), 4);

  queue_dequeue(queue);

  CU_ASSERT_EQUAL(queue_length(queue), 3);

  queue_dequeue(queue);
  queue_dequeue(queue);
  queue_dequeue(queue);

  CU_ASSERT_EQUAL(queue_length(queue), 0);

  queue_delete(queue);
}

void test_queue_dequeue() {
  queue_t *queue = queue_create();

  int numbers[4] = {1,2,3,4};

  queue_enqueue(queue, &numbers[0]);
  queue_dequeue(queue);
  CU_ASSERT_EQUAL(queue_length(queue), 0);
  queue_enqueue(queue, &numbers[1]);
  queue_enqueue(queue, &numbers[2]);
  queue_enqueue(queue, &numbers[3]);

  CU_ASSERT_EQUAL(queue_length(queue), 3);
  queue_dequeue(queue);
  queue_dequeue(queue);
  queue_dequeue(queue);
  CU_ASSERT_EQUAL(queue_length(queue), 0);

  queue_delete(queue);
}

void test_queue_is_empty() {
  queue_t *queue = queue_create();
  CU_ASSERT_TRUE(queue_is_empty(queue));

  int numbers[4] = {1,2,3,4};
  queue_enqueue(queue, &numbers[0]);

  CU_ASSERT_FALSE(queue_is_empty(queue));

  queue_delete(queue);
}

void apply_func(void *item, void *data) {
  (*(int *)item)++;
}

void test_apply() {
  queue_t *queue = queue_create();
  int numbers[4] = {1,2,3,4};

  queue_enqueue(queue, &numbers[0]);
  queue_enqueue(queue, &numbers[1]);
  queue_enqueue(queue, &numbers[2]);
  queue_enqueue(queue, &numbers[3]);

  queue_apply(queue, apply_func, NULL);

  CU_ASSERT_EQUAL((*(int *) queue_first(queue)), 2);
  CU_ASSERT_EQUAL((*(int *) queue_dequeue(queue)), 2);
  CU_ASSERT_EQUAL((*(int *) queue_first(queue)), 3);
  CU_ASSERT_EQUAL((*(int *) queue_dequeue(queue)), 3);
  CU_ASSERT_EQUAL((*(int *) queue_first(queue)), 4);
  CU_ASSERT_EQUAL((*(int *) queue_dequeue(queue)), 4);
  CU_ASSERT_EQUAL((*(int *) queue_first(queue)), 5);
  CU_ASSERT_EQUAL((*(int *) queue_dequeue(queue)), 5);


  queue_delete(queue);
}

int main(int argc, char *argv[]) {
  CU_initialize_registry();

  CU_pSuite creation = CU_add_suite("Test creation", NULL, NULL);
  CU_add_test(creation, "Queue new", test_queue_create);
  CU_add_test(creation, "Queue enqueue", test_queue_enqueue);
  CU_add_test(creation, "Queue dequeue", test_queue_dequeue);
  CU_add_test(creation, "Queue first", test_queue_first);
  CU_add_test(creation, "Queue length", test_queue_length);
  CU_add_test(creation, "Queue is empty", test_queue_is_empty);
  CU_add_test(creation, "Queue apply", test_apply);

  CU_basic_run_tests();
  CU_cleanup_registry();

  return CU_get_error();
}
