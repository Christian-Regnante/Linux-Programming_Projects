# Forensic Assessment

## Purpose and methodology

This assessment compares two controlled ELF builds advertised as synchronization
utilities. Static inspection used `file`, `readelf`, `nm`, and `objdump`; the
binaries were not executed. Raw output is retained under `outputs/`.

## Key findings

Both files are dynamically linked ELF64 x86-64 executables with `libc.so.6` as
their declared dependency. `data_sync` is `ET_EXEC` with entry `0x401090`,
consistent with the requested non-PIE build. `data_sync_v2` is `ET_DYN` PIE with
entry `0x1100`, suitable for ASLR relocation. Both retain `main` and their
regular symbol tables. Only v2 contains DWARF `.debug_*` sections, matching its
`-g` compilation.

## Comparative assessment

The dynamic symbol evidence matches the source-level expansion. `data_sync`
imports directory traversal, string comparison, reporting, and error APIs.
V2 additionally imports `stat`, `fopen`, `fprintf`, `fclose`, `time`, and
`snprintf`; its source contains a DJB2 hash and appends records to
`sync_audit.log`. This is an observable capability for metadata inspection and
disk logging, not proof that every invocation will write or that the log is
malicious. The extra code and debug sections explain the larger structural
surface.

## Conclusions and limitations

Static evidence confirms a functional and linking expansion in v2 and the
different PIE/debug build modes. It cannot establish runtime reachability for
all branches, data integrity, network behavior, or environmental triggers.
Dynamic execution in an isolated sandbox would be required for those questions.
