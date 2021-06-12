#pragma once

#include "csr.h"
#include "types.h"

/// Output Format
/// <id> <distance> <parent>
typedef struct {
  u32 *parent;
  u32 *distance;
} bfs_result;

typedef bfs_result (*bfs_func)(csr *, u32);

/// Serial breadth first search on csr matrix
bfs_result bfs(csr *adj, u32 src);

/// Parallel breadth first search with OpenMP
bfs_result bfs_omp(csr *adj, u32 src);

/// Write bfs result to file
void write_bfs_result(bfs_result r, u32 n, FILE *f);
