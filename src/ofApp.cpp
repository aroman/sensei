#include "ofApp.h"

#include <GazeEstimation.h>

#include "tbb/tbb.h"

using namespace boost::filesystem;

void drawBoundingBox(double x, double y, double width, double height, double thickness) {
    ofPath path;
    path.setFillColor(ofColor::blue);
    path.rectangle(x, y, width, height);
    path.rectangle(
                   x + (thickness / 2),
                   y + (thickness / 2),
                   width - thickness,
                   height - thickness);
    path.draw();
}



/**********************************************************************************************//**
 * @fn	void NonOverlapingDetections(const vector<LandmarkDetector::CLNF>& clnf_models, vector<cv::Rect_<double> >& face_detections)
 *
 * @brief	Non overlaping detections.
 *
 * @param 		  	clnf_models	   	The clnf models.
 * @param [in,out]	face_detections	The face detections.
 **************************************************************************************************/
void NonOverlapingDetections(const vector<LandmarkDetector::CLNF>& clnf_models, vector<cv::Rect_<double> >& face_detections)
{
    // Go over the model and eliminate detections that are not informative (there already is a tracker there)
    for (size_t model = 0; model < clnf_models.size(); ++model)
    {
        // See if the detections intersect
        cv::Rect_<double> model_rect = clnf_models[model].GetBoundingBox();
        for (int detection = face_detections.size() - 1; detection >= 0; --detection)
        {
            double intersection_area = (model_rect & face_detections[detection]).area();
            double union_area = model_rect.area() + face_detections[detection].area() - 2 * intersection_area;
            // If the model is already tracking what we're detecting ignore the detection, this is determined by amount of overlap
            if (intersection_area / union_area > 0.5)
            {
                face_detections.erase(face_detections.begin() + detection);
            }
        }
    }
}



//--------------------------------------------------------------
void ofApp::setup() {
    
    initialized = false;
    ofBackground(30, 30, 30);
    kinectFrameCounter = 0;
    
    // ofxKinect2 guarantees that device ID 0 will always refer to the same kinect
    // if there's only ever one plugged in
    // see: https://github.com/ofTheo/ofxKinectV2/blob/a536824/src/ofxKinectV2.h#L20
    int kinectId = 0;
    
    textFont.loadFont("../Resources/Hack-Regular.ttf", 28, true);
    
    kinect = new ofxKinectV2();
    
    // connect to the kinect. updates occur on a separate thread
    bool didOpenSuccessfully = kinect->open(kinectId);
    
    if (!didOpenSuccessfully) {
        std::exit(1);
    }

    int num_faces_max = 5;

    model_parameters.use_face_template = true;
    // This is so that the model would not try re-initialising itself
    model_parameters.reinit_video_every = -1;
    model_parameters.curr_face_detector = LandmarkDetector::FaceModelParameters::HOG_SVM_DETECTOR;
    model_parameters.model_location = "../Resources/model";
    model_parameters.face_detector_location = "../Resources/classifiers/haarcascade_frontalface_default.xml";
    
    det_parameters.push_back(model_parameters);
    
    LandmarkDetector::CLNF model(model_parameters.model_location);
    model.face_detector_HAAR.load(model_parameters.face_detector_location);
    model.face_detector_location = model_parameters.face_detector_location;
    
    models.reserve(num_faces_max);
    
    models.push_back(model);
    active_models.push_back(false);
    
    for (int i = 1; i < num_faces_max; ++i) {
        models.push_back(model);
        active_models.push_back(false);
        model_parameters.track_gaze = true;
        det_parameters.push_back(model_parameters);
    }
    
//     Grab camera parameters, if they are not defined (approximate values will be used)
//    LandmarkDetector::get_camera_params(d, fx, fy, cx, cy);
    

    vector<string> output_similarity_align;
    double sim_scale = 0.7;
    bool dynamic = true; // Indicates if a dynamic AU model should be used (dynamic is useful if the video is long enough to include neutral expressions)

    // Used for image masking
    string tri_loc = "../Resources/model/tris_68_full.txt";
    
//    // Will warp to scaled mean shape
//    cv::Mat_<double> similarity_normalised_shape = model.pdm.mean_shape * sim_scale;
//    // Discard the z component
//    similarity_normalised_shape = similarity_normalised_shape(cv::Rect(0, 0, 1, 2*similarity_normalised_shape.rows/3)).clone();
//    
    // If multiple video files are tracked, use this to indicate if we are done
    bool done = false;
    int f_n = -1;
    int curr_img = -1;
    string au_loc;
    
    if (dynamic) {
        au_loc = "../Resources/AU_predictors/AU_all_best.txt";
    } else {
        au_loc = "../Resources/AU_predictors/AU_all_static.txt";
    }

    
    // Creating a  face analyser that will be used for AU extraction
    FaceAnalysis::FaceAnalyser face_analyser(vector<cv::Vec3d>(), 0.7, 112, 112, au_loc, tri_loc);


}

