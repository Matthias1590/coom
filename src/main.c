#include "state.h"

static state_t state = {0};

int main(void) {
    if (!state_init(&state)) {
        return 1;
    }

    while (state_update(&state))
        ;

    return 0;
}
