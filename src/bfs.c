#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "bfs.h"
#include "bitmap.h"
#include "csr.h"
#include "queue.h"
#include "types.h"

void write_bfs_result(bfs_result r, usize n, FILE *f) {
  for (usize i = 0; i < n; i += 1) {
    fprintf(f, "%lu\t%lu\t%lu\n", i, r.distance[i], r.parent[i]);
  }
}

bfs_result bfs(csr *adj, usize src) {
  // Adjacency matrix should be square matrix
  assert(adj->m == adj->n);

  queue *q = queue_new(adj->n);
  bitmap *b = bitmap_new(adj->n);
  usize *parent = malloc(sizeof(usize) * adj->n);
  usize *distance = malloc(sizeof(usize) * adj->n);

  queue_push(q, src);
  while (!queue_empty(q)) {
    usize v = queue_pop(q);
    for (usize j = csr_row_begin(adj, v); j < csr_row_end(adj, v); j += 1) {
      usize next = adj->c[j];
      if (bitmap_test_set(b, next)) {
        queue_push(q, next);
        parent[next] = v;
        distance[next] = distance[v] + 1;
      }
    }
  }

  queue_free(q);
  bitmap_free(b);

  return (bfs_result){parent, distance};
}
