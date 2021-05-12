#include <stdio.h>

#include "bfs.h"
#include "coo.h"
#include "csr.h"
#include "types.h"

static void print_csr(csr *mat) {
  for (usize i = 0; i < mat->m; i += 1) {
    for (usize j = csr_row_begin(mat, i); j < csr_row_end(mat, i); j += 1) {
      printf("%lu\t", mat->c[j]);
    }
    printf("\n");
  }
}

static coo_tup list[] = {
    {0, 1, 1}, {0, 2, 1}, {1, 3, 1}, {1, 4, 1},
    {2, 4, 1}, {3, 4, 1}, {3, 5, 1}, {4, 5, 1},
};

static coo simple = {6, 6, 8, list};

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: bfs <input>\n");
    return -1;
  }

  FILE *in = fopen(argv[1], "r");

  coo *coo = coo_from_mkt(in);
  coo_sort(coo);

  coo_sort(&simple);
  csr *csr = csr_from_coo(&simple);

  print_csr(csr);
  bfs(csr, 0);

  csr_free(csr);
  coo_free(coo);
  fclose(in);

  return 0;
}
