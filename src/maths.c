#include "maths.h"

#include <stdint.h>
#include "bios_math.h"

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

v2f_t v2f_from_angle(rad_t angle) {
    float a = cosf(angle);
    float b = sinf(angle);
    return V2F(a, b);
    // return V2F(cosf(angle), sinf(angle));
}

float v2f_to_angle(v2f_t vec) {
    return atan2f(vec.y, vec.x);
}

float v2f_get_slope(v2f_t vec) {
    if (vec.x == 0) {
        return INFINITY;
    }
    return vec.y / vec.x;
}

float v2f_squared_distance(v2f_t vec_a, v2f_t vec_b) {
    return fabsf(v2f_squared_length(v2f_sub(vec_a, vec_b)));
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

static int16_t float_to_int_floor(float f) {
    int16_t result;

    return f;
    
    // Inline assembly to floor float to integer
    asm volatile (
        "fld %1 \n"         // Load float value into FPU stack
        "frndint \n"        // Round towards nearest integer (truncate decimals)
        "fistp %0 \n"       // Store integer result from FPU stack to result variable
        : "=m" (result)     // Output: result as integer
        : "m" (f)           // Input: f as float
        : "st"              // Clobbers FPU stack
    );

    return result;
}

v2i_t v2f_floor_to_v2i(v2f_t vec) {
    int16_t x = float_to_int_floor(vec.x);
    int16_t y = float_to_int_floor(vec.y);
    return V2I(x, y);
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
