OUTPUT_FORMAT(binary)
OUTPUT_ARCH(i386)

SECTIONS
{
    . = 0x1000;
    .start : { *(.start) }
    .text : { *(.text) }
    .data : { *(.data) }
    .bss : { *(.bss) }

    . = ALIGN(0x1000);  /* Align the address to the next 0x1000 boundary */
    .backbuffer : {
        backbuffer = .;  /* Mark the start address of the back buffer */
        . += 320 * 200;  /* Reserve 64,000 bytes for the back buffer */
    }
}
