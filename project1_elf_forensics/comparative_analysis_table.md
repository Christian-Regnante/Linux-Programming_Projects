# ELF Comparative Analysis

The binaries were compiled and inspected on this host. `run_forensics.sh` contains
the complete static command set; neither target binary was executed.

| Property | `data_sync` | `data_sync_v2` | Technical cause and evidence |
| :--- | :--- | :--- | :--- |
| ELF class and architecture | ELF64, x86-64 | ELF64, x86-64 | `file` and `readelf -h` agree. |
| ELF file type | `EXEC`; entry `0x401090` | `DYN` PIE; entry `0x1100` | `-no-pie` versus `-pie -fPIE`; a PIE entry is an offset from its load base. |
| Main symbol | Present, unstripped | Present, unstripped | `nm -n` and `readelf -s`. |
| Important sections | `.text`, `.rodata`, `.data`, `.bss`, `.dynsym`, `.symtab` | Same plus `.debug_aranges`, `.debug_info`, `.debug_abbrev`, `.debug_line`, `.debug_str` | `-g` adds DWARF sections to v2. Sections describe link-time regions; program segments describe runtime mappings. |
| Dynamic dependency | `libc.so.6` | `libc.so.6` | `readelf -d` shows `DT_NEEDED` for both. |
| Relevant imported functions | `opendir`, `readdir`, `closedir`, `strcmp`, `printf`, `perror` | Those plus `stat`, `fopen`, `fprintf`, `fclose`, `snprintf`, `time` | `nm -D`/`objdump -T` show undefined dynamic references. |
| Debugging information | None reported by `file` | DWARF `debug_info` reported by `file` and sections | The v2 build used `-g`. |
| Functional difference | Directory traversal and reporting | Metadata checks, DJB2 name hashes, append-only audit logging | Source and import evidence show the expanded behavior; imports alone do not prove every path executes. |

Both binaries are dynamically linked rather than statically linked. An undefined
symbol is a reference unresolved in the executable's own symbol table; a dynamic
import is the runtime-resolved form of such a reference, and `libc.so.6` is the
shared-library provider.
