#pragma once

#include "coo.h"
#include "types.h"

/// 'Compressed Sparse Row' representation of sparse matrix
typedef struct matrix_csr {
  /// Number of rows
  usize m;
  /// Number of columns
  usize n;
  /// Number of non-zero elements
  usize nz;
  /// Top level column pointers
  usize *r;
  /// Secondary row indices
  usize *c;
  /// Actual values
  u32 *v;
} csr;

csr *csr_new(usize m, usize n, usize nz);

csr *csr_from_coo(coo* mat);

usize csr_row_begin(csr* mat, usize i);

usize csr_row_end(csr* mat, usize i);

void csr_free(csr *mat);
