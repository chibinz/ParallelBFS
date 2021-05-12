#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "types.h"

typedef struct {
  usize front;
  usize end;
  usize capacity;
  usize *data;
} queue;

queue *queue_new(usize capacity) {
  usize *data = malloc(sizeof(usize) * capacity);
  queue *ret = malloc(sizeof(queue));

  *ret = (queue){0, 0, capacity, data};

  return ret;
}

void queue_push(queue *q, usize e) {
  assert(q->end < q->capacity);

  q->data[q->end] = e;
  q->end += 1;
}

usize queue_pop(queue *q) {
  assert(q->front != q->end);

  q->front += 1;
  return q->data[q->front - 1];
}

void queue_free(queue *q) {
  free(q->data);
  free(q);
}

bool queue_empty(queue *q) { return q->front == q->end; }
