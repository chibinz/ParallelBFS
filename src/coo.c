#include <stdio.h>
#include <stdlib.h>

#include "coo.h"
#include "types.h"

coo *coo_new(usize m, usize n, usize nz) {
  coo_tup *tup = malloc(sizeof(coo_tup) * nz);
  coo *ret = malloc(sizeof(coo));

  *ret = (coo){m, n, nz, tup};

  return ret;
}

coo *coo_from_mkt(FILE *mkt) {
  usize m, n, nz, i = 0;
  fscanf(mkt, "%lu %lu %lu\n", &m, &n, &nz);

  coo *ret = coo_new(m, n, nz);
  while (fscanf(mkt, "%u %u %u\n", &ret->tup[i].i, &ret->tup[i].j,
                &ret->tup[i].v) != EOF) {
    i += 1;
  }

  return ret;
}

void coo_to_mkt(coo *mat, FILE *mkt) {
  fprintf(mkt, "%lu %lu %lu\n", mat->m, mat->n, mat->nz);

  for (usize i = 0; i < mat->nz; i += 1) {
    fprintf(mkt, "%u %u %u\n", mat->tup[i].i, mat->tup[i].j,
            mat->tup[i].v);
  }
}

void coo_free(coo *mat) {
  free(mat->tup);
  free(mat);
}

static int cmp_func(const void *a, const void *b) {
  const coo_tup *ta = a;
  const coo_tup *tb = b;
  u32 drow = ta->i - tb->i;
  u32 dcol = ta->j - tb->j;

  return drow == 0 ? dcol : drow;
}

void coo_sort(coo *mat) { qsort(mat->tup, mat->nz, sizeof(coo_tup), cmp_func); }
