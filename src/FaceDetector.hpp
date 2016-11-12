//
//  FaceDetector.hpp
//  ClassDemo
//
//  Created by Avi Romanoff on 11/12/16.
//
//

#ifndef FaceDetector_hpp
#define FaceDetector_hpp

#include <stdio.h>
#include "ofThread.h"
#include "ofxCv.h"

#endif /* FaceDetector_hpp */


class FaceDetector : public ofThread {

public:
//    FaceDetector();
    void threadedFunction();
    void updateImage(cv::Mat grayscaleImage);
    
    vector<cv::Rect_<double>> faces_detected;

private:
    bool isImageDirty;
    cv::Mat_<uchar> matGrayscale;

};
