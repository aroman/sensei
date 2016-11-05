#include "ofApp.h"

// TODO: don't hard-code these
#define MODEL_LOCATION "/Users/avi/Developer/ClassDemo/bin/model"
#define FACE_DETECTOR_LOCATION "/Users/avi/Developer/ClassDemo/bin/classifiers/haarcascade_frontalface_default.xml"


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

//--------------------------------------------------------------
void ofApp::setup() {
    
    initialized = false;
    
    // set background color
    ofBackground(30, 30, 30);
    
    kinectFrameCounter = 0;
    
    // ofxKinect2 guarantees that device ID 0 will always refer to the same kinect
    // if there's only ever one plugged in
    // see: https://github.com/ofTheo/ofxKinectV2/blob/a536824/src/ofxKinectV2.h#L20
    int kinectId = 0;
    
    kinect = new ofxKinectV2();
    
    // connect to the kinect. updates occur on a separate thread
    bool didOpenSuccessfully = kinect->open(kinectId);
    
    if (!didOpenSuccessfully) {
        std::exit(1);
    }
    
//    model_parameters.model_location = MODEL_LOCATION;
//    model_parameters.face_detector_location = FACE_DETECTOR_LOCATION;
//    model_parameters.use_face_template = true;
//    // This is so that the model would not try re-initialising itself
//    model_parameters.reinit_video_every = -1;
//    model_parameters.curr_face_detector = LandmarkDetector::FaceModelParameters::HOG_SVM_DETECTOR;

    
    //model = new LandmarkDetector::CLNF(model_parameters.model_location);
    //model->face_detector_HAAR.load(model_parameters.face_detector_location);
    //model->face_detector_location = model_parameters.face_detector_location;
}

//--------------------------------------------------------------
void ofApp::update() {
    
    kinect->update();
    
    if (kinect->isFrameNew()) {
        initialized = true;
        
        // Update Kinect FPS tracking
        if (++kinectFrameCounter == 15) {
            double tsNow = (double)ofGetElapsedTimeMillis();
            kinectFPS = (double)kinectFrameCounter / ((tsNow/1000) - (tsKinectFPS/1000));
            tsKinectFPS = tsNow;
            kinectFrameCounter = 0;
        }
        
        ofPixels pixelsRGB = kinect->getRgbPixels();
        
        texRGB.loadData(pixelsRGB);
        texDepth.loadData(kinect->getDepthPixels());

        //LandmarkDetector::DetectFaces(face_detections, matGrayscale, model->face_detector_HAAR);
        
        //cv::Mat matRGB;
        //matRGB = ofxCv::toCv(pixelsRGB);
        //cv::cvtColor(matRGB, matGrayscale, CV_BGR2GRAY);
        
        //vector<double> confidences;
        //LandmarkDetector::DetectFacesHOG(face_detections, matGrayscale, model->face_detector_HOG, confidences);
        
        
        //LandmarkDetector::DetectLandmarksInVideo(matGrayscale, *model, model_parameters);
//        
//        ofLog() << "size:" << face_detections.size();
//        
//        
//        if (ofGetFrameNum() % 10 == 0) {
//            ofLog() << "Running model (" << ofGetFrameNum() << ")";
//
//           // if (model->tracking_initialised) {
//           //     ofLog() << "tracking initialized!";
//           // }
//            
//            //2D landmark location (in image):
//            
//            //clnf_model.detected_landmarks contains a double matrix in following format [x1;x2;...xn;y1;y2...yn]
//            // describing the detected landmark locations in the image
//        }
    }
    
}

//--------------------------------------------------------------
void ofApp::draw() {
    if (!initialized) return;
    
    texRGB.draw(0, 0);
    texDepth.draw(0, 0); //512 x 424

    ofDrawBitmapStringHighlight("Kinect FPS: " + ofToString(kinectFPS, 2), 0, 10);
    ofDrawBitmapString("Draw FPS: " + ofToString(ofGetFrameRate(), 2), 0, 30);
    
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
