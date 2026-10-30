#!/bin/bash
set -eu

mkdir -p traces
gcc -Wall -Wextra -pedantic -std=c99 backup_tool.c -o backup_tool

printf '%s\n' "Sample production payload data for backup test." > source.txt
rm -rf backup_dir
strace -f -s 128 -o traces/backup_success_trace.txt ./backup_tool source.txt

rm -rf backup_dir
strace -f -s 128 -o traces/backup_failed_trace.txt ./backup_tool non_existent_file.txt || true
