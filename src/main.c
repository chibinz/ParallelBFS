#include <stdio.h>

#include "types.h"
#include "coo.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: bfs <input>\n");
    return -1;
  }

  FILE *in = fopen(argv[1], "r");

  coo *coo = coo_from_mkt(in);
  coo_sort(coo);
  coo_to_mkt(coo, stdout);
  coo_free(coo);

  fclose(in);

  return 0;
}
