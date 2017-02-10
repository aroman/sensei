#include "ofxCv.h"

struct mtcnn_face_bbox {
  double x1;
  double y1;
  double x2;
  double y2;
  double score;
};

vector<mtcnn_face_bbox> mxnet_detect(const cv::Mat& mat);
