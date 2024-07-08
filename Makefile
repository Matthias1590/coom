# Tools
CC=cc
CWARNINGS=-Wall -Wextra -Werror -Wno-error=unused-parameter -Wno-error=unused-variable -Wno-error=unused-function
CFLAGS=-m16 -nostdlib -ffreestanding -fno-pie -ffloat-store -O0 $(CWARNINGS)
NASM=nasm
NASMFLAGS=-f bin
DD=dd
LD=ld

# Project
SRCDIR=src
SRCS=$(filter-out $(SRCDIR)/sdl_io.c, $(wildcard $(SRCDIR)/*.c))
BINDIR=bin
OBJS=$(patsubst $(SRCDIR)/%.c, $(BINDIR)/%.o, $(SRCS))
NAME=coom.img

all: $(NAME)

qemu: $(NAME)
	qemu-system-i386 -drive file=$(NAME),format=raw

$(NAME): bin/bootloader.bin bin/coom.bin
	$(DD) of=$@ if=/dev/zero          bs=32M count=1
	$(DD) of=$@ if=bin/bootloader.bin bs=512 count=1  conv=notrunc
	$(DD) of=$@ if=bin/coom.bin       bs=512 count=32 conv=notrunc seek=1

bin/coom.bin: src/coom.ld $(OBJS)
	$(LD) -T src/coom.ld -o $@ $(OBJS)

$(BINDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(BINDIR)
	$(CC) -c -o $@ $^ $(CFLAGS)

$(BINDIR)/%.bin: $(SRCDIR)/%.asm
	mkdir -p $(BINDIR)
	$(NASM) -o $@ $^

clean:
	rm -rf $(BINDIR)

fclean: clean
	rm -f $(NAME)
