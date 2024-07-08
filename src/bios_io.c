#include "io.h"

#include "bios_math.h"

#define SCREEN_HEIGHT 200
#define SCREEN_WIDTH 320

static void draw_pixel(v2i_t position);

bool io_init(void) {
    asm volatile (
        "mov $0x13, %%ax\n"
        "int $0x10\n"
        :
        :
        : "%ax"
    );

    return true;
}

bool io_update(void) {
    return true;
}

uint16_t get_current_time(void) {
    uint16_t time;

    asm volatile (
        "xor %%ax, %%ax\n"
        "int $0x1a\n"
        "mov %%dx, %%ax\n"
        "mov %%cx, %%dx\n"
        "mov %%ax, %0\n"
        : "=g"(time)
        :
        : "%dx", "%ax", "%cx"
    );

    return time;
}

uint8_t get_pressed_key(void) {
    return KEY_NONE;
}

extern uint8_t backbuffer[];

void update_screen(void) {
    uint8_t *video_memory = (uint8_t *)0x9ff00;
    for (uint16_t i = 0; i < 320 * 200; ++i) {
        video_memory[i] = backbuffer[i];
    }

    set_color(0);
    draw_rect(V2I(0, 0), V2I(SCREEN_WIDTH, SCREEN_HEIGHT));
}

static uint8_t color = 15;

void set_color(uint8_t _color) {
    color = _color;
}

static void draw_pixel(v2i_t position) {
    if (position.x < 0 || position.x >= SCREEN_WIDTH || position.y < 0 || position.y >= SCREEN_HEIGHT) {
        return;
    }

    backbuffer[position.y * SCREEN_WIDTH + position.x] = color;
}

void draw_rect(v2i_t top_left, v2i_t size) {
    if (top_left.x >= SCREEN_WIDTH || top_left.x + size.x < 0 || top_left.y >= SCREEN_HEIGHT || top_left.y + size.y < 0) {
        return;
    }

    if (top_left.x < 0) {
        size.x -= absi(top_left.x);
        top_left.x = 0;
    }
    if (top_left.y < 0) {
        size.y -= absi(top_left.y);
        top_left.y = 0;
    }

    for (int16_t y = 0; y < size.y; y++) {
        for (int16_t x = 0; x < size.x; x++) {
            draw_pixel(v2i_add(top_left, V2I(x, y)));
        }
    }
}

void putchar(char c) {
    asm volatile (
        "mov $0x0e, %%ah\n"
        "mov %0, %%al\n"
        "mov $0x00, %%bh\n"
        "mov $0x07, %%bl\n"
        "int $0x10\n"
        :
        : "g"(c)
        : "%al", "%ah", "%bh", "%bl"
    );
}

void putnbr(int16_t x) {
    if (x < 0) {
        putchar('-');
        x = -x;
    }
    if (x > 10) {
        putnbr(x / 10);
    }
    putchar('0' + (x % 10));
}
