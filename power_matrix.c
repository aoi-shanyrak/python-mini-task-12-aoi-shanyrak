#define PY_SSIZE_T_CLEAN
#include <Python.h>

static PyObject* multy_matrix(PyObject* A, PyObject* B) {
  Py_ssize_t rows = PyList_Size(A);
  PyObject* result = PyList_New(rows);

  double sum;
  for (Py_ssize_t i = 0; i < rows; i++) {
    PyObject* row_a = PyList_GetItem(A, i);
    PyObject* new_row = PyList_New(rows);

    for (Py_ssize_t j = 0; j < rows; j++) {
      sum = 0;
      for (Py_ssize_t l = 0; l < rows; l++) {
        PyObject* a_item = PyList_GetItem(row_a, l);
        PyObject* b_item = PyList_GetItem(PyList_GetItem(B, l), j);

        double a_factor = PyFloat_AsDouble(a_item);
        double b_factor = PyFloat_AsDouble(b_item);

        sum += a_factor * b_factor;
      }
      PyList_SetItem(new_row, j, PyFloat_FromDouble(sum));
    }
    PyList_SetItem(result, i, new_row);
  }
  return result;
}

static PyObject* copy_matrix(PyObject* matrix) {
  Py_ssize_t rows = PyList_Size(matrix);
  PyObject* copy = PyList_New(rows);

  for (Py_ssize_t i = 0; i < rows; i++) {
    PyObject* row = PyList_GetItem(matrix, i);
    PyObject* copy_row = PyList_New(rows);

    for (Py_ssize_t j = 0; j < rows; j++) {
      PyObject* elem = PyList_GetItem(row, j);
      double val = PyFloat_AsDouble(elem);

      PyObject* elem_val = PyFloat_FromDouble(val);
      PyList_SetItem(copy_row, j, elem_val);
    }
    PyList_SetItem(copy, i, copy_row);
  }
  return copy;
}

static PyObject* powerMatrix(PyObject* self, PyObject* args) {
  PyObject* matrix;
  int power;

  if (!PyArg_ParseTuple(args, "Oi", &matrix, &power))
    return NULL;

  if (!PyList_Check(matrix))
    return NULL;
  Py_ssize_t rows = PyList_Size(matrix);
  if (rows == 0)
    return PyList_New(0);

  for (Py_ssize_t i = 0; i < rows; i++) {
    PyObject* row = PyList_GetItem(matrix, i);
    if ((!row) || (!PyList_Check(row)))
      return NULL;
    if (PyList_Size(row) != rows)
      return NULL;
    for (Py_ssize_t j = 0; j < rows; j++) {
      PyObject* elem = PyList_GetItem(row, j);
      if (!PyFloat_Check(elem))
        return NULL;
    }
  }
  if (power < 0)
    return NULL;
  if (power == 0) {
    PyObject* identity = PyList_New(rows);
    for (Py_ssize_t i = 0; i < rows; i++) {
      PyObject* row = PyList_New(rows);
      for (Py_ssize_t j = 0; j < rows; j++) {
        PyObject* elem = PyFloat_FromDouble((i == j) ? 1.0 : 0.0);
        PyList_SetItem(row, j, elem);
      }
      PyList_SetItem(identity, i, row);
    }
    return identity;
  }
  if (power == 1) {
    return copy_matrix(matrix);
  }
  PyObject* result;
  PyObject* old_temp;
  PyObject* temp = copy_matrix(matrix);

  size_t iterations = (size_t) power - 1;
  for (size_t i = 0; i < iterations; i++) {
    result = multy_matrix(temp, matrix);
    if (!result) {
      Py_DECREF(temp);
      return NULL;
    }
    old_temp = temp;
    temp = result;
    Py_DECREF(old_temp);
  }
  return temp;
}

static PyMethodDef matrixMethods[] = {
    {"pow",
     powerMatrix, METH_VARARGS,
     "Powering by number matrix as list with lists of floats"},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef power_matrix_module = {
    PyModuleDef_HEAD_INIT,
    "power_matrix",
    "returns a result of powering matrix by number",
    -1,
    matrixMethods};

PyMODINIT_FUNC PyInit_power_matrix(void) {
  return PyModule_Create(&power_matrix_module);
}
