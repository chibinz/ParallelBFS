#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "types.h"

typedef struct {
  u32 front;
  u32 end;
  u32 capacity;
  u32 *data;
} queue;

queue *queue_new(u32 capacity) {
  u32 *data = malloc(sizeof(u32) * capacity);
  queue *ret = malloc(sizeof(queue));

  *ret = (queue){0, 0, capacity, data};

  return ret;
}

void queue_push(queue *q, u32 e) {
  assert(q->end < q->capacity);

  q->data[q->end] = e;
  q->end += 1;
}

u32 queue_pop(queue *q) {
  assert(q->front != q->end);

  q->front += 1;
  return q->data[q->front - 1];
}

void queue_free(queue *q) {
  free(q->data);
  free(q);
}

bool queue_empty(queue *q) { return q->front == q->end; }
