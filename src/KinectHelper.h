#pragma once

#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/registration.h>
// #include <libfreenect2/logger.h>
#include "ofThread.h"
#include "ofMain.h"

class KinectHelper : public ofThread {

public:
    KinectHelper();
    void threadedFunction();
    bool connect();
    void disconnect();

    bool isConnected;

    ofPixels getColorPixels();
    ofFloatPixels getUnalignedDepthPixels();
    ofFloatPixels getAlignedDepthPixels();

    // Color camera parameters. Used for OpenFace.
    float fx, fy, cx, cy;

private:
    // Double-buffer to avoid tearing and artifacts
    ofPixels colorPixelsBack;
    ofFloatPixels unalignedDepthPixelsBack;
    ofFloatPixels alignedDepthPixelsBack;

    ofPixels colorPixelsFront;
    ofFloatPixels unalignedDepthPixelsFront;
    ofFloatPixels alignedDepthPixelsFront;

    libfreenect2::FrameMap frames;
    libfreenect2::Freenect2 freenect2;
    libfreenect2::Freenect2Device *device = NULL;
    libfreenect2::SyncMultiFrameListener *listener = NULL;

    libfreenect2::Registration *registration = NULL;
    libfreenect2::Frame *undistorted = NULL;
    libfreenect2::Frame *registered = NULL;
    libfreenect2::Frame *bigDepth = NULL;
};
