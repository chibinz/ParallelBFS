#include <stdio.h>
#include <stdlib.h>

#include "coo.h"
#include "types.h"

mat_coo *mat_coo_new(usize nrow, usize ncol, usize nval) {
  mat_coo_tup *tup = malloc(sizeof(mat_coo_tup) * nval);
  mat_coo *ret = malloc(sizeof(mat_coo));

  *ret = (mat_coo){tup, nrow, ncol, nval};

  return ret;
}

mat_coo *mat_coo_from_mkt(FILE *mkt) {
  usize nrow, ncol, nval, i = 0;
  fscanf(mkt, "%lu %lu %lu\n", &nrow, &ncol, &nval);

  mat_coo *ret = mat_coo_new(nrow, ncol, nval);
  while (fscanf(mkt, "%u %u %u\n", &ret->tup[i].row, &ret->tup[i].col,
                &ret->tup[i].val) != EOF) {
    i += 1;
  }

  return ret;
}

void mat_coo_free(mat_coo *mat) {
  free(mat->tup);
  free(mat);
}

static int cmp_func(const void *a, const void *b) {
  const mat_coo_tup *ta = a;
  const mat_coo_tup *tb = b;
  u32 drow = ta->row - tb->row;
  u32 dcol = ta->col - tb->col;

  return drow == 0 ? dcol : drow;
}

void mat_coo_sort(mat_coo *mat) {
  qsort(mat->tup, mat->nval, sizeof(mat_coo_tup), cmp_func);
}
