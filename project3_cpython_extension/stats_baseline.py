def _values(data):
    if not isinstance(data, (list, tuple)):
        raise TypeError("Expected a sequence of numbers")
    if not data:
        raise ValueError("Dataset cannot be empty")
    if any(not isinstance(value, (int, float)) or isinstance(value, bool)
           for value in data):
        raise TypeError("All elements must be numerical")
    return data


def calculate_mean(data):
    values = _values(data)
    return sum(values) / len(values)


def calculate_variance(data):
    values = _values(data)
    mean = sum(values) / len(values)
    return sum((value - mean) ** 2 for value in values) / len(values)


def find_min_max(data):
    values = _values(data)
    return min(values), max(values)


def count_outliers(data, threshold):
    values = _values(data)
    return sum(1 for value in values if abs(value) > threshold)


def normalize_data(data):
    values = _values(data)
    minimum, maximum = min(values), max(values)
    value_range = maximum - minimum
    if value_range == 0:
        return [0.0] * len(values)
    return [(value - minimum) / value_range for value in values]
