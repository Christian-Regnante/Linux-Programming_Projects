import math

import fast_stats
import stats_baseline


FUNCTIONS = (
    ("calculate_mean", lambda module, data: module.calculate_mean(data)),
    ("calculate_variance", lambda module, data: module.calculate_variance(data)),
    ("find_min_max", lambda module, data: module.find_min_max(data)),
    ("count_outliers", lambda module, data: module.count_outliers(data, 2.0)),
    ("normalize_data", lambda module, data: module.normalize_data(data)),
)


def assert_equivalent(left, right):
    if isinstance(left, list):
        assert len(left) == len(right)
        assert all(math.isclose(a, b, rel_tol=1e-12, abs_tol=1e-12)
                   for a, b in zip(left, right))
    elif isinstance(left, tuple):
        assert all(math.isclose(a, b, rel_tol=1e-12, abs_tol=1e-12)
                   for a, b in zip(left, right))
    elif isinstance(left, float):
        assert math.isclose(left, right, rel_tol=1e-12, abs_tol=1e-12)
    else:
        assert left == right


def main():
    for data in ([1.0], [-3.5, 0.0, 4.5, 9.0],
                 [float(index) / 7.0 for index in range(-1000, 1001)]):
        for name, call in FUNCTIONS:
            assert_equivalent(call(stats_baseline, data), call(fast_stats, data))
    for module in (stats_baseline, fast_stats):
        for data, error in (([], ValueError), ([1, "bad"], TypeError), ("bad", TypeError)):
            for name, call in FUNCTIONS:
                try:
                    call(module, data)
                except error:
                    pass
                else:
                    raise AssertionError(f"{module.__name__}.{name} accepted invalid data")
    assert fast_stats.normalize_data([5.0, 5.0]) == [0.0, 0.0]
    print("Correctness tests passed: operations, large input, and edge cases agree.")


if __name__ == "__main__":
    main()
