# Signal Service Explanation

`server_controller` installs one `sigaction` disposition for each required
signal. The handler only assigns `volatile sig_atomic_t` flags; it never calls
stdio, allocation, or other non-async-signal-safe functions. The main loop
consumes those flags and performs all output and state transitions in ordinary
thread context.

`SIGUSR1` prints the PID and current state. Each `SIGUSR2` toggles the state
between `NORMAL` and `MAINTENANCE`, which changes the heartbeat text. `SIGINT`
prints cleanup messages and returns success, representing a graceful shutdown.
`SIGTERM` is treated as urgent: the loop writes an async-signal-safe emergency
message and returns failure. The captured `terminal_logs.txt` demonstrates all
four signals, both state transitions, a maintenance status report, and both
shutdown paths.
