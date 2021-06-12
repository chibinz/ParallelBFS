#pragma once

#include <stdatomic.h>
#include <stdbool.h>

#include "types.h"

typedef struct {
  u32 capacity;
  atomic_uint *map;
} bitmap;

bitmap *bitmap_new(u32 capacity);

void bitmap_set(bitmap *b, u32 n);

bool bitmap_test(bitmap *b, u32 n);

/// Atomically changes the state of a bit to set (true)
/// and returns the value it held before.
bool bitmap_test_set(bitmap *b, u32 n);

void bitmap_free(bitmap *b);
