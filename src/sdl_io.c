#include "io.h"

#include <SDL2/SDL.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

bool io_init(void) {
    if (SDL_CreateWindowAndRenderer(320, 200, 0, &window, &renderer) < 0) {
        return false;
    }

    return true;
}

bool io_update(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return false;
        }
    }

    return true;
}

uint64_t get_current_time(void) {
    return SDL_GetTicks64();
}

bool is_key_down(io_key_t key) {
    const uint8_t *state = SDL_GetKeyboardState(NULL);

    int scancode = -1;
    switch (key) {
        case KEY_W: {
            scancode = SDL_SCANCODE_W;
        } break;
        case KEY_A: {
            scancode = SDL_SCANCODE_A;
        } break;
        case KEY_S: {
            scancode = SDL_SCANCODE_S;
        } break;
        case KEY_D: {
            scancode = SDL_SCANCODE_D;
        } break;
    }

    if (scancode == -1) {
        return false;
    }

    return state[scancode];
}

void update_screen(void) {
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void set_color(uint8_t r, uint8_t g, uint8_t b) {
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
}

void draw_rect(v2i_t top_left, v2i_t size) {
    SDL_Rect rect = {
        .x = top_left.x,
        .y = top_left.y,
        .w = size.x,
        .h = size.y,
    };

    SDL_RenderFillRect(renderer, &rect);
}
