#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "bfs.h"
#include "bitmap.h"
#include "frontier.h"
#include "types.h"

bfs_result bfs_omp(csr *adj, u32 src) {
  // Adjacency matrix should be square matrix
  assert(adj->m == adj->n);

  frontier *f = frontier_new(adj->nz);
  frontier *fnext = frontier_new(adj->nz);
  bitmap *b = bitmap_new(adj->n);
  bitmap *bnext = bitmap_new(adj->n);

  // Start with top-down method
  bool top_down = true;
  u32 mf = csr_row_len(adj, src);
  u32 nf = 1;
  u32 mu = adj->nz;

  // Scratch buffer for storing degrees of vertices in frontier
  u32 *degree = malloc(sizeof(u32) * (adj->n + 1));
  u32 *parent = malloc(sizeof(u32) * adj->n);
  u32 *distance = calloc(adj->n, sizeof(u32));

  degree[0] = 0;
  distance[src] = 0;
  parent[src] = src;

  f->node[0] = src;
  f->len = 1;

  while (!(top_down && frontier_empty(f))) {
    // Collect vertex degrees

    if (top_down) {
      /// Top down travesal
#pragma omp parallel for
      for (u32 i = 0; i < f->len; i += 1) {
        degree[i + 1] = adj->r[f->node[i] + 1] - adj->r[f->node[i]];
      }

      // Run prefix sum on degree on array
      prefix_sum(degree, f->len + 1);
      fnext->len = degree[f->len];

#pragma omp parallel for schedule(guided) reduction(+: mf, nf) reduction(- : mu)
      for (u32 i = 0; i < f->len; i += 1) {
        u32 v = f->node[i], index = degree[i];
        for (u32 j = 0; j < csr_row_len(adj, v); j += 1) {
          /// \todo: Frequent cache miss here... Graph compression?
          u32 next = adj->c[csr_row_begin(adj, v) + j];
          if (!bitmap_test_set(b, next)) {
            fnext->node[index + j] = next;
            distance[next] = distance[v] + 1;

            mf += csr_row_len(adj, next);
            mu -= csr_row_len(adj, next);
            nf += 1;
          } else {
            fnext->node[index + j] = SENTINEL;
          }
        }
      }

      frontier_cull(fnext, f);
    } else {
      /// Bottom up travesal
      bitmap_clear(bnext);

#pragma omp parallel for schedule(guided) reduction(+ : mf, nf) reduction(- : mu)
      for (u32 v = 0; v < adj->n; v += 1) {
        if (!bitmap_test(b, v)) {
          for (u32 i = csr_row_begin(adj, v); i < csr_row_end(adj, v); i += 1) {
            /// \todo: Frequent cache miss here too... 40% Global, 22% LLC...
            if (bitmap_test(b, adj->c[i])) {
              bitmap_set(bnext, v);
              distance[v] = distance[adj->c[i]] + 1;

              mf += csr_row_len(adj, v);
              mu -= csr_row_len(adj, v);
              nf += 1;
              break;
            }
          }
        }
      }

      bitmap_merge(b, bnext);
    }

    if (top_down) {
      if (mf > mu / 14) {
        top_down = false;
      }
    } else {
      if (nf < adj->n / 24) {
        f->len = bitmap2array(bnext, f->node);
        top_down = true;
      }
    }

    mf = 0;
    nf = 0;
  }

  frontier_free(f);
  frontier_free(fnext);
  bitmap_free(b);
  bitmap_free(bnext);
  free(degree);

  return (bfs_result){parent, distance};
}
