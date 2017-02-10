#include <python2.7/Python.h>
#include <stdio.h>
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>
#include "mtcnn.h"

void py_bridge_init() {
  Py_SetProgramName("mtcnn-bridge");
  Py_Initialize();
  import_array();
}

vector<mtcnn_face_bbox> mxnet_detect(const cv::Mat& mat) {
    vector<mtcnn_face_bbox> detected_faces;
    PyObject *pModule, *pModuleName, *pDetectFunc, *pValue, *pArray, *pArgs;

    py_bridge_init();

    pModuleName = PyString_FromString((char*)"mtcnn_runner");
    pModule = PyImport_Import(pModuleName);
    Py_DECREF(pModuleName);
    if (pModule == NULL) {
      PyErr_SetString(PyExc_TypeError, "could not import module");
      PyErr_Print();
      return detected_faces;
    }
    pDetectFunc = PyObject_GetAttrString(pModule,(char*)"detect");
    npy_intp dims[] = {1080, 1920, 3};
    pArray = PyArray_SimpleNewFromData(3, dims, NPY_UINT8, mat.data);

    if (!PyCallable_Check(pDetectFunc)) {
        PyErr_SetString(PyExc_TypeError, "function is not callable?!");
        PyErr_Print();
        return detected_faces;
    }
    pArgs = Py_BuildValue("(O)", pArray);
    // Py_BEGIN_ALLOW_THREADS
    pValue = PyObject_CallObject(pDetectFunc, pArgs);
    Py_DECREF(pArgs);
    Py_DECREF(pDetectFunc);
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
          detected_faces.push_back(face);
          Py_DECREF(curItem);
        }
    }

    // Py_END_ALLOW_THREADS
    Py_DECREF(pModule);
    Py_Finalize();
    return detected_faces;
}
