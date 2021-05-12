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
  /// Number of non-zero values
  u32 nval;
} mat_coo;

/// Allocate memory
mat_coo *mat_coo_new(usize nval);

/// Free structure and inner fields
void mat_coo_free(mat_coo *mat);

/// Casting to u64 retrieves the first 2 field of tup concatenated together
static int cmp_func(const void *a, const void *b);

/// Sort matrix first by its row index, and then column index
void mat_coo_sort(mat_coo *mat);
