CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

.PHONY: all build forensics clean

all: build forensics

build: data_sync data_sync_v2

data_sync: data_sync.c
	$(CC) $(CFLAGS) -no-pie -fno-stack-protector $< -o $@

data_sync_v2: data_sync_v2.c
	$(CC) $(CFLAGS) -pie -fPIE -g $< -o $@

forensics: build
	./run_forensics.sh

clean:
	rm -f data_sync data_sync_v2 sync_audit.log
	rm -rf outputs
