#pragma once

#include <stdbool.h>

#include "types.h"

#define SENTINEL (u32)(-1)

typedef struct {
  u32 *node;
  u32 len;
} frontier;

void prefix_sum(u32 *a, u32 len);

void prefix_sum_omp(u32 *a, u32 len);

frontier *frontier_new(u32 n);

frontier *frontier_with_src(u32 src);

bool frontier_empty(frontier *f);

void frontier_cull(frontier *f, frontier *fnext);

void frontier_free(frontier *f);
