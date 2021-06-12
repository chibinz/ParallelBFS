#pragma once

#include <stdatomic.h>
#include <stdbool.h>

#include "types.h"

typedef struct {
  usize capacity;
  atomic_uint *map;
} bitmap;

bitmap *bitmap_new(usize capacity);

void bitmap_set(bitmap *b, usize n);

bool bitmap_test(bitmap *b, usize n);

/// Atomically changes the state of a bit to set (true)
/// and returns the value it held before.
bool bitmap_test_set(bitmap *b, usize n);

void bitmap_free(bitmap *b);
