CC=/usr/bin/clang
LDFLAGS=-L/lib 
CPPFLAGS=-I/include
CFLAGS=-std=c99 -Wall -Wextra -pedantic -fdiagnostics-color=always
CFLAGS+=$(CPPFLAGS) -O0 -g3 -Qunused-arguments # -DNDEBUG

OBJS=src/main.o src/flags.o src/filesystem.o src/errors.o src/filters.o src/context.o lib/MegaMimes.o
OUT=ftc

install: $(OBJS) $(OUT)
	$(CC) $(CFLAGS) $(OBJS) -c -o $(OUT)

src/.c.o:
	$(CC) -c $< -o $@

lib/MegaMimes.o: lib/MegaMimes.h lib/MegaMimes.c

ftc: src/main.o
	$(CC) $(CFLAGS) $(OBJS) -o $(OUT)

clean:
	rm -rf $(OBJS) $(OUT)

.PHONY: clean
