#include <assert.h>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "bfs.h"
#include "coo.h"
#include "csr.h"
#include "types.h"

static coo_tup list[] = {
    {0, 1, 1}, {0, 2, 1}, {1, 3, 1}, {1, 4, 1},
    {2, 4, 1}, {3, 4, 1}, {3, 5, 1}, {4, 5, 1},
};

static coo simple = {6, 6, 8, list};

void test(csr *adj) {
  bfs_result s = bfs(adj, 1);
  bfs_result p = bfs_omp(adj, 1);
  assert(memcmp(s.distance, p.distance, adj->n) == 0);
  free(s.distance);
  free(p.distance);
  free(s.parent);
  free(p.parent);
}

static double bench_one(bfs_func func, csr *adj, u32 src) {
  double start = omp_get_wtime();
  bfs_result r = func(adj, src);
  double end = omp_get_wtime();
  free(r.distance);
  free(r.parent);
  return end - start;
}

static void bench_multiple(bfs_func func, csr *adj, u32 n, char *name) {
  srand(0xC5121);
  double sum = 0.0, sum2 = 0.0;
  for (u32 i = 0; i < n; i += 1) {
    double dt = bench_one(func, adj, rand() % adj->n);
    sum += dt;
    sum2 += dt * dt;
  }
  double mean = sum / (double)(n);
  double stddev = sqrt((sum2 / (double)(n)) - mean * mean);
  double edges = (double)(adj->nz) / 1e6;
  printf("%-16s%-16.4f%-16.4f%-16.4f\n", name, edges / mean,
         edges * stddev / mean, mean);
}

int main(int argc, char **argv) {
  coo *coo;
  FILE *in;
  if (argc == 2) {
    in = fopen(argv[1], "r");
    coo = coo_from_mkt(in);
  } else if (argc == 4) {
    u32 n = atoi(argv[2]);
    u32 nz = atoi(argv[3]);
    in = fopen(argv[1], "r");
    coo = coo_from_edge(in, n, nz);
  } else {
    printf("Usage: bfs <input> [vertices] [edges]...\n");
    return -1;
  }
  fclose(in);

  coo_sort(&simple);
  coo_sort(coo);
  csr *csr = csr_from_coo(coo);

  // printf("%s\t", argv[1]);
  // printf("%-16s%-16s%-16s%-16s\n", "32*Iter", "Mean/MEdges", "StdDev/MEdges",
  //  "Wall/s");
  // bench_multiple(bfs, csr, 32, "Serial(Ts)");
  bench_multiple(bfs_omp, csr, 32, "Parallel(Tp)");
  // omp_set_num_threads(1);
  // bench_multiple(bfs_omp, csr, 32, "Parallel(T1)");

  csr_free(csr);
  coo_free(coo);

  return 0;
}
