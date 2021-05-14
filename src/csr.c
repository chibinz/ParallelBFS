#include <stdlib.h>
#include <string.h>

#include "coo.h"
#include "csr.h"
#include "types.h"

csr *csr_new(usize m, usize n, usize nz) {
  usize *r = malloc(sizeof(usize) * (m + 1));
  usize *c = malloc(sizeof(usize) * (nz));
  u32 *v = malloc(sizeof(u32) * (nz));
  csr *ret = malloc(sizeof(csr));

  r[m] = nz;
  *ret = (csr){m, n, nz, r, c, v};

  return ret;
}

csr *csr_from_coo(coo *mat) {
  csr *ret = csr_new(mat->m, mat->n, mat->nz);

  memset(ret->r, 0, sizeof(usize) * (mat->m + 1));

  for (usize i = 0; i < mat->nz; i += 1) {
    ret->r[mat->tup[i].i + 1] += 1;
    ret->c[i] = mat->tup[i].j;
    ret->v[i] = mat->tup[i].v;
  }

  // Prefix sum on `csr->r`
  for (usize i = 0; i < mat->m; i += 1) {
    ret->r[i + 1] += ret->r[i];
  }

  return ret;
}

usize csr_row_begin(csr *mat, usize i) { return mat->r[i]; }

usize csr_row_end(csr *mat, usize i) { return mat->r[i + 1]; }

usize csr_row_len(csr *mat, usize i) {
  return csr_row_end(mat, i) - csr_row_begin(mat, i);
}

void csr_free(csr *mat) {
  free(mat->r);
  free(mat->c);
  free(mat->v);
  free(mat);
}
