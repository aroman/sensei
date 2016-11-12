#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxKinectV2.h"
#include "LandmarkCoreIncludes.h"
#include "ofxFaceTracker2.h"
#include <FaceAnalyser.h>

#include "FaceDetector.hpp"


class ofApp : public ofBaseApp{

public:
    
    ofxKinectV2 *kinect;
    double tsKinectFPS;
    uint32_t kinectFrameCounter;
    double kinectFPS;
    
    ofTrueTypeFont textFont;
    
    ofTexture texRGB;
    ofTexture texDepth;
    cv::Mat_<uchar> matGrayscale;
    cv::Mat_<float> matDepth;
    
    bool initialized;
    
    float fx, fy, cx, cy;
    vector<LandmarkDetector::CLNF> models;
    vector<LandmarkDetector::FaceModelParameters> model_parameters;
    vector<bool> active_models;
    vector<cv::Rect_<double>> faces_detected;
    FaceAnalysis::FaceAnalyser face_analyser;

    ofxFaceTracker2 tracker;
    FaceDetector faceDetector;
    
    void setup();
    void update();
    void draw();
    
    void updateKinect();
    bool detectFacesWithOpenFace();
    bool detectLandmarks();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
};
