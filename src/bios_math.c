#include "bios_math.h"

#include <stdint.h>

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

int16_t absi(int16_t x) {
    if (x < 0) {
        return -x;
    }
    return x;
}

float fabsf(float x) {
    if (x < 0) {
        return -x;
    }
    return x;
}

float cosf(float x) {
    // Reduce x to the range [-pi, pi]
    while (x > PI) x -= 2.0f * PI;
    while (x < -PI) x += 2.0f * PI;

    // Polynomial approximation for cos(x) around 0
    float x2 = x * x;
    return 1.0f - x2 / 2.0f + x2 * x2 / 24.0f;
}

float sinf(float x) {
    // Reduce x to the range [-pi, pi]
    while (x > PI) x -= 2.0f * PI;
    while (x < -PI) x += 2.0f * PI;

    // Polynomial approximation for sin(x) around 0
    float x2 = x * x;
    return x * (1.0f - x2 / 6.0f);
}

float atan2f(float y, float x) {
    // Handle special cases
    if (x == 0.0f) {
        if (y > 0.0f) return PI / 2.0f;
        if (y < 0.0f) return -PI / 2.0f;
        return 0.0f; // atan2f(0, 0) is typically 0
    }

    float atan;
    float z = y / x;

    // Approximate atan(z) using a simple polynomial approximation
    if (fabsf(z) < 1.0f) {
        atan = z / (1.0f + 0.28f * z * z);
        if (x < 0.0f) {
            if (y < 0.0f) return atan - PI;
            return atan + PI;
        }
    } else {
        atan = PI / 2.0f - z / (z * z + 0.28f);
        if (y < 0.0f) return atan - PI;
    }

    return atan;
}

float ceilf(float x) {
    int16_t xi = float_to_int_floor(x);
    float result = (float)xi;
    
    if (result < x) {
        result += 1.0f;
    }
    
    return result;
}

float floorf(float x) {
    int16_t xi = float_to_int_floor(x);
    float result = (float)xi;
    
    if (result > x) {
        result -= 1.0f;
    }
    
    return result;
}

float sqrtf(float x) {
    if (x == 0.0f || x == 1.0f)
        return x;
    
    float guess = x / 2.0f;
    float prev_guess = 0.0f;
    
    while (guess != prev_guess) {
        prev_guess = guess;
        guess = (guess + x / guess) / 2.0f;
    }
    
    return guess;
}
