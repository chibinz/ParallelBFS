#include <stdlib.h>

#include "coo.h"
#include "types.h"

mat_coo *mat_coo_new(usize nval) {
  mat_coo_tup * tup = malloc(sizeof(mat_coo_tup) * nval);
  mat_coo *ret = malloc(sizeof(mat_coo));

  *ret = (mat_coo){tup, nval};

  return ret;
}

void mat_coo_free(mat_coo *mat) {
  free(mat->tup);
  free(mat);
}

/// Casting to u64 retrieves the first 2 field of tup concatenated together
static int cmp_func(const void *a, const void *b) {
  return *(u64 *)(a) - *(u64 *)(b);
}

void mat_coo_sort(mat_coo *mat) {
  qsort(mat->tup, mat->nval, sizeof(mat_coo_tup), cmp_func);
}
