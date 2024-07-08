#pragma once

#include <stdint.h>
#include "bios_math.h"

#define TWO_PI 6.28318530718
#define EPS 0.00001

#define DEG_TO_RAD(a) \
    ((a)*(PI/180.0f))

#define RAD_TO_DEG(a) \
    ((a)*(180.0f/PI))

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
typedef float deg_t;

typedef struct {
    float x;
    float y;
} v2f_t;

typedef struct {
    int16_t x;
    int16_t y;
} v2i_t;

int16_t ifloorf(float x);
float signf(float x);
float minf(float a, float b);
deg_t wrap_degs(deg_t angle);

v2f_t v2f_from_angle(rad_t angle);
rad_t v2f_to_angle(v2f_t vec);
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
int16_t v2i_squared_length(v2i_t vec);
