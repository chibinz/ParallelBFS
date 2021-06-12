#include <omp.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdlib.h>

#include "frontier.h"
#include "types.h"

void prefix_sum(u32 *a, u32 len) {
  for (u32 i = 1; i < len; i += 1) {
    a[i] = a[i] + a[i - 1];
  }
}

void prefix_sum_omp(u32 *a, u32 len) {
  u32 *local;
#pragma omp parallel
  {
    u32 id = omp_get_thread_num();
    u32 n = omp_get_num_threads();
#pragma omp single
    local = malloc(sizeof *local * (n + 1)), local[0] = 0;

    u32 s = 0;
#pragma omp for schedule(static) nowait
    for (u32 i = 0; i < len; i++) {
      s += a[i];
      a[i] = s;
    }
    local[id + 1] = s;

#pragma omp barrier
    u32 offset = 0;
    for (u32 i = 0; i < id + 1; i++)
      offset += local[i];

#pragma omp for schedule(static)
    for (u32 i = 0; i < len; i++) {
      a[i] += offset;
    }
  }
  free(local);
}

frontier *frontier_new(u32 n) {
  u32 *node = malloc(sizeof(u32) * n);
  frontier *ret = malloc(sizeof(frontier));

  *ret = (frontier){node, n};

  return ret;
}

frontier *frontier_with_src(u32 src) {
  frontier *ret = frontier_new(1);
  ret->node[0] = src;
  ret->len = 1;

  return ret;
}

bool frontier_empty(frontier *f) { return f->len == 0; }

void frontier_cull(frontier *f, frontier *fnext) {
  u32 *index = malloc(sizeof(u32) * (f->len + 1));
  index[0] = 0;

#pragma omp parallel for
  for (u32 i = 0; i < f->len; i += 1) {
    index[i + 1] = (u32)(f->node[i] != SENTINEL);
  }

  prefix_sum_omp(index, f->len + 1);
  fnext->len = index[f->len];

#pragma omp parallel for
  for (u32 i = 0; i < f->len; i += 1) {
    if (f->node[i] != SENTINEL) {
      fnext->node[index[i]] = f->node[i];
    }
  }

  free(index);
}

void frontier_free(frontier *f) {
  free(f->node);
  free(f);
}
