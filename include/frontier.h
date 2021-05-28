#pragma once

#include <stdbool.h>

#include "types.h"

#define SENTINEL (usize)(-1)

typedef struct {
  usize *node;
  usize len;
} frontier;

void prefix_sum(usize *a, usize len);

void prefix_sum_omp(usize *a, usize len);

frontier *frontier_new(usize n);

frontier *frontier_with_src(usize src);

bool frontier_empty(frontier *f);

void frontier_cull(frontier *f);

void frontier_free(frontier *f);
