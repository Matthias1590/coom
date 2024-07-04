#include "state.h"

#include <math.h>
#include "maths.h"
#include "world.h"
#include "io.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define ROTATE_SPEED 5.0
#define MOVE_SPEED 10.0

#define TILE_SIZE 20.0

#define NEAR_CLIPPING_DISTANCE 0.01
#define FAR_CLIPPING_DISTANCE 10.0
#define FOV DEGREES(75)

bool state_init(state_t *state) {
    // init io
    if (!io_init()) {
        return false;
    }

    // init player
    state->player.position = V2F(3.5, 3.5);
    state->player.rotation = DEGREES(16);

    // init world
    state->world.height = 20;
    state->world.width = 20;
    state->world.tiles[0][0].type = TILE_COLORED;
    state->world.tiles[0][0].as_colored.r = 255;
    state->world.tiles[1][0].type = TILE_COLORED;
    state->world.tiles[1][0].as_colored.g = 255;
    state->world.tiles[2][0].type = TILE_COLORED;
    state->world.tiles[2][0].as_colored.b = 255;

    return true;
}

static void get_clipping_plane(v2f_t camera, rad_t direction, float distance, v2f_t *left, v2f_t *right) {
    *left = v2f_add(camera, v2f_scale(v2f_from_angle(direction + -FOV / 2), distance));
    *right = v2f_add(camera, v2f_scale(v2f_from_angle(direction + FOV / 2), distance));
}

static float calculate_delta_time(state_t *state) {
    uint64_t current_frame_time = get_current_time();
    
    float delta_time = (float)(current_frame_time - state->last_frame_time) / 1000.0f;

    state->last_frame_time = current_frame_time;

    return delta_time;
}

static v2f_t get_next_point_on_ray(v2f_t current, v2f_t direction, float dir_slope) {
    v2f_t nx = current;
    if (direction.x > 0)
    {
        nx.x = ceilf(current.x + EPS);
    }
    else
    {
        nx.x = floorf(current.x - EPS);
    }
    float dx = nx.x - current.x;
    nx.y += dx * dir_slope;

    v2f_t ny = current;
    if (direction.y > 0)
    {
        ny.y = ceilf(current.y + EPS);
    }
    else
    {
        ny.y = floorf(current.y - EPS);
    }
    float dy = ny.y - current.y;
    ny.x += dy / dir_slope;

    v2f_t n = ny;
    if (v2f_squared_distance(current, nx) < v2f_squared_distance(current, ny))
    {
        n = nx;
    }

    return n;
}

static ray_hit_t cast_ray(const world_t *world, v2f_t start, v2f_t direction) {
    float dir_slope = v2f_get_slope(direction);

    v2f_t current = start;
    while (world_contains_v2f(world, current))
    {
        v2i_t current_i = v2f_floor_to_v2i(v2f_add(current, v2f_scale(v2f_map(direction, signf), EPS)));
        if (world_is_filled(world, current_i))
        {
            return RAY_HIT(current);
        }

        current = get_next_point_on_ray(current, direction, dir_slope);
    }

    return RAY_MISS();
}

static void draw_ray(const world_t *world, v2f_t start, v2f_t direction) {
    ray_hit_t hit = cast_ray(world, start, direction);
    if (!hit.hit) {
        return;
    }

    set_color(0, 255, 0);
    draw_rect(v2i_sub(v2f_floor_to_v2i(v2f_scale(hit.position, TILE_SIZE)), V2I(2, 2)), V2I(4, 4));
}

static void draw_world(const world_t *world) {
    for (int y = 0; y < world->height; y++) {
        for (int x = 0; x < world->width; x++) {
            tile_t tile = world->tiles[y][x];
            switch (tile.type) {
                case TILE_EMPTY: {
                    // do nothing
                } break;
                case TILE_COLORED: {
                    set_color(tile.as_colored.r, tile.as_colored.g, tile.as_colored.b);
                    draw_rect(V2I(x * TILE_SIZE, y * TILE_SIZE), V2I(TILE_SIZE, TILE_SIZE));
                } break;
            }
        }
    }
}

bool state_update(state_t *state) {
    float delta_time = calculate_delta_time(state);

    if (!io_update()) {
        return false;
    }

    // rotate player
    int rotate_amount = 0;
    if (is_key_down(KEY_A)) {
        rotate_amount -= 1;
    }
    if (is_key_down(KEY_D)) {
        rotate_amount += 1;
    }
    state->player.rotation += rotate_amount * delta_time * ROTATE_SPEED;

    // move player
    int move_amount = 0;
    if (is_key_down(KEY_W)) {
        move_amount += 1;
    }
    if (is_key_down(KEY_S)) {
        move_amount -= 1;
    }
    state->player.position = v2f_add(state->player.position, v2f_scale(v2f_from_angle(state->player.rotation), move_amount * delta_time * MOVE_SPEED));

    // draw screen
    // todo: figure out why we need a near and far clipping plane
    // todo: fix the inverse fisheye effect
    // v2f_t near_plane_left;
    // v2f_t near_plane_right;
    // get_clipping_plane(state->player.position, state->player.rotation, NEAR_CLIPPING_DISTANCE, &near_plane_left, &near_plane_right);

    // v2f_t far_plane_left;
    // v2f_t far_plane_right;
    // get_clipping_plane(state->player.position, state->player.rotation, FAR_CLIPPING_DISTANCE, &far_plane_left, &far_plane_right);

    for (int x = 0; x < SCREEN_WIDTH; x++) {
        rad_t direction = state->player.rotation + (x - SCREEN_WIDTH / 2) * (FOV / SCREEN_WIDTH);
        v2f_t direction_vec = v2f_from_angle(direction);

        ray_hit_t hit = cast_ray(&state->world, state->player.position, direction_vec);
        if (!hit.hit) {
            continue;
        }

        float euclidian_distance = sqrtf(v2f_squared_distance(state->player.position, hit.position));
        float perpendicular_distance = euclidian_distance * cosf(direction - state->player.rotation);

        int wall_height = SCREEN_HEIGHT / perpendicular_distance;
        float shading = minf(1.0, 1.0 / perpendicular_distance * 2);

        v2i_t tile_pos = v2f_floor_to_v2i(v2f_add(hit.position, v2f_scale(v2f_map(direction_vec, signf), EPS)));
        tile_t tile = state->world.tiles[tile_pos.y][tile_pos.x];
        switch (tile.type) {
            case TILE_EMPTY: {
                // unreachable
            } break;
            case TILE_COLORED: {
                set_color(tile.as_colored.r * shading, tile.as_colored.g * shading, tile.as_colored.b * shading);
                draw_rect(V2I(x, -wall_height / 2 + SCREEN_HEIGHT / 2), V2I(1, wall_height));
            } break;
        }
    }

    update_screen();

    return true;
}
