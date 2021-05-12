#pragma once

#include "types.h"

typedef struct {
  usize front;
  usize end;
  usize capacity;
  usize *data;
} queue;

queue *queue_new(usize n);

void queue_push(queue *q, usize e);

usize queue_pop(queue *q);

void queue_free(queue *q);

bool queue_empty(queue *q);
