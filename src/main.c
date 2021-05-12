#include <stdio.h>

#include "types.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: bfs <input>\n");
    return -1;
  }

  FILE *f = fopen(argv[1], "r");
  u32 row, col, val;

  while (fscanf(f, "%u %u %u\n", &row, &col, &val) != EOF) {
    printf("%u %u %u\n", row, col, val);
  }

  return 0;
}
