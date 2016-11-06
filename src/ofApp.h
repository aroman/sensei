#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxKinectV2.h"
#include "LandmarkCoreIncludes.h"


class ofApp : public ofBaseApp{

public:
    double tsKinectFPS;
    uint32_t kinectFrameCounter;
    double kinectFPS;
    
    ofTrueTypeFont textFont;
    
    ofxKinectV2 *kinect;
    ofTexture texRGB;
    ofTexture texDepth;
    cv::Mat matGrayscale;
    
    LandmarkDetector::FaceModelParameters model_parameters;
    LandmarkDetector::CLNF *model;
    
    vector<cv::Rect_<double> > face_detections;
    
    bool initialized;
    
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
