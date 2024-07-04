# Tools
LDFLAGS=-lm
CC=gcc
CFLAGS=-Wall -Wextra -Werror -Wno-error=unused-variable -Wno-error=unused-function `sdl2-config --cflags --libs` $(LDFLAGS)

# Project
SRCDIR=src
SRCS=$(wildcard $(SRCDIR)/*.c)
BINDIR=bin
OBJS=$(patsubst $(SRCDIR)/%.c, $(BINDIR)/%.o, $(SRCS))
NAME=$(BINDIR)/coom

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS) $(CFLAGS)

$(BINDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(BINDIR)
	$(CC) -c -o $@ $^ $(CFLAGS)

clean:
	rm -rf $(BINDIR) $(NAME)