void ofApp::updateKinect() {
    kinect->update();
    
    if (kinect->isFrameNew()) {
        // Update Kinect FPS tracking
        if (++kinectFrameCounter == 15) {
            double tsNow = (double)ofGetElapsedTimeMillis();
            kinectFPS = (double)kinectFrameCounter / ((tsNow/1000) - (tsKinectFPS/1000));
            tsKinectFPS = tsNow;
            kinectFrameCounter = 0;
        }

        ofPixels pixelsRGB = kinect->getRgbPixels();
        ofPixels pixelsDepth = kinect->getDepthPixels();
        ofFloatPixels pixelsDepthRaw = kinect->getRawDepthPixels();
        
        texRGB.loadData(pixelsRGB);
        texDepth.loadData(pixelsDepth);
        
        // Convert RGB frame to mat and then grayscale
        cv::Mat matRGB = ofxCv::toCv(pixelsRGB);
        cv::cvtColor(matRGB, matGrayscale, CV_BGR2GRAY);
        
        // Convert depth frame to mat
        matDepth = cv::Mat(pixelsDepthRaw.getHeight(), pixelsDepthRaw.getWidth(), ofxCv::getCvImageType(pixelsDepthRaw), pixelsDepthRaw.getData(), 0);
        
        initialized = true;
    }
}

void ofApp::detectFaces() {
    // Haar detector
//    LandmarkDetector::DetectFaces(face_detections, matGrayscale, model->face_detector_HAAR);
    
//    // HOG detector
//    vector<double> confidences;
//    LandmarkDetector::DetectFacesHOG(face_detections, matGrayscale, model->face_detector_HOG, confidences);
    
    
    ofLog() << "faces detected:" << face_detections.size();
}

