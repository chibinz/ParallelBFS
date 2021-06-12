#pragma once

#include "coo.h"
#include "types.h"

/// 'Compressed Sparse Row' representation of sparse matrix
typedef struct matrix_csr {
  /// Number of rows
  u32 m;
  /// Number of columns
  u32 n;
  /// Number of non-zero elements
  u32 nz;
  /// Top level column pointers
  u32 *r;
  /// Secondary row indices
  u32 *c;
  /// Actual values
  u32 *v;
} csr;

csr *csr_new(u32 m, u32 n, u32 nz);

csr *csr_from_coo(coo *mat);

u32 csr_row_begin(const csr *mat, u32 i);

u32 csr_row_end(const csr *mat, u32 i);

u32 csr_row_len(const csr *mat, u32 i);

void csr_free(csr *mat);
