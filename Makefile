CC=/usr/bin/clang
LDFLAGS=-L/lib 
CPPFLAGS=-I/include
CFLAGS=-std=c99 -Wall -Wextra -Werror -Wno-error=unused-parameter -Wno-error=unused-but-set-variable -pedantic -fdiagnostics-color=always
CFLAGS+=$(CPPFLAGS) -O0 -g3 -Qunused-arguments # -DNDEBUG

OBJS=src/main.o src/flags.o src/filesystem.o src/errors.o src/filters.o src/context.o
OUT=ftc

install: $(OBJS) $(OUT)
	$(CC) $(CFLAGS) $(OBJS) -c -o $(OUT)

src/.c.o:
	$(CC) -c $< -o $@

ftc: src/main.o
	$(CC) $(CFLAGS) $(OBJS) -o $(OUT)

clean:
	rm -rf $(OBJS) $(OUT)

.PHONY: clean
