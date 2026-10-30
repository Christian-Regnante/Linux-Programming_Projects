.PHONY: all build test benchmark clean

all: build test benchmark

build:
	python3 setup.py build_ext --inplace

test: build
	python3 test_correctness.py

benchmark: build
	python3 benchmark.py

clean:
	rm -rf build fast_stats*.so __pycache__ benchmark_results.md
