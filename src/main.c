#include <stdio.h>

#include "types.h"
#include "coo.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: bfs <input>\n");
    return -1;
  }

  FILE *f = fopen(argv[1], "r");

  mat_coo *coo = mat_coo_from_mkt(f);
  mat_coo_sort(coo);
  mat_coo_free(coo);

  fclose(f);

  return 0;
}
