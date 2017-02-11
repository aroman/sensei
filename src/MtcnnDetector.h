#include <python2.7/Python.h>
#include "ofxCv.h"

struct mtcnn_face_bbox {
  double x1;
  double y1;
  double x2;
  double y2;
  double score;
};

class MtcnnDetector {
    int width, height;
    PyObject *pDetectFunc;
  public:
    MtcnnDetector();
   ~MtcnnDetector();
    vector<mtcnn_face_bbox> detectFaces(const cv::Mat& mat);
};
