#include "world.h"

bool world_contains_v2f(const world_t *world, v2f_t vec) {
    return vec.x >= 0 && vec.x <= world->width
        && vec.y >= 0 && vec.y <= world->height;
}

bool world_contains_v2i(const world_t *world, v2i_t vec) {
    return vec.x >= 0 && vec.x <= world->width
        && vec.y >= 0 && vec.y <= world->height;
}

bool world_is_filled(const world_t *world, v2i_t vec) {
    if (!world_contains_v2i(world, vec)) {
        return false;
    }

    return world->tiles[vec.y][vec.x].type != TILE_EMPTY;
}
