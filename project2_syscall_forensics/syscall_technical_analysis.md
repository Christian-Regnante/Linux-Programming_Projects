# Technical System-call Analysis

## Kernel interaction pipeline

In the successful trace, the parent creates `backup_dir` at line 31 and opens
`backup_dir/backup.log` at line 32. After the child is created, lines 44–47
show `openat("source.txt", O_RDONLY) = 3`, creation of the destination as fd
4, a 48-byte `read`, and a matching 48-byte `write`. Lines 49–50 close both
descriptors. The parent then appends completion status to the log.

## Process and synchronization evidence

The parent `clone` at line 36 returns child PID 47415 and its `wait4` resumes
at line 52 with `WIFEXITED` and status 0. The child exits through
`exit_group(0)` at line 50. This demonstrates that the parent does not report
success until the worker has completed. The failure trace repeats the same
structure with child PID 47420 and status 1.

## Success versus failure

Experiment A copies the source and reports success. In Experiment B, line 43
shows `openat(..., "non_existent_file.txt", O_RDONLY) = -1 ENOENT`; the child
immediately exits with status 1 at line 44, never opens a destination or
performs a data `read`/`write`. The parent observes that status at line 46,
logs failure, and exits 1. This prevents a false successful copy, although the
initial log directory and failure record are still created.

## Other calls and reliability

The traces also include `execve`, loader `openat`/`read`, memory setup calls
such as `brk` and `mmap`, and `set_tid_address` before application work. These
are runtime initialization rather than backup logic. A production version
should write to a temporary destination, call `fsync`, and use `renameat`
atomically so a crash cannot leave a partially written destination reported as
complete.
