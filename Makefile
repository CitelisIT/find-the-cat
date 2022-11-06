CC=/usr/bin/clang
LDFLAGS=-L/lib 
CPPFLAGS=-I/include
CFLAGS=-std=c99 -Wall -Wextra -pedantic -fdiagnostics-color=always
CFLAGS+=$(CPPFLAGS) -O0 -g3 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls -Qunused-arguments # -DNDEBUG
LDFLAGS+=-fsanitize=address

RUN_ENV=LSAN_OPTIONS=suppressions=lsan.ignore:print_suppressions=0 ASAN_OPTIONS=detect_leaks=1:symbolize=1 MallocNanoZone=0
#RUN_ENV=ASAN_OPTIONS=detect_leaks=1:symbolize=1
#RUN_ENV=

OBJS=src/flags.o src/filesystem.o src/errors.o src/main.o
OUT=ftc

install: $(OBJS) $(OUT)
	$(CC) $(CFLAGS) $(OBJS) -c -o $(OUT)

src/flags.o: src/flags.h src/flags.c 

src/filesystem.o: src/filesystem.h src/filesystem.c

src/errors.o: src/errors.h src/errors.c

src/main.o: src/main.c

ftc: src/main.o
	$(CC) $(CFLAGS) $(OBJS) -o $(OUT)

clean:
	rm -rf $(OBJS) $(OUT)

.PHONY: clean
