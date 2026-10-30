# Linux Programming Formative Assignment 1

## Build & Submission Guide

### System Requirements
- Linux x86-64 (tested on GCC 16.2.1)
- GNU Binutils (`readelf`, `nm`, `objdump`, `file`)
- `strace` (for Project 2)
- Python 3.x with `setuptools` (for Project 3)
- `gcc` with C99 support

### Quick Start: Build Everything
```bash
cd /home/_regnante_/Projects/Linux_Programming/Formative_Assignment1
make all
```

### Individual Projects

```bash
# Project 1: ELF Binary Forensics
make project1
# Outputs: project1_elf_forensics/outputs/*.txt, comparative_analysis_table.md, forensic_assessment_report.md

# Project 2: System Call Forensics
make project2
# Outputs: project2_syscall_forensics/traces/*.txt, syscall_summary_table.md, syscall_technical_analysis.md

# Project 3: CPython C Extension
make project3
# Outputs: fast_stats.so, benchmark_results.md, cpython_technical_report.md

# Project 4: Signal Service Controller
make project4
# Outputs: terminal_logs.txt, signal_service_explanation.md
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

### Project 1: ELF Binary Forensics (5.0 pts)
Static analysis of two ELF binaries using `file`, `readelf`, `nm`, `objdump` — without executing the binaries. Compares `ET_EXEC` vs `ET_DYN` (PIE) binary structures, symbol tables, debug sections, and imported libraries.

### Project 2: System Call Forensics (4.0 pts)
A C backup simulator using `fork()`, `waitpid()`, POSIX file I/O. Traced under `strace -f` for both success and failure scenarios. Analyzes kernel-level syscall sequences including `clone`, `openat`, `read`, `write`, `wait4`.

### Project 3: CPython C Extension (5.0 pts)
High-performance statistical library implemented as a CPython C extension (`fast_stats.c`). Benchmarked against pure Python baseline across 1K, 100K, 1M float datasets. Achieves significant speedups through direct machine code execution and contiguous C memory.

### Project 4: Signal-Driven Service Controller (4.0 pts)
A Linux monitoring daemon using `sigaction()` and `volatile sig_atomic_t` flags to safely handle `SIGUSR1`, `SIGUSR2`, `SIGINT`, and `SIGTERM`. Demonstrates async-signal-safety principles and operational state machine design.
