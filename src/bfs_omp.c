#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "bfs.h"
#include "bitmap.h"
#include "frontier.h"
#include "types.h"

bfs_result bfs_omp(csr *adj, usize src) {
  // Adjacency matrix should be square matrix
  assert(adj->m == adj->n);

  frontier *f = frontier_with_src(src);
  bitmap *b = bitmap_new(adj->n);

  // Scratch buffer for storing degrees of vertices in frontier
  usize *degree = malloc(sizeof(usize) * (adj->n + 1));
  usize *parent = malloc(sizeof(usize) * adj->n);
  usize *distance = calloc(adj->n, sizeof(usize));

  degree[0] = 0;
  distance[src] = 0;
  parent[src] = src;

  while (!frontier_empty(f)) {

    // Collect vertex degrees
#pragma omp parallel for
    for (usize i = 0; i < f->len; i += 1) {
      degree[i + 1] = adj->r[f->node[i] + 1] - adj->r[f->node[i]];
    }

    // Run prefix sum on degree on array
    prefix_sum(degree, f->len + 1);
    frontier *newf = frontier_new(degree[f->len]);

#pragma omp parallel for
    for (usize i = 0; i < f->len; i += 1) {
      usize v = f->node[i], index = degree[i];
      for (usize j = 0; j < csr_row_len(adj, v); j += 1) {
        usize next = adj->c[csr_row_begin(adj, v) + j];
        if (!bitmap_test_set(b, next)) {
          newf->node[index + j] = next;
          distance[next] = distance[v] + 1;
        } else {
          newf->node[index + j] = SENTINEL;
        }
      }
    }

    frontier_free(f);
    frontier_cull(newf);
    f = newf;
  }

  frontier_free(f);
  bitmap_free(b);
  free(degree);

  return (bfs_result){parent, distance};
}
