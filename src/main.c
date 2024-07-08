#include "state.h"

static state_t state = {0};

void __attribute__((section(".start"))) main(void) {
    // mov ah, 0x0e    ; AH = 0x0e for teletype output
    // mov al, 'A'     ; AL = ASCII code for character 'A' (65)
    // mov bh, 0x00    ; BH = 0x00 for page number (usually 0)
    // mov bl, 0x07    ; BL = 0x07 for text attribute (white on black)
    // int 0x10        ; BIOS interrupt 0x10 for video services

    if (!state_init(&state)) {
        return;
    }

    while (state_update(&state))
        ;
}
