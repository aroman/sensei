#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxKinectV2.h"
#include "LandmarkCoreIncludes.h"
#include <FaceAnalyser.h>


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
    cv::Mat_<float>  matDepth;
    
    bool initialized;
    
    LandmarkDetector::FaceModelParameters model_parameters;
    // wtf is this, actually?
    vector<LandmarkDetector::FaceModelParameters> det_parameters;
    LandmarkDetector::CLNF model;
    // The modules that are being used for tracking
    vector<LandmarkDetector::CLNF> models;
    vector<bool> active_models;
    vector<cv::Rect_<double> > face_detections;
    
    
    void setup();
    void update();
    void draw();
    
    void updateKinect();
    void detectFaces();
    void updateFeatures();

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
