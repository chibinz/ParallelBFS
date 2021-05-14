#include <assert.h>
#include <omp.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "bfs.h"
#include "bitmap.h"
#include "types.h"

typedef struct {
  usize *node;
  usize len;
} frontier;

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
  usize *index = malloc(sizeof(usize) * f->len);
  for (usize i = 1; i < f->len; i += 1) {

  }
}

void frontier_free(frontier *f) {
  free(f->node);
  free(f);
}

void prefix_sum(usize *a, usize len) {
  for (usize i = 1; i < len; i += 1) {
    a[i] = a[i] + a[i - 1];
  }
}

bfs_result bfs_omp(csr *adj, usize src) {
  // Adjacency matrix should be square matrix
  assert(adj->m == adj->n);

  frontier *f = frontier_with_src(src);
  bitmap *b = bitmap_new(adj->n);
  // Scratch buffer for storing degrees of vertices in frontier
  usize *degree = malloc(sizeof(usize) * adj->n);
  usize *parent = malloc(sizeof(usize) * adj->n);
  usize *distance = calloc(adj->n, sizeof(usize));

  memset(parent, -1, sizeof(usize) * adj->n);

  while (!frontier_empty(f)) {

    // Collect vertex degrees
#pragma omp parallel for
    for (usize i = 0; i < f->len; i += 1) {
      degree[i] = adj->r[f->node[i] + 1] - adj->r[f->node[i]];
    }

    // Run prefix sum on degree on array
    prefix_sum(degree, f->len);
    frontier *newf = frontier_new(degree[f->len - 1]);

#pragma omp parallel for
    for (usize i = 0; i < f->len; i += 1) {
      usize v = f->node[i], index = degree[i];
      for (usize j = csr_row_begin(adj, v); j < csr_row_end(adj, v); j += 1) {
        usize next = adj->c[j];
        usize new = -1;
        if (parent[next] == -1 &&
            atomic_compare_exchange_weak(&parent[next], &new, v)) {
          newf->node[index + j] = next;
        } else {
          newf->node[index + j] = -1;
        }
      }
    }

    frontier_free(f);
    f = newf;
  }

  frontier_free(f);
  bitmap_free(b);

  return (bfs_result){parent, distance};
}
