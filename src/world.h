#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "maths.h"

#define MAX_WORLD_HEIGHT 64
#define MAX_WORLD_WIDTH 64

typedef enum {
    TILE_EMPTY,
    TILE_COLORED,
} tile_type_t;

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} tile_colored_t;

typedef struct {
    tile_type_t type;
    union {
        tile_colored_t as_colored;
    };
} tile_t;

typedef struct {
    tile_t tiles[MAX_WORLD_HEIGHT][MAX_WORLD_WIDTH];
    int height;
    int width;
} world_t;

bool world_contains_v2f(const world_t *world, v2f_t vec);
bool world_contains_v2i(const world_t *world, v2i_t vec);
bool world_is_filled(const world_t *world, v2i_t vec);
