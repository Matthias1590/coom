#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "maths.h"

bool io_init(void);
bool io_update(void);

typedef enum {
    KEY_W,
    KEY_A,
    KEY_S,
    KEY_D,
} io_key_t;

uint64_t get_current_time(void);

bool is_key_down(io_key_t key);

void update_screen(void);
void set_color(uint8_t r, uint8_t g, uint8_t b);
void draw_rect(v2i_t top_left, v2i_t size);
