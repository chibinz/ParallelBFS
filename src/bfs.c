#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "bitmap.h"
#include "csr.h"
#include "queue.h"
#include "types.h"

/// Output Format
/// <id> <distance> <parent>
typedef struct {
  usize *parent;
  usize *distance;
} bfs_result;

void write_bfs_result(bfs_result r, usize n, FILE *f) {
  for (usize i = 0; i < n; i += 1) {
    fprintf(f, "%lu\t%lu\t%lu\n", i, r.distance[i], r.parent[i]);
  }
}

/// Serial breadth first search on csr matrix
bfs_result bfs(csr *adj, usize src) {
  // Adjacency matrix should be square matrix
  assert(adj->m == adj->n);

  queue *q = queue_new(adj->n);
  bitmap *b = bitmap_new(adj->n);
  usize *parent = malloc(sizeof(usize) * adj->n);
  usize *distance = malloc(sizeof(usize) * adj->n);

  queue_push(q, src);
  while (!queue_empty(q)) {
    usize e = queue_pop(q);
    for (usize j = csr_row_begin(adj, e); j < csr_row_end(adj, e); j += 1) {
      if (bitmap_test_set(b, adj->c[j])) {
        queue_push(q, adj->c[j]);
        parent[adj->c[j]] = e;
        distance[adj->c[j]] = distance[e] + 1;
      }
    }
  }

  queue_free(q);
  bitmap_free(b);

  return (bfs_result){parent, distance};
}
