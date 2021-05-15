#include <stdatomic.h>
#include <stdbool.h>
#include <stdlib.h>

#include "bitmap.h"
#include "types.h"

bitmap *bitmap_new(usize capacity) {
  u32 *map = calloc((capacity / 32 + 1), sizeof(u32));
  bitmap *ret = malloc(sizeof(bitmap));

  *ret = (bitmap){capacity, map};

  return ret;
}

void bitmap_set(bitmap *b, usize n) {
  usize index = n / 32;
  usize offset = n % 32;
  b->map[index] |= 1 << offset;
}

bool bitmap_get(bitmap *b, usize n) {
  usize index = n / 32;
  usize offset = n % 32;
  return !!((b->map[index] >> offset) & 1);
}

bool bitmap_test_set(bitmap *b, usize n) {
  usize index = n / 32;
  usize offset = n % 32;
  bool prev = !!((b->map[index] >> offset) & 1);

  /// Guard to prevent unnecessary contention
  if (!prev) {
    prev = !!((atomic_fetch_or(&b->map[index], 1 << offset) >> offset) & 1);
  }

  return prev;
}

void bitmap_free(bitmap *b) {
  free(b->map);
  free(b);
}
