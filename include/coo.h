#pragma once

#include <stdio.h>

#include "types.h"

/// 3 - tuple containing single coordinate list matrix element
typedef struct {
  /// Row index
  usize i;
  /// Column index
  usize j;
  /// Actual value
  u32 v;
} coo_tup;

/// 'Coordinate List' representation of sparse matrix
typedef struct matrix_coo {
  /// Number of rows
  usize m;
  /// Number of columns
  usize n;
  /// Number of non-zero values
  usize nz;
  /// Array of 3 - tuples
  coo_tup *tup;
} coo;

/// Allocate memory
coo *coo_new(usize m, usize n, usize nz);

/// Read from matrix market file into coo representation
coo *coo_from_mkt(FILE *mkt);

/// Read from edge list file into coo representation
coo *coo_from_edge(FILE *mkt);

/// Write coo matrix to matrix market file
void coo_to_mkt(coo *mat, FILE *mkt);

/// Free structure and inner fields
void coo_free(coo *mat);

/// Sort matrix first by its row index, and then column index
void coo_sort(coo *mat);
