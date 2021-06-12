#pragma once

#include "types.h"

typedef struct {
  u32 front;
  u32 end;
  u32 capacity;
  u32 *data;
} queue;

queue *queue_new(u32 n);

void queue_push(queue *q, u32 e);

u32 queue_pop(queue *q);

void queue_free(queue *q);

bool queue_empty(queue *q);
