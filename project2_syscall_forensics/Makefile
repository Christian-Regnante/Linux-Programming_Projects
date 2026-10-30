CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

.PHONY: all build experiments clean

all: experiments

build: backup_tool

backup_tool: backup_tool.c
	$(CC) $(CFLAGS) $< -o $@

experiments: build
	./run_experiments.sh

clean:
	rm -f backup_tool source.txt
	rm -rf backup_dir traces
