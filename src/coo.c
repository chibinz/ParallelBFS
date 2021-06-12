#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "coo.h"
#include "types.h"

coo *coo_new(u32 m, u32 n, u32 nz) {
  coo_tup *tup = malloc(sizeof(coo_tup) * nz);
  coo *ret = malloc(sizeof(coo));

  *ret = (coo){m, n, nz, tup};

  return ret;
}

coo *coo_from_mkt(FILE *mkt) {
  u32 m, n, nz, i = 0;
  assert(fscanf(mkt, "%u %u %u\n", &m, &n, &nz) != EOF);

  coo *ret = coo_new(m, n, nz);
  while (fscanf(mkt, "%u %u %u\n", &ret->tup[i].i, &ret->tup[i].j,
                &ret->tup[i].v) != EOF) {
    // Matrix indices start from 1...
    // ret->tup[i].i -= 1;
    // ret->tup[i].j -= 1;
    assert(ret->tup[i].i < m);
    assert(ret->tup[i].j < n);
    i += 1;
  }

  return ret;
}

coo *coo_from_edge(FILE *edge, u32 n, u32 nz) {
  u32 i = 0;

  coo *ret = coo_new(n, n, nz);
  while (fscanf(edge, "%u %u\n", &ret->tup[i].i, &ret->tup[i].j) != EOF) {
    assert(ret->tup[i].i < n);
    assert(ret->tup[i].j < n);
    i += 1;
  }

  return ret;
}

void coo_to_mkt(coo *mat, FILE *mkt) {
  fprintf(mkt, "%u\t%u\t%u\n", mat->m, mat->n, mat->nz);

  for (u32 i = 0; i < mat->nz; i += 1) {
    fprintf(mkt, "%u\t%u\t%u\n", mat->tup[i].i, mat->tup[i].j, mat->tup[i].v);
  }
}

void coo_free(coo *mat) {
  free(mat->tup);
  free(mat);
}

static int cmp_func(const void *a, const void *b) {
  const coo_tup *ta = a;
  const coo_tup *tb = b;
  u32 di = ta->i - tb->i;
  u32 dj = ta->j - tb->j;

  return di == 0 ? dj : di;
}

void coo_sort(coo *mat) { qsort(mat->tup, mat->nz, sizeof(coo_tup), cmp_func); }
