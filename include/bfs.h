#pragma once

#include "csr.h"
#include "types.h"

/// Output Format
/// <id> <distance> <parent>
typedef struct {
  usize *parent;
  usize *distance;
} bfs_result;

/// Serial breadth first search on csr matrix
bfs_result bfs(csr *adj, usize src);

/// Parallel breadth first search with OpenMP
bfs_result bfs_omp(csr *adj, usize src);

/// Write bfs result to file
void write_bfs_result(bfs_result r, usize n, FILE *f);
