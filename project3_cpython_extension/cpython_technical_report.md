# CPython Extension Technical Report

`fast_stats.c` exposes five `METH_VARARGS` functions through a
`PyModuleDef`; `setup.py` builds them into an importable ABI-tagged shared
object with `python3 setup.py build_ext --inplace`. Each call converts the
input sequence to a temporary contiguous `double` array, validates numeric
items, performs the arithmetic in C, and releases allocated memory and Python
references on every path. Empty inputs raise `ValueError`, invalid sequences
or items raise `TypeError`, and constant-valued normalization returns zeros.

`test_correctness.py` passed for single-element, mixed-sign, 2,001-element, and
invalid inputs. The measured five-run means in `benchmark_results.md` show
approximately 21–35x speedups at one million values for mean, variance,
min/max, and outlier counting. Normalization reaches 6.28x because the C
extension must still allocate and populate one Python float per output item.
Small inputs include fixed conversion/call overhead, while larger inputs make
the reduced Python bytecode dispatch more visible. The extension retains the
Python API but adds a compiled-module build dependency and a manual
reference/memory-management surface.
