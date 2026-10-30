from setuptools import Extension, setup


setup(
    name="fast_stats",
    version="1.0",
    description="CPython C extension for statistical operations",
    ext_modules=[
        Extension(
            "fast_stats",
            sources=["fast_stats.c"],
            extra_compile_args=["-O3", "-Wall", "-Wextra"],
        )
    ],
)
