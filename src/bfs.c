#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "bfs.h"
#include "bitmap.h"
#include "csr.h"
#include "queue.h"
#include "types.h"

void write_bfs_result(bfs_result r, u32 n, FILE *f) {
  for (u32 i = 0; i < n; i += 1) {
    fprintf(f, "%u\t%u\t%u\n", i, r.distance[i], r.parent[i]);
  }
}

bfs_result bfs(csr *adj, u32 src) {
  // Adjacency matrix should be square matrix
  assert(adj->m == adj->n);

  queue *q = queue_new(adj->n);
  bitmap *b = bitmap_new(adj->n);
  u32 *parent = malloc(sizeof(u32) * adj->n);
  u32 *distance = calloc(adj->n, sizeof(u32));

  memset(parent, -1, sizeof(u32) * adj->n);

  queue_push(q, src);
  while (!queue_empty(q)) {
    u32 v = queue_pop(q);
    for (u32 j = csr_row_begin(adj, v); j < csr_row_end(adj, v); j += 1) {
      u32 next = adj->c[j];
      if (parent[next] = (u32)(-1)) {
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
