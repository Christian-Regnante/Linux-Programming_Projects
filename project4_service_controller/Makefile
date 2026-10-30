CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

.PHONY: all build test clean

all: test

build: server_controller

server_controller: server_controller.c
	$(CC) $(CFLAGS) $< -o $@

test: build
	./test_signals.sh

clean:
	rm -f server_controller terminal_logs.txt
