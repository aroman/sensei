//
//  FaceDetector.cpp
//  ClassDemo
//
//  Created by Avi Romanoff on 11/12/16.
//
//

#include "FaceDetector.hpp"
#include "LandmarkCoreIncludes.h"

/**
 * @brief   Non overlaping detections
 *
 * Go over the model and eliminate detections that are not informative (there already is a tracker there)
 *
 * @param 		  	models          The clnf models.
 * @param [in,out]	face_detections	The face detections.
 */
void NonOverlapingDetections(const vector<LandmarkDetector::CLNF>& models, vector<cv::Rect_<double> >& face_detections) {
    for (size_t model = 0; model < models.size(); ++model) {
        // See if the detections intersect
        cv::Rect_<double> model_rect = models[model].GetBoundingBox();
        for (int detection = face_detections.size() - 1; detection >= 0; --detection) {
            double intersection_area = (model_rect & face_detections[detection]).area();
            double union_area = model_rect.area() + face_detections[detection].area() - 2 * intersection_area;
            // If the model is already tracking what we're detecting ignore the detection, this is determined by amount of overlap
            if (intersection_area / union_area > 0.5) {
                face_detections.erase(face_detections.begin() + detection);
            }
        }
    }
}

void FaceDetector::updateImage(cv::Mat grayscaleImage) {
//    mutex.lock();
    grayscaleImage.copyTo(matGrayscale);
    isImageDirty = true;
//    mutex.unlock();
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


//    // Keep only non overlapping detectionsb
//    NonOverlapingDetections(models, faces_detected);

}
