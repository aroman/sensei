//
//  FaceDetector.cpp
//  ClassDemo
//
//  Created by Avi Romanoff on 11/12/16.
//
//

#include "FaceDetector.hpp"
#include "LandmarkCoreIncludes.h"

void FaceDetector::updateImage(cv::Mat grayscaleImage) {
    grayscaleImage.copyTo(matGrayscale);
    isImageDirty = true;
}

void FaceDetector::threadedFunction() {

    while(isThreadRunning()) {

        mutex.lock();
        if (isImageDirty) {
            vector<double> confidences;
            LandmarkDetector::DetectFacesHOG(faces_detected, matGrayscale, confidences);
        }
        mutex.unlock();
    }


//    // Keep only non overlapping detections
//    NonOverlapingDetections(models, faces_detected);

}
