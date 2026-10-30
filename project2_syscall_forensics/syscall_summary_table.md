# System-call Summary

Entries below are taken from the generated traces, not inferred from the
source. Runtime loader calls such as `openat` on `/etc/ld.so.cache` are present
alongside the backup operations.

| System call | Purpose | Relevant trace arguments | Return value/error | Role |
| :--- | :--- | :--- | :--- | :--- |
| `mkdir` | Create destination directory | `"backup_dir", 0755` | `0` in both runs | Initializes the backup area. |
| `openat` | Open/create files | `source.txt, O_RDONLY`; destination with `O_CREAT|O_TRUNC` | `3`, `4`; missing source `-1 ENOENT` | Opens input, output, and log. |
| `read` | Read source bytes | fd `3`, count `4096` | `48`, then `0` | Transfers source contents into user memory. |
| `write` | Persist bytes/log messages | fd `4`, count `48`; fd `3` for log | Matching byte counts | Writes destination and status records. |
| `close` | Release descriptors | fds `3` and `4` | `0` | Completes descriptor lifecycle. |
| `clone` | Create child worker | `flags=...|SIGCHLD` | Child PID `47415` (success), `47420` (failure) | Implements `fork()` through glibc. |
| `wait4` | Wait for child | PID `47415` or `47420` | Child PID and exit status | Parent synchronizes before reporting. |
| `exit_group` | Process termination | status `0` or `1` | Does not return | Child and parent publish completion status. |
| `execve` | Start the program | `"./backup_tool", ...` | `0` | Launches the traced executable. |
| `brk` / `mmap` | Runtime setup | libc loader allocations | Successful mappings/addresses | Initialization before application work. |
