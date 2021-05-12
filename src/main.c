#include <stdio.h>

#include "types.h"
#include "coo.h"
#include "csr.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: bfs <input>\n");
    return -1;
  }

  FILE *in = fopen(argv[1], "r");

  coo *coo = coo_from_mkt(in);
  coo_sort(coo);

  csr *csr = csr_from_coo(coo);
  for (usize i = 0; i < csr->m + 1; i += 1) {
    printf("%lu\n", csr->r[i]);
  }

  csr_free(csr);
  coo_free(coo);

  fclose(in);

  return 0;
}