void ofApp::updateFeatures() {
    

    float fx, fy, cx, cy;
    int d = 0;
    
    // If optical centers are not defined just use center of image
    cx = 1920 / 2.0f;
    cy = 1080 / 2.0f;
  
    // Use a rough guess-timate of focal length
    fx = 500 * (1920 / 640.0);
    fy = 500 * (1080 / 480.0);
    fx = (fx + fy) / 2.0;
    fy = fx;
    
    // This is useful for a second pass run (if want AU predictions)
    vector<cv::Vec6d> params_global_video;
    vector<bool> successes_video;
    vector<cv::Mat_<double>> params_local_video;
    vector<cv::Mat_<double>> detected_landmarks_video;
    
    bool visualise_hog = true;
    
    // XXX: @avi do we want world coordinates?
    bool use_world_coordinates = true;

    // The actual facial landmark detection / tracking
    bool detection_success;
    
    vector<cv::Rect_<double> > face_detections;
    

    if (model_parameters.curr_face_detector == LandmarkDetector::FaceModelParameters::HOG_SVM_DETECTOR) {
        vector<double> confidences;
        detection_success = LandmarkDetector::DetectFacesHOG(face_detections, matGrayscale, models[0].face_detector_HOG, confidences);
    } else {
        detection_success = LandmarkDetector::DetectFaces(face_detections, matGrayscale, models[0].face_detector_HAAR);
    }
    
    // Keep only non overlapping detections (also convert to a concurrent vector)
    NonOverlapingDetections(models, face_detections);
    vector<tbb::atomic<bool> > face_detections_used(face_detections.size());
    
    for (int i = 0; i < face_detections.size(); i++) {
        ofLog() << "Face detected at:" << face_detections[i].x << "," << face_detections[i].y;
    }
//
//    // Go through every model and update the tracking
//    tbb::parallel_for(0, (int)models.size(), [&](int model_ind) {
//        bool detection_success = false;
//        
//        // If the current model has failed more than 4 times in a row, remove it
//        if (models[model_ind].failures_in_a_row > 4) {
//            active_models[model_ind] = false;
//            models[model_ind].Reset();
//        }
//        
//        // If the model is inactive reactivate it with new detections
//        if (!active_models[model_ind]) {
//            for (size_t detection_ind = 0; detection_ind < face_detections.size(); ++detection_ind) {
//                // if it was not taken by another tracker take it (if it is false swap it to true and enter detection, this makes it parallel safe)
//                if (face_detections_used[detection_ind].compare_and_swap(true, false) == false) {
//                    // Reinitialise the model
//                    models[model_ind].Reset();
//                    // This ensures that a wider window is used for the initial landmark localisation
//                    models[model_ind].detection_success = false;
//                    
//                    detection_success = LandmarkDetector::DetectLandmarksInVideo(matGrayscale, matDepth, face_detections[detection_ind], models[model_ind], det_parameters[model_ind]);
//                    
//                    // This activates the model
//                    active_models[model_ind] = true;
//                    // break out of the loop as the tracker has been reinitialised
//                    break;
//                }
//            }
//        } else {
//            // The actual facial landmark detection / tracking
//            detection_success = LandmarkDetector::DetectLandmarksInVideo(matGrayscale, matDepth, models[model_ind], det_parameters[model_ind]);
//        }
//    });
    
    // Go through every model and visualise the results
    for (size_t model_ind = 0; model_ind < models.size(); ++model_ind) {
        if (!models[model_ind].detection_success) {
            continue;
        }
        
        // Gaze tracking, absolute gaze direction
        cv::Point3f gazeDirection0(0, 0, -1);
        cv::Point3f gazeDirection1(0, 0, -1);
        FaceAnalysis::EstimateGaze(models[model_ind], gazeDirection0, fx, fy, cx, cy, true);
        FaceAnalysis::EstimateGaze(models[model_ind], gazeDirection1, fx, fy, cx, cy, false);
//        // Do face alignment
//        cv::Mat sim_warped_img;
//        cv::Mat_<double> hog_descriptor;
//       
//            // But only if needed in output
//            if (!output_similarity_align.empty() || hog_output_file.is_open() || output_AUs) {
//                face_analyser.AddNextFrame(captured_image, clnf_models[model], time_stamp, false, !det_parameters[model].quiet_mode);
//                face_analyser.GetLatestAlignedFace(sim_warped_img);
//                if (!det_parameters[model].quiet_mode) {
//                    cv::imshow("sim_warp", sim_warped_img);
//                    stringstream filePath;
//                    filePath << outputPath << "\\ProcessedImages\\" << getDateTime() << "_" << model << ".jpg";;
//                    cv::imwrite(filePath.str(), sim_warped_img);
//                }
//                if (hog_output_file.is_open())
//                {
//                    FaceAnalysis::Extract_FHOG_descriptor(hog_descriptor, sim_warped_img, num_hog_rows, num_hog_cols);
//                    if (visualise_hog && !det_parameters[model].quiet_mode)
//                    {
//                        cv::Mat_<double> hog_descriptor_vis;
//                        FaceAnalysis::Visualise_FHOG(hog_descriptor, num_hog_rows, num_hog_cols, hog_descriptor_vis);
//                        cv::imshow("hog", hog_descriptor_vis);
//                    }
//                }
//            }
//
        
        // Work out the pose of the head from the tracked model
        cv::Vec6d pose_estimate;
        if (use_world_coordinates) {
            pose_estimate = LandmarkDetector::GetCorrectedPoseWorld(models[model_ind], fx, fy, cx, cy);
        } else {
            pose_estimate = LandmarkDetector::GetCorrectedPoseCamera(models[model_ind], fx, fy, cx, cy);
        }
        
        // Visualising the tracker
//        visualise_tracking(captured_image, models[model], det_parameters[model], gazeDirection0, gazeDirection1, frame_count, fx, fy, cx, cy);
        
    }

    // ***!!!!***
    // do we actually want to do this??
    
//    // Reset the models for the next video
//    face_analyser.Reset();
//    for (size_t i = 0; i < clnf_models.size(); ++i) {
//        clnf_models[i].Reset();
//        active_models[i] = false;
//    }
    
    
}

//--------------------------------------------------------------
void ofApp::update() {
    updateKinect();
    if (initialized) {
//        detectFaces();
    }
    
    bool all_models_active = true;
    for (unsigned int model = 0; model < models.size(); ++model) {
        if (!active_models[model]) {
            all_models_active = false;
        }
    }
    
    if ((ofGetFrameNum() % 30 == 0) && !all_models_active) {
        ofLog() << "Running model (" << ofGetFrameNum() << ")";
        updateFeatures();
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    if (!initialized) return;
    
    texRGB.draw(0, 0);
    texDepth.draw(0, 0); //512 x 424

    ofSetColor(ofColor::black);
    ofDrawRectangle(0, ofGetHeight() - 140, 410, 140);
    ofSetColor(ofColor::red);
    textFont.drawString("Kinect FPS: " + ofToString(kinectFPS, 2), 10, ofGetHeight() - textFont.getSize() + 10);
    textFont.drawString("Draw FPS: " + ofToString(ofGetFrameRate(), 2), 10, ofGetHeight() - (3 * textFont.getSize()));
    ofSetColor(ofColor::white);
    
    for (int i = 0; i < face_detections.size(); i++) {
        cv::Rect d = face_detections[i];
        // skip tiny "faces" because there's a huge false-positive rate
        ofLog() << d.width * d.height;
        if ((d.width * d.height) < 5000) {
            continue;
        }
        drawBoundingBox(d.x, d.y, d.width, d.height, 5);
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
