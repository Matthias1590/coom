#include "maths.h"

#include <stdint.h>
#include "bios_math.h"

int16_t ifloorf(float x) {
    int16_t result;

    asm volatile (
        "fld %1\n"
        "frndint\n"
        "fistp %0\n"
        : "=m" (result)
        : "m" (x)
        : "st"
    );

    return result;
}

float signf(float x) {
    if (x < 0) {
        return -1.0f;
    }
    if (x > 0) {
        return 1.0f;
    }
    return 0.0f;
}

float minf(float a, float b) {
    if (a < b) {
        return a;
    }
    return b;
}

deg_t wrap_degs(deg_t angle) {
    while (angle >= 360.0f) {
        angle -= 360.0f;
    }
    while (angle < 0.0f) {
        angle += 360.0f;
    }
    return angle;
}

v2f_t v2f_from_angle(rad_t angle) {
    return V2F(cosf(angle), sinf(angle));
}

rad_t v2f_to_angle(v2f_t vec) {
    return atan2f(vec.y, vec.x);
}

float v2f_get_slope(v2f_t vec) {
    if (vec.x == 0) {
        return INFINITY;
    }
    return vec.y / vec.x;
}

float v2f_squared_distance(v2f_t vec_a, v2f_t vec_b) {
    return v2f_squared_length(v2f_sub(vec_a, vec_b));
}

float v2f_squared_length(v2f_t vec) {
    return vec.x * vec.x + vec.y * vec.y;
}

v2f_t v2f_map(v2f_t vec, float (*func)(float x)) {
    return V2F(func(vec.x), func(vec.y));
}

v2f_t v2f_scale(v2f_t vec, float amount) {
    return V2F(vec.x * amount, vec.y * amount);
}

v2f_t v2f_add(v2f_t vec_a, v2f_t vec_b) {
    return V2F(vec_a.x + vec_b.x, vec_a.y + vec_b.y);
}

v2f_t v2f_sub(v2f_t vec_a, v2f_t vec_b) {
    return V2F(vec_a.x - vec_b.x, vec_a.y - vec_b.y);
}

v2i_t v2f_floor_to_v2i(v2f_t vec) {
    return V2I(ifloorf(vec.x), ifloorf(vec.y));
}

v2i_t v2i_scale(v2i_t vec, float amount) {
    return V2I(vec.x * amount, vec.y * amount);
}

v2i_t v2i_add(v2i_t vec_a, v2i_t vec_b) {
    return V2I(vec_a.x + vec_b.x, vec_a.y + vec_b.y);
}

v2i_t v2i_sub(v2i_t vec_a, v2i_t vec_b) {
    return V2I(vec_a.x - vec_b.x, vec_a.y - vec_b.y);
}

int16_t v2i_squared_length(v2i_t vec) {
    return vec.x * vec.x + vec.y * vec.y;
}
