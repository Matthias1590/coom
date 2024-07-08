#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "player.h"
#include "world.h"

#define RAY_HIT(_position) \
    ((ray_hit_t) { \
        .hit = true, \
        .position = (_position), \
    })

#define RAY_MISS() \
    ((ray_hit_t) { \
        .hit = false, \
    })

typedef struct {
    uint16_t last_frame_time;
    player_t player;
    world_t world;
} state_t;

typedef struct {
    bool hit;
    v2f_t position;
} ray_hit_t;

bool state_init(state_t *state);
bool state_update(state_t *state);
