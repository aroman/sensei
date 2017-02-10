#include <iostream>
#include "ofApp.h"
#include <GazeEstimation.h>
#include "tbb/tbb.h"

using namespace std;



// bufferFrame

    void bufferFrame::doRGBD(libfreenect2::Registration* registration){
        const bool enable_filter = true; //filter out pixels not visible to both

        registration->apply(rgbFrame,depthFrame,undistorted,registered,enable_filter,bigdepth);




    }



    void bufferFrame::draw() {

        //int width = (frame->rgb).getWidth();
        //int height = (frame->rgb).getHeight();
        if(hasData){
            ofTexture tRGB;
            tRGB.loadData(pRGB);
            tRGB.draw(0,0);

            ofTexture tDepth;
            tDepth.loadData(pDepth);
            //tDepth.draw(0,0);

            for(int i = 0; i < faces.size(); i++){
                rect r = faces[i].r;

                ofPath p;
                p.setFillColor(ofColor::blue);
                p.rectangle(r.x,
                            r.y,
                            r.width,
                            r.height);

                p.draw();

            }

        }
    }


    vector<rect> bufferFrame::randNRects(uint nRects){
        vector<rect> rectVec;
        rectVec.reserve(nRects);

        for(int i = 0; i < nRects; i++){
            rect r;

            r.x      = rand() % 500;
            r.y      = rand() % 500;
            r.width  = rand() % 100;
            r.height = rand() % 100;

            rectVec.push_back(r);
        }
        return rectVec;
    }

    void bufferFrame::findFaces() {
        faces.clear();

        //>>>>>>>> INSERT FACE DETECTOR >>>>>>>>>>>>>>>

        uint numRandFaces = 10;

        vector<rect> randR = randNRects(numRandFaces);

        int numFacesFound = numRandFaces;

        //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

        faces.reserve(numFacesFound); //increases the capacity

        for(uint i = 0; i < numFacesFound; i++){
            faceData face;

            face.r = randR[i];

            faces.push_back(face);
        }

    }




// figKinect

    void figKinect::setup() {
        cout << "figKinect::setup()" << endl;

        if(kinect != NULL) delete kinect;
        kinect = new ofxKinectV2();

        uint port = 0;
        bool connected = kinect->open(port);
        if(!connected){
            cerr << "ERROR: could not kinect on port " << port << endl;
            std::exit(1);
        }

        if(frame != NULL) frame = new bufferFrame();
        frame->hasData = false;

        cout << "connected to kinect on port " << port << endl;

        freenect2 = kinect->freenect2;
        registration = kinect->registration;

        // here's how you use the mtcnn stuff

        // 1. load test image (for you, will be a frame)
        // ofImage testImg;
        // testImg.load(ofFilePath::getCurrentWorkingDirectory() + "/test.jpg");
        // cv::Mat imgMat = ofxCv::toCv(testImg);
        // cv::Mat imgConv;

        // 2. convert to grayscale
        // cv::cvtColor(imgMat, imgConv, CV_RGB2BGR);

        // 3. profit !
        // vector<mtcnn_face_bbox> faces = mxnet_detect(imgConv);
        // printf("%d faces found\n", faces.size());
        // for (mtcnn_face_bbox face : faces) {
        //   printf("\t[(%f,%f), (%f,%f), score = %f\n", face.x1, face.y1, face.x2`, face.y2, face.score);
        // }
    }

    void figKinect::update() {
        kinect->update();

        if(kinect->isFrameNew()){
            frame->hasData = false;

            frame->pRGB = kinect->getRgbPixels();
            frame->pDepth = kinect->getDepthPixels();

            frame->rgbFrame = (libfreenect2::Frame *)kinect->getRgbFrame();
            frame->depthFrame = (libfreenect2::Frame *)kinect->getDepthFrame();

            frame->findFaces();

            frame->doRGBD(registration);

            //frame->getPoses();

            frame->hasData = true;
        }
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
        kinect->close();
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
