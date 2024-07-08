bits 16

; the boot sector is loaded at 0x7c00
org 0x7c00

start:
    ; disable interrupts
    cli

    ; set segments
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; set up stack
    mov sp, 0x9000

    ; read kernel
    mov bx, 0x1000
    mov ah, 2
    mov al, 32
    mov dh, 0
    mov cl, 2
    mov ch, 0
    int 0x13

    ; jump to kernel
    jmp 0x10:0x1000

; boot signature
times 510 - ($ - $$) db 0
dw 0xaa55
