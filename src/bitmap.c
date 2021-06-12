#include <assert.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdlib.h>

#include "bitmap.h"
#include "types.h"

bitmap *bitmap_new(u32 capacity) {
  atomic_uint *map = calloc((capacity / 32 + 1), sizeof(atomic_uint));
  bitmap *ret = malloc(sizeof(bitmap));

  *ret = (bitmap){capacity, map};

  return ret;
}

void bitmap_set(bitmap *b, u32 n) {
  u32 index = n / 32;
  u32 offset = n % 32;
  b->map[index] |= 1 << offset;
}

bool bitmap_test(bitmap *b, u32 n) {
  u32 index = n / 32;
  u32 offset = n % 32;
  return !!((b->map[index] >> offset) & 1);
}

bool bitmap_test_set(bitmap *b, u32 n) {
  u32 index = n / 32;
  u32 offset = n % 32;
  bool prev = !!((b->map[index] >> offset) & 1);

  // Guard to prevent unnecessary contention
  if (!prev) {
    prev = !!((atomic_fetch_or(&b->map[index], 1 << offset) >> offset) & 1);
  }

  return prev;
}

void bitmap_clear(bitmap *b) {
#pragma omp parallel for
  for (u32 i = 0; i < b->capacity / 32 + 1; i += 1) {
    b->map[i] = 0;
  }
}

void bitmap_merge(bitmap *self, bitmap *other) {
  assert(self->capacity == other->capacity);

#pragma omp parallel for
  for (u32 i = 0; i < self->capacity / 32 + 1; i += 1) {
    self->map[i] |= other->map[i];
  }
}

void bitmap_free(bitmap *b) {
  free(b->map);
  free(b);
}

u32 bitmap2array(bitmap *b, u32 *array) {
  u32 n = 0;

  for (u32 i = 0; i < b->capacity; i += 1) {
    if (bitmap_test(b, i)) {
      array[n++] = i;
    }
  }

  return n;
}

void array2bitmap(u32 *array, bitmap *b, u32 n) {
  for (u32 i = 0; i < n; i += 1) {
    bitmap_set(b, array[i]);
  }
}
