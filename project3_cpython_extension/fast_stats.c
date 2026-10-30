#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <math.h>
#include <stdlib.h>

static int extract_doubles(PyObject *sequence, double **values, Py_ssize_t *length)
{
    PyObject *fast;
    Py_ssize_t index;
    Py_ssize_t count;
    double *array;

    if (!PySequence_Check(sequence)) {
        PyErr_SetString(PyExc_TypeError, "Expected a sequence of numbers");
        return 0;
    }
    fast = PySequence_Fast(sequence, "Expected a sequence of numbers");
    if (fast == NULL) {
        return 0;
    }
    count = PySequence_Fast_GET_SIZE(fast);
    if (count == 0) {
        Py_DECREF(fast);
        PyErr_SetString(PyExc_ValueError, "Dataset cannot be empty");
        return 0;
    }
    array = malloc((size_t)count * sizeof(*array));
    if (array == NULL) {
        Py_DECREF(fast);
        PyErr_NoMemory();
        return 0;
    }
    for (index = 0; index < count; ++index) {
        PyObject *item = PySequence_Fast_GET_ITEM(fast, index);
        if ((!PyFloat_Check(item) && !PyLong_Check(item)) || PyBool_Check(item)) {
            free(array);
            Py_DECREF(fast);
            PyErr_SetString(PyExc_TypeError, "All elements must be numerical");
            return 0;
        }
        array[index] = PyFloat_AsDouble(item);
        if (PyErr_Occurred()) {
            free(array);
            Py_DECREF(fast);
            return 0;
        }
    }
    Py_DECREF(fast);
    *values = array;
    *length = count;
    return 1;
}

static PyObject *py_calculate_mean(PyObject *self, PyObject *args)
{
    PyObject *sequence;
    double *values;
    Py_ssize_t index, length;
    double sum = 0.0;

    (void)self;
    if (!PyArg_ParseTuple(args, "O", &sequence)
        || !extract_doubles(sequence, &values, &length)) {
        return NULL;
    }
    for (index = 0; index < length; ++index) {
        sum += values[index];
    }
    free(values);
    return PyFloat_FromDouble(sum / (double)length);
}

static PyObject *py_calculate_variance(PyObject *self, PyObject *args)
{
    PyObject *sequence;
    double *values;
    Py_ssize_t index, length;
    double sum = 0.0;
    double mean;
    double variance = 0.0;

    (void)self;
    if (!PyArg_ParseTuple(args, "O", &sequence)
        || !extract_doubles(sequence, &values, &length)) {
        return NULL;
    }
    for (index = 0; index < length; ++index) {
        sum += values[index];
    }
    mean = sum / (double)length;
    for (index = 0; index < length; ++index) {
        double difference = values[index] - mean;
        variance += difference * difference;
    }
    free(values);
    return PyFloat_FromDouble(variance / (double)length);
}

static PyObject *py_find_min_max(PyObject *self, PyObject *args)
{
    PyObject *sequence;
    double *values;
    Py_ssize_t index, length;
    double minimum, maximum;

    (void)self;
    if (!PyArg_ParseTuple(args, "O", &sequence)
        || !extract_doubles(sequence, &values, &length)) {
        return NULL;
    }
    minimum = maximum = values[0];
    for (index = 1; index < length; ++index) {
        if (values[index] < minimum) {
            minimum = values[index];
        }
        if (values[index] > maximum) {
            maximum = values[index];
        }
    }
    free(values);
    return Py_BuildValue("(dd)", minimum, maximum);
}

static PyObject *py_count_outliers(PyObject *self, PyObject *args)
{
    PyObject *sequence;
    double threshold;
    double *values;
    Py_ssize_t index, length, count = 0;

    (void)self;
    if (!PyArg_ParseTuple(args, "Od", &sequence, &threshold)
        || !extract_doubles(sequence, &values, &length)) {
        return NULL;
    }
    for (index = 0; index < length; ++index) {
        if (fabs(values[index]) > threshold) {
            ++count;
        }
    }
    free(values);
    return PyLong_FromSsize_t(count);
}

static PyObject *py_normalize_data(PyObject *self, PyObject *args)
{
    PyObject *sequence;
    PyObject *result;
    double *values;
    Py_ssize_t index, length;
    double minimum, maximum, value_range;

    (void)self;
    if (!PyArg_ParseTuple(args, "O", &sequence)
        || !extract_doubles(sequence, &values, &length)) {
        return NULL;
    }
    minimum = maximum = values[0];
    for (index = 1; index < length; ++index) {
        if (values[index] < minimum) {
            minimum = values[index];
        }
        if (values[index] > maximum) {
            maximum = values[index];
        }
    }
    result = PyList_New(length);
    if (result == NULL) {
        free(values);
        return NULL;
    }
    value_range = maximum - minimum;
    for (index = 0; index < length; ++index) {
        double normalized = value_range == 0.0
            ? 0.0 : (values[index] - minimum) / value_range;
        PyObject *number = PyFloat_FromDouble(normalized);
        if (number == NULL) {
            Py_DECREF(result);
            free(values);
            return NULL;
        }
        PyList_SET_ITEM(result, index, number);
    }
    free(values);
    return result;
}

static PyMethodDef methods[] = {
    {"calculate_mean", py_calculate_mean, METH_VARARGS, "Calculate arithmetic mean"},
    {"calculate_variance", py_calculate_variance, METH_VARARGS, "Calculate population variance"},
    {"find_min_max", py_find_min_max, METH_VARARGS, "Find minimum and maximum values"},
    {"count_outliers", py_count_outliers, METH_VARARGS, "Count absolute outliers"},
    {"normalize_data", py_normalize_data, METH_VARARGS, "Min-max normalize a sequence"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    "fast_stats",
    "Statistical operations in C",
    -1,
    methods,
    NULL,
    NULL,
    NULL,
    NULL
};

PyMODINIT_FUNC PyInit_fast_stats(void)
{
    return PyModule_Create(&module);
}
