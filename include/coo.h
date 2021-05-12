#pragma once

#include "types.h"

/// 3 - tuple containing single coordinate list matrix element
typedef struct {
  /// Row index
  u32 row;
  /// Column index
  u32 col;
  /// Actual value
  u32 val;
} mat_coo_tup;

/// Structure for holding sparse matrix in 'Coordinate List' form
typedef struct {
  /// Array of 3 - tuples
  mat_coo_tup *tup;
  /// Number of rows
  u32 nrow;
  /// Number of columns
  u32 ncol;
  /// Number of non-zero values
  u32 nval;
} mat_coo;

/// Allocate memory
mat_coo *mat_coo_new(usize nrow, usize ncol, usize nval);

/// Read from matrix market file into coo format
mat_coo *mat_coo_from_mkt(FILE *mkt);

/// Free structure and inner fields
void mat_coo_free(mat_coo *mat);

/// Sort matrix first by its row index, and then column index
void mat_coo_sort(mat_coo *mat);
