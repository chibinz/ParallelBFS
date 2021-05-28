#include <omp.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdlib.h>

#include "frontier.h"
#include "types.h"

void prefix_sum(usize *a, usize len) {
  for (usize i = 1; i < len; i += 1) {
    a[i] = a[i] + a[i - 1];
  }
}

void prefix_sum_omp(usize *a, usize len) {
  usize *local;
#pragma omp parallel
  {
    usize id = omp_get_thread_num();
    usize n = omp_get_num_threads();
#pragma omp single
    local = malloc(sizeof *local * (n + 1)), local[0] = 0;

    usize s = 0;
#pragma omp for schedule(static) nowait
    for (usize i = 0; i < len; i++) {
      s += a[i];
      a[i] = s;
    }
    local[id + 1] = s;

#pragma omp barrier
    usize offset = 0;
    for (usize i = 0; i < id + 1; i++)
      offset += local[i];

#pragma omp for schedule(static)
    for (usize i = 0; i < len; i++) {
      a[i] += offset;
    }
  }
  free(local);
}

frontier *frontier_new(usize n) {
  usize *node = malloc(sizeof(usize) * n);
  frontier *ret = malloc(sizeof(frontier));

  *ret = (frontier){node, n};

  return ret;
}

frontier *frontier_with_src(usize src) {
  frontier *ret = frontier_new(1);
  ret->node[0] = src;
  ret->len = 1;

  return ret;
}

bool frontier_empty(frontier *f) { return f->len == 0; }

void frontier_cull(frontier *f) {
  usize *index = malloc(sizeof(usize) * (f->len + 1));
  index[0] = 0;

#pragma omp parallel for
  for (usize i = 0; i < f->len; i += 1) {
    index[i + 1] = (usize)(f->node[i] != SENTINEL);
  }

  prefix_sum_omp(index, f->len + 1);

  usize newlen = index[f->len];
  usize *newnode = malloc(sizeof(usize) * newlen);

#pragma omp parallel for
  for (usize i = 0; i < f->len; i += 1) {
    if (f->node[i] != SENTINEL) {
      newnode[index[i]] = f->node[i];
    }
  }

  free(index);
  free(f->node);
  f->node = newnode;
  f->len = newlen;
}

void frontier_free(frontier *f) {
  free(f->node);
  free(f);
}
