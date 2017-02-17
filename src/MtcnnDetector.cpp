#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>
#include "MtcnnDetector.h"

MtcnnDetector::MtcnnDetector() {
  Py_SetProgramName((char*)"mtcnn-bridge");
  Py_Initialize();
  import_array();

  PyObject *pModule, *pModuleName;

  pModuleName = PyString_FromString((char*)"mtcnn_runner");
  pModule = PyImport_Import(pModuleName);
  Py_DECREF(pModuleName);
  if (pModule == NULL) {
    PyErr_SetString(PyExc_TypeError, "could not import mtcnn_runner module");
    PyErr_Print();
  }
  pDetectFunc = PyObject_GetAttrString(pModule,(char*)"detect");
  if (pDetectFunc == NULL || !PyCallable_Check(pDetectFunc)) {
      PyErr_SetString(PyExc_TypeError, "detect function is not callable");
      PyErr_Print();
  }
  Py_DECREF(pModule);
}

MtcnnDetector::~MtcnnDetector() {
  Py_XDECREF(pDetectFunc);
  Py_Finalize();
}

vector<mtcnn_face_bbox> MtcnnDetector::detectFaces(const cv::Mat& mat) {
    vector<mtcnn_face_bbox> faces;
    PyObject *pValue, *pArray, *pArgs;

    npy_intp dims[] = {1080, 1920, 3};
    pArray = PyArray_SimpleNewFromData(3, dims, NPY_UINT8, mat.data);
    pArgs = Py_BuildValue("(O)", pArray);
    // Py_BEGIN_ALLOW_THREADS
    pValue = PyObject_CallObject(pDetectFunc, pArgs);
    if (PyErr_Occurred()) {
      PyErr_Print();
    }
    Py_DECREF(pArgs);
    Py_DECREF(pArray);

    if (pValue != NULL) {
        for (int i = 0; i < PyList_Size(pValue); i++) {
          PyObject *curItem = PyList_GetItem(pValue, i);
          mtcnn_face_bbox face;
          face.x1 = PyFloat_AsDouble(PyList_GetItem(curItem, 0));
          face.y1 = PyFloat_AsDouble(PyList_GetItem(curItem, 1));
          face.x2 = PyFloat_AsDouble(PyList_GetItem(curItem, 2));
          face.y2 = PyFloat_AsDouble(PyList_GetItem(curItem, 3));
          face.score = PyFloat_AsDouble(PyList_GetItem(curItem, 4));
          faces.push_back(face);
          // Py_DECREF(pValue);
        }
    }
    Py_XDECREF(pValue);

    // Py_END_ALLOW_THREADS
    return faces;
}
