#include <iostream>
#include "ofApp.h"
#include <GazeEstimation.h>
#include "tbb/tbb.h"

using namespace std;

// bufferFrame

    void bufferFrame::draw() {
        if (!hasData) return;

        ofTexture tRGB;
        tRGB.loadData(pRGB);
        tRGB.draw(0,0);


        // for(int i = 0; i < pDepth.size(); i++){
        //     // pixels[i] = ofMap(rawDepthPixels[i], minDistance, maxDistance, 255, 0, true);
        //     pDepth[i] /= 4500;
        // }
        ofTexture tDepth;
        tDepth.loadData(pDepth);
        tDepth.draw(0, 360, 640, 360);

        ofTexture tBigDepth;
        // tBigDepth.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
        tBigDepth.loadData(pBigDepth);
        tBigDepth.draw(640, 0, 640, 360);

        for (int i = 0; i < faces.size(); i++) {
            rect r = faces[i].r;

            ofPath p;
            p.setFillColor(ofColor::blue);
            p.rectangle(r.x,
                        r.y,
                        r.width,
                        r.height);

            p.rectangle(r.x + (r.width*0.1),
                        r.y + (r.height*0.1),
                        (r.width*0.8),
                        (r.height*0.8));

            p.draw();

          // float *pixels = pBigDepth.getData();
          // int closePixel = static_cast<int>(r.x * r.y);
          // int closePixel = 1920*50 + 600;
          // ofDrawBitmapStringHighlight(std::to_string(pixels[closePixel]), r.x, r.y);
        }
    }

    void bufferFrame::findFaces(FaceDetector *faceDetector) {
        if (!hasData) return;
        faces.clear();

        pRGB.resize(640, 360, OF_INTERPOLATE_NEAREST_NEIGHBOR);
        cv::Mat matAdjust = ofxCv::toCv(pRGB);
        // XXX: When I rewrote the kinect integration, this conversion became necessary...
        cv::cvtColor(ofxCv::toCv(pRGB), matAdjust, CV_BGRA2BGR);
        faceDetector->updateImage(matAdjust);
        vector<mtcnn_face_bbox> mxFaces = faceDetector->detectedFaces.bboxes;
        int numFacesFound = mxFaces.size();

        faces.reserve(numFacesFound); //increases the capacity

        for(uint i = 0; i < numFacesFound; i++){
            faceData face;

            face.r.x = mxFaces[i].x1;
            face.r.y = mxFaces[i].y1;
            face.r.width = abs(mxFaces[i].x2-mxFaces[i].x1);
            face.r.height = abs(mxFaces[i].y2-mxFaces[i].y1);

            faces.push_back(face);
        }

    }

// figKinect

    void figKinect::setup() {
        cout << "figKinect::setup()" << endl;

        if (kinect != NULL) delete kinect;
        kinect = new KinectHelper();

        bool didConnectSuccessfully = kinect->connect();
        if (!didConnectSuccessfully) {
            std::exit(1);
        }

        if (frame == NULL) frame = new bufferFrame();
        frame->hasData = false;

        faceDetector = new FaceDetector();
        faceDetector->startThread(true);
    }

    void figKinect::update() {
        if (!kinect->isConnected) {
          return;
        }
        frame->pRGB = kinect->getRgbPixels();
        frame->pDepth = kinect->getDepthPixels();
        frame->pBigDepth = kinect->getBigDepthPixels();
        frame->hasData = (frame->pRGB.size() > 0);
        frame->findFaces(faceDetector);

        // kinect->update();
        //
        // if(kinect->isFrameNew()){
        //     // frame->hasData = false;
        //     //
        //     // frame->pRGB = kinect->getRgbPixels();
        //     // frame->pDepth = kinect->getDepthPixels();
        //     //
        //     // frame->rgbFrame = (libfreenect2::Frame *)kinect->getRgbFrame();
        //     // frame->depthFrame = (libfreenect2::Frame *)kinect->getDepthFrame();
        //
        //     // frame->findFaces(faceDetector);
        //
        //
        //     //frame->getPoses();
        //
        //     frame->hasData = true;
        // }
    }

    void figKinect::draw() {
        //cout << "figKinect::draw()" << endl;

        if(frame != NULL){
            frame->draw();
        }

    }

    figKinect::~figKinect() {
        cout << "figKinect::~figKinect" << endl;

        frame->faces.clear();
        delete frame;
        kinect->disconnect();
        delete kinect;
    }

// ofApp

    void ofApp::setup() {
        cout << "ofApp::setup()" << endl;

        if(kinect != NULL) delete kinect;
        kinect = new figKinect();
        kinect->setup(); //if this returns, we connected

        //font.load("/opt/sensei/Hack-Regular.ttf", 28, true);
    }

    void ofApp::update() {
        //cout << "ofApp::update()" << endl;
        kinect->update();
    }

    void ofApp::draw() {
        //cout << "ofApp::draw()" << endl;
        // ofClear(0);
        kinect->draw();
    }

    ofApp::~ofApp() {
        //cout << "ofApp::~ofApp" << endl;
        if(kinect != NULL) delete kinect;
    }

    // ofApp I/O
    void ofApp::keyPressed(int key){}
    void ofApp::keyReleased(int key){}
    void ofApp::mouseMoved(int x, int y){}
    void ofApp::mouseDragged(int x, int y, int button){}
    void ofApp::mousePressed(int x, int y, int button){}
    void ofApp::mouseReleased(int x, int y, int button){}
    void ofApp::mouseEntered(int x, int y){}
    void ofApp::mouseExited(int x, int y){}
    void ofApp::windowResized(int w, int h){}
    void ofApp::gotMessage(ofMessage msg){}
    void ofApp::dragEvent(ofDragInfo dragInfo){}
