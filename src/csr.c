#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "coo.h"
#include "csr.h"
#include "types.h"

void write_csr(csr *mat, FILE *f) {
  for (u32 i = 0; i < mat->m; i += 1) {
    for (u32 j = csr_row_begin(mat, i); j < csr_row_end(mat, i); j += 1) {
      fprintf(f, "%u\t", mat->c[j]);
    }
    fprintf(f, "\n");
  }
}

csr *csr_new(u32 m, u32 n, u32 nz) {
  u32 *r = malloc(sizeof(u32) * (m + 1));
  u32 *c = malloc(sizeof(u32) * (nz));
  u32 *v = malloc(sizeof(u32) * (nz));
  csr *ret = malloc(sizeof(csr));

  r[m] = nz;
  *ret = (csr){m, n, nz, r, c, v};

  return ret;
}

csr *csr_from_coo(coo *mat) {
  csr *ret = csr_new(mat->m, mat->n, mat->nz);

  memset(ret->r, 0, sizeof(u32) * (mat->m + 1));

  for (u32 i = 0; i < mat->nz; i += 1) {
    ret->r[mat->tup[i].i + 1] += 1;
    ret->c[i] = mat->tup[i].j;
    ret->v[i] = mat->tup[i].v;
  }

  // Prefix sum on `csr->r`
  for (u32 i = 0; i < mat->m; i += 1) {
    ret->r[i + 1] += ret->r[i];
  }

  return ret;
}

u32 csr_row_begin(const csr *mat, u32 i) { return mat->r[i]; }

u32 csr_row_end(const csr *mat, u32 i) { return mat->r[i + 1]; }

u32 csr_row_len(const csr *mat, u32 i) {
  return csr_row_end(mat, i) - csr_row_begin(mat, i);
}

void csr_free(csr *mat) {
  free(mat->r);
  free(mat->c);
  free(mat->v);
  free(mat);
}
