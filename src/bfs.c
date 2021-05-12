#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "bitmap.h"
#include "csr.h"
#include "queue.h"
#include "types.h"

/// Serial breadth first search on csr matrix
void bfs(csr *adj, usize src) {
  // Adjacency matrix should be square matrix
  assert(adj->m == adj->n);
  queue *q = queue_new(adj->m);
  bitmap *b = bitmap_new(adj->m);

  queue_push(q, src);
  while (!queue_empty(q)) {
    usize e = queue_pop(q);
    for (usize j = csr_row_begin(adj, e); j < csr_row_end(adj, e); j += 1) {
      if (bitmap_test_set(b, adj->c[j])) {
        queue_push(q, adj->c[j]);
        printf("%lu\n", adj->c[j]);
      }
    }
  }

  queue_free(q);
  bitmap_free(b);
}
