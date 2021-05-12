#include "csr.h"
#include "coo.h"
#include "types.h"

csr *csr_new(usize m, usize n, usize nz) {
  usize *r = malloc(sizeof(usize) * (m + 1));
  usize *c = malloc(sizeof(usize) * (nz));
  usize *v = malloc(sizeof(usize) * (nz));
  csr *ret = malloc(sizeof(csr));

  r[m] = nz;
  *ret = (csr){m, n, nz, r, c, v};

  return ret;
}

void csr_free(csr *mat) {
  free(mat->r);
  free(mat->c);
  free(mat->v);
  free(mat);
}
