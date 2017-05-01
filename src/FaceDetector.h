#pragma once

#include <vector>
#include "ofThread.h"
#undef Status
#undef None
#include "MtcnnDetector.h"

class FaceDetector : public ofThread {

public:
    FaceDetector(double scaleFactor = 4.0);
    ~FaceDetector();
    void threadedFunction();
    void updateImage(ofPixels colorPixels);

    double scaleFactor;
    ofEvent<const vector<ofRectangle>> onDetectionResults;

private:
    MtcnnDetector *detector;
    bool isImageDirty;

    ofPixels colorPixels;

};
