#!/bin/bash
set -eu

gcc -Wall -Wextra -pedantic -std=c99 server_controller.c -o server_controller
: > terminal_logs.txt

./server_controller >> terminal_logs.txt 2>&1 &
PID=$!
sleep 1
echo "Testing SIGUSR1..." >> terminal_logs.txt
kill -SIGUSR1 "$PID"
sleep 1
echo "Testing SIGUSR2 (Switch to MAINTENANCE)..." >> terminal_logs.txt
kill -SIGUSR2 "$PID"
sleep 1
echo "Testing SIGUSR1 in MAINTENANCE..." >> terminal_logs.txt
kill -SIGUSR1 "$PID"
sleep 1
echo "Testing SIGUSR2 (Switch to NORMAL)..." >> terminal_logs.txt
kill -SIGUSR2 "$PID"
sleep 1
echo "Testing SIGINT (Graceful shutdown)..." >> terminal_logs.txt
kill -SIGINT "$PID"
wait "$PID" || true

echo "Testing SIGTERM on separate instance..." >> terminal_logs.txt
./server_controller >> terminal_logs.txt 2>&1 &
PID2=$!
sleep 1
kill -SIGTERM "$PID2"
wait "$PID2" || true

cat terminal_logs.txt
