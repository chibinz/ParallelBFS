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

  prefix_sum(index, f->len + 1);

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
