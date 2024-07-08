#include "state.h"

#include "bios_math.h"
#include "maths.h"
#include "world.h"
#include "io.h"

#define SCREEN_HEIGHT 200
#define SCREEN_WIDTH 320

#define ROTATE_SPEED 20
#define MOVE_SPEED 100

#define TILE_SIZE 8

#define NEAR_CLIPPING_DISTANCE 0.01
#define FAR_CLIPPING_DISTANCE 10.0
#define FOV_DEG 75.0f

bool state_init(state_t *state) {
    // init io
    if (!io_init()) {
        return false;
    }

    // init player
    state->player.position = V2F(3.5, 3.5);
    state->player.rotation = DEG_TO_RAD(-150);

    // init world
    state->world.height = 20;
    state->world.width = 20;
    // state->world.tiles[0+1][0+1].type = TILE_COLORED;
    // state->world.tiles[0+1][0+1].as_colored.color = 210;
    // state->world.tiles[1+1][0+1].type = TILE_COLORED;
    // state->world.tiles[1+1][0+1].as_colored.color = 53;
    state->world.tiles[2+1][0+1].type = TILE_COLORED;
    state->world.tiles[2+1][0+1].as_colored.color = 112;

    return true;
}

static void get_clipping_plane(v2f_t camera, rad_t direction, float distance, v2f_t *left, v2f_t *right) {
    *left = v2f_add(camera, v2f_scale(v2f_from_angle(direction + -FOV_DEG / 2), distance));
    *right = v2f_add(camera, v2f_scale(v2f_from_angle(direction + FOV_DEG / 2), distance));
}

static float calculate_delta_time(state_t *state) {
    uint16_t current_frame_time = get_current_time();
    
    if (state->last_frame_time == 0) {
        state->last_frame_time = current_frame_time;
        return 0;
    }

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
    v2f_t current_pushed = v2f_add(current, v2f_scale(direction, EPS));
    while (world_contains_v2f(world, current_pushed))
    {
        v2i_t current_floored = v2f_floor_to_v2i(current_pushed);
        if (world_is_filled(world, current_floored))
        {
            // draw green
            set_color(50);
            draw_rect(v2f_floor_to_v2i(v2f_scale(current_pushed, TILE_SIZE)), V2I(1, 1));

            return RAY_HIT(current);
        }

        // draw purple
        set_color(5);
        draw_rect(v2f_floor_to_v2i(v2f_scale(current_pushed, TILE_SIZE)), V2I(1, 1));

        current = get_next_point_on_ray(current, direction, dir_slope);
        current_pushed = v2f_add(current, v2f_scale(direction, EPS));
    }

    return RAY_MISS();
}

static void draw_ray(const world_t *world, v2f_t start, v2f_t direction) {
    ray_hit_t hit = cast_ray(world, start, direction);
    if (!hit.hit) {
        return;
    }

    set_color(0);
    draw_rect(v2i_sub(v2f_floor_to_v2i(v2f_scale(hit.position, TILE_SIZE)), V2I(2, 2)), V2I(4, 4));
}

static void draw_world(const world_t *world) {
    for (int16_t y = 0; y < world->height; y++) {
        for (int16_t x = 0; x < world->width; x++) {
            tile_t tile = world->tiles[y][x];
            switch (tile.type) {
                case TILE_EMPTY: {
                    // do nothing
                } break;
                case TILE_COLORED: {
                    set_color(tile.as_colored.color);
                    draw_rect(V2I(x * TILE_SIZE + TILE_SIZE/4, y * TILE_SIZE + TILE_SIZE/4), V2I(TILE_SIZE/2 + 1, TILE_SIZE/2 + 1));
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

    uint8_t key = get_pressed_key();

    // rotate player
    float rotate_amount = 0;
    if (key == KEY_A) {
        rotate_amount -= 1;
    }
    if (key == KEY_D) {
        rotate_amount += 1;
    }
    rotate_amount = -1;
    state->player.rotation += rotate_amount * delta_time * ROTATE_SPEED;

    // move player
    float move_amount = 0;
    if (key == KEY_W) {
        move_amount += 1;
    }
    if (key == KEY_S) {
        move_amount -= 1;
    }
    // move_amount = 0.3;
    state->player.position = v2f_add(state->player.position, v2f_scale(v2f_from_angle(state->player.rotation), move_amount * delta_time * MOVE_SPEED));

    // draw screen
    draw_world(&state->world);

    // todo: figure out if and why we need a near and far clipping plane
    // todo: fix the inverse fisheye effect
    for (int16_t x = 0; x < SCREEN_WIDTH; x++) {
        rad_t direction = DEG_TO_RAD(wrap_degs(RAD_TO_DEG(state->player.rotation) + (x - SCREEN_WIDTH / 2) * (FOV_DEG / SCREEN_WIDTH)));
        v2f_t direction_vec = v2f_from_angle(direction);

        ray_hit_t hit = cast_ray(&state->world, state->player.position, direction_vec);
        if (!hit.hit) {
            continue;
        }

        float euclidean_distance = sqrtf(v2f_squared_distance(state->player.position, hit.position));
        float perpendicular_distance = euclidean_distance * cosf(direction - state->player.rotation);

        int16_t wall_height = ifloorf(SCREEN_HEIGHT / euclidean_distance);

        v2i_t tile_pos = v2f_floor_to_v2i(v2f_add(hit.position, v2f_scale(direction_vec, EPS)));
        tile_t tile = state->world.tiles[tile_pos.y][tile_pos.x];
        switch (tile.type) {
            case TILE_EMPTY: {
                // unreachable
            } break;
            case TILE_COLORED: {
                set_color(tile.as_colored.color);
                draw_rect(V2I(x, SCREEN_HEIGHT/2 - wall_height/2), V2I(1, wall_height));
                continue;
            } break;
        }
    }

    set_color(15);
    draw_rect(v2f_floor_to_v2i(v2f_scale(state->player.position, TILE_SIZE)), V2I(3, 3));
    draw_rect(v2f_floor_to_v2i(v2f_scale(v2f_add(state->player.position, v2f_from_angle(state->player.rotation)), TILE_SIZE)), V2I(1, 1));

    update_screen();

    return true;
}
