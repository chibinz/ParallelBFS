#pragma once

#include <stdbool.h>

#include "types.h"

typedef struct {
  usize capacity;
  u32 *map;
} bitmap;

bitmap *bitmap_new(usize capacity);

void bitmap_set(bitmap *b, usize n);

bool bitmap_get(bitmap *b, usize n);

/// NOT ATOMIC
/// If nth bit already set return false,
/// otherwise set it to true and return true
bool bitmap_test_set(bitmap *b, usize n);

void bitmap_free(bitmap *b);
