#include "state.h"

static state_t state = {0};

void __attribute__((section(".start"))) main(void) {
    if (!state_init(&state)) {
        return;
    }

    while (state_update(&state))
        ;
}
