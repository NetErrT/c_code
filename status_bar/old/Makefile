CHECKS=-pedantic -Wall -Wextra
VARIABLES=-D_POSIX_C_SOURCE=200809L
LIBS=$(shell pkg-config --libs --cflags x11 alsa) -lm -lpthread
FILES=main.c utils.c components/keyboard.c components/audio.c components/clock.c components/brightness.c

CC=clang
CFLAGS=-std=c99 -O2 $(CHECKS) $(VARIABLES) $(LIBS)

build:
	$(CC) $(CFLAGS) -o main.o $(FILES)

debug: CFLAGS += -fsanitize=address -g
debug:
	$(CC) $(CFLAGS) -o debug.o $(FILES)

clang:
	bear -- make build
	clang-tidy -checks="clang-analyzer-*,bugprone-*,concurrency-*,llvm-*,performance-*" --format-style=llvm -dump-config > .clang-tidy

clean:
	rm -f main.o debug.o
