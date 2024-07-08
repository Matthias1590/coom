#pragma once

#include <stdint.h>
#include "bios_math.h"

#define TWO_PI 6.28318530718
#define EPS 0.00001

#define DEGREES(a) \
    ((a)*(PI/180))

#define V2F(_x, _y) \
    ((v2f_t) { \
        .x = (_x), \
        .y = (_y), \
    })

#define V2I(_x, _y) \
    ((v2i_t) { \
        .x = (_x), \
        .y = (_y), \
    })

typedef float rad_t;

typedef struct {
    float x;
    float y;
} v2f_t;

typedef struct {
    int16_t x;
    int16_t y;
} v2i_t;

float signf(float x);
float minf(float a, float b);

v2f_t v2f_from_angle(rad_t angle);
float v2f_to_angle(v2f_t vec);
float v2f_get_slope(v2f_t vec);
float v2f_squared_distance(v2f_t vec_a, v2f_t vec_b);
float v2f_squared_length(v2f_t vec);
v2f_t v2f_map(v2f_t vec, float (*func)(float x));
v2f_t v2f_scale(v2f_t vec, float amount);
v2f_t v2f_add(v2f_t vec_a, v2f_t vec_b);
v2f_t v2f_sub(v2f_t vec_a, v2f_t vec_b);
v2i_t v2f_floor_to_v2i(v2f_t vec);

v2i_t v2i_scale(v2i_t vec, float amount);
v2i_t v2i_add(v2i_t vec_a, v2i_t vec_b);
v2i_t v2i_sub(v2i_t vec_a, v2i_t vec_b);
