# Linux Programming Formative Assignment 1

## Build & Submission Guide

### System Requirements
- Linux x86-64
- GNU Binutils (`readelf`, `nm`, `objdump`, `file`)
- `strace` (for Project 2)
- Python 3.x with `setuptools` and Python development headers (for Project 3)
- `gcc` with C99 support

### Quick Start: Build Everything
```bash
make all
```

### Individual Projects

```bash
# Project 1: ELF Binary Forensics
make project1
# Outputs: project1_elf_forensics/outputs/*.txt,
#          project1_elf_forensics/comparative_analysis_table.md,
#          project1_elf_forensics/forensic_assessment_report.md

# Project 2: System Call Forensics
make project2
# Outputs: project2_syscall_forensics/traces/*.txt,
#          project2_syscall_forensics/syscall_summary_table.md,
#          project2_syscall_forensics/syscall_technical_analysis.md

# Project 3: CPython C Extension
make project3
# Outputs: project3_cpython_extension/fast_stats*.so,
#          project3_cpython_extension/benchmark_results.md,
#          project3_cpython_extension/cpython_technical_report.md

# Project 4: Signal Service Controller
make project4
# Outputs: project4_service_controller/terminal_logs.txt,
#          project4_service_controller/signal_service_explanation.md
```

### Verify All Deliverables
```bash
make test
```

### Clean All Artifacts
```bash
make clean
```

---

## Project Descriptions

### Project 1: ELF Binary Forensics
Static analysis of two controlled ELF file-synchronization utilities using `file`, `readelf`, `nm`, and `objdump`, without executing the binaries. The comparison covers `ET_EXEC` versus `ET_DYN` (PIE), entry points, sections and segments, symbols, debug information, dynamic dependencies, and the added metadata and audit-logging behavior in the second build.

### Project 2: System Call Forensics
A C backup simulator uses `mkdir()`, POSIX file I/O, `fork()`, and `waitpid()`. It is traced with `strace -f` for successful and missing-source scenarios, with analysis of the observed `clone`, `openat`, `read`, `write`, `close`, `wait4`, and `exit_group` calls.

### Project 3: CPython C Extension
Five statistical operations are implemented in a CPython C extension (`fast_stats.c`) and compared with a pure Python baseline. Correctness tests cover valid and invalid inputs, and benchmarks use 1,000, 100,000, and 1,000,000 float datasets. The C implementation reduces Python bytecode and object-processing overhead while still paying conversion and Python-list construction costs.

### Project 4: Signal-Driven Service Controller
A foreground Linux monitoring service uses `sigaction()` and `volatile sig_atomic_t` flags to safely handle `SIGUSR1`, `SIGUSR2`, `SIGINT`, and `SIGTERM`. It reports its PID and state, toggles between `NORMAL` and `MAINTENANCE`, emits periodic heartbeats, and demonstrates async-signal-safe signal handling and graceful versus emergency shutdown.
