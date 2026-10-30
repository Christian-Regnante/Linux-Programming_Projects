#!/bin/bash
set -eu

BINARIES=("data_sync" "data_sync_v2")
OUT="outputs"
mkdir -p "$OUT"

for bin in "${BINARIES[@]}"; do
    file "$bin" > "$OUT/file_${bin}.txt"
    readelf -h "$bin" > "$OUT/readelf_header_${bin}.txt"
    readelf -l "$bin" > "$OUT/readelf_segments_${bin}.txt"
    readelf -S "$bin" > "$OUT/readelf_sections_${bin}.txt"
    readelf -s "$bin" > "$OUT/readelf_symbols_${bin}.txt"
    readelf -d "$bin" > "$OUT/readelf_dynamic_${bin}.txt"
    nm -n "$bin" > "$OUT/nm_numeric_${bin}.txt"
    nm -D "$bin" > "$OUT/nm_dynamic_${bin}.txt"
    objdump -h "$bin" > "$OUT/objdump_headers_${bin}.txt"
    objdump -t "$bin" > "$OUT/objdump_symtab_${bin}.txt"
    objdump -T "$bin" > "$OUT/objdump_dynsym_${bin}.txt"
    objdump -d "$bin" > "$OUT/objdump_disasm_${bin}.txt"
    objdump -p "$bin" > "$OUT/objdump_private_${bin}.txt"
done
