#define PY_SSIZE_T_CLEAN
#include <Python.h>

static PyObject* powerMatrix(PyObject* self, PyObject* args) {
  double a, b;

  if (!PyArg_ParseTuple(args, "dd", &a, &b))
    return NULL;

  double result = a * b;
  return PyFloat_FromDouble(result);
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
    matrixMethods
};

PyMODINIT_FUNC PyInit_power_matrix(void) {
  return PyModule_Create(&power_matrix_module);
}
