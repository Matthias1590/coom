#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "maths.h"

bool io_init(void);
bool io_update(void);

typedef enum {
    KEY_NONE = 0,
    KEY_W = 17,
    KEY_A = 30,
    KEY_S = 31,
    KEY_D = 32,
} io_key_t;

uint16_t get_current_time(void);
bool is_key_pressed(io_key_t key);

void update_screen(void);
void set_color(uint8_t color);
void draw_rect(v2i_t top_left, v2i_t size);
void putchar(char c);
void putnbr(int16_t x);
