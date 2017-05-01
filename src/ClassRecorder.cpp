#include "ClassRecorder.h"

#include "ofUtils.h"
#include "gzstream/gzstream.h"
#include "ofxTimeMeasurements.h"
#include "drawUtils.h"


// path to save recordings relative to the 'data' directory
static const std::string RECORDINGS_DIR_PATH = "../recordings";

ClassRecorder::ClassRecorder() {
  auto recordingPath = ofFilePath::join(RECORDINGS_DIR_PATH, ofGetTimestampString());
  colorDirectory = new ofDirectory(ofFilePath::join(recordingPath, "color"));
  depthDirectory = new ofDirectory(ofFilePath::join(recordingPath, "depth"));

  colorDirectory->create(true);
  depthDirectory->create(true);

  vidRecorder.setPixelFormat("rgb32");
  vidRecorder.setVideoCodec("libx264");

  ofAddListener(vidRecorder.outputFileCompleteEvent, this, &ClassRecorder::recordingComplete);

  // readDepthFile("/home/sensei/Developer/sensei/install/recordings/2017-04-28-01-51-48-360/depth/81.dat");
}

ClassRecorder::~ClassRecorder() {
  ofRemoveListener(vidRecorder.outputFileCompleteEvent, this, &ClassRecorder::recordingComplete);
  vidRecorder.close();

  delete colorDirectory;
  delete depthDirectory;
  this->kinect->waitForThread(true);
  this->kinect->disconnect();
}

void ClassRecorder::recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args){
    ofLogNotice("ClassRecorder") << "The recoded video file is now complete.";
}


void ClassRecorder::readDepthFile(std::string path) {
  igzstream compressedDepth;
  compressedDepth.open(path.c_str());
  if (!compressedDepth.good()) {
    ofLogError("ClassRecorder") << "Couldn't open depth file at '" << path << "'";
    exit(1);
  }

  ofBuffer depthBufRead(compressedDepth);
  // ofLogNotice("ClassRecorder") << "file size = " << depthBufRead.size();

  ofFloatPixels depthPixelsRead;
  depthPixelsRead.setFromPixels(reinterpret_cast<float *>(depthBufRead.getData()), 512, 424, OF_PIXELS_GRAY);
  scaleDepthPixelsForDrawing(&depthPixelsRead);
  depthImage.setFromPixels(depthPixelsRead);
}

void ClassRecorder::update() {
  if (!this->kinect->isConnected) return;

  ofPixels colorPixels = this->kinect->getColorPixels();
  ofFloatPixels depthPixels = this->kinect->getUnalignedDepthPixels();

  bool hasData = (colorPixels.size() > 0);

  if (!hasData) return;

  auto colorVideoPath = ofFilePath::join(colorDirectory->getAbsolutePath(), ofToString(ofGetFrameNum()) + ".mov");
  auto depthImagePath = ofFilePath::join(depthDirectory->getAbsolutePath(), ofToString(ofGetFrameNum()) + ".dat");

  if (!vidRecorder.isInitialized()) {
    vidRecorder.setup(colorVideoPath, colorPixels.getWidth(), colorPixels.getHeight(), 30, 0, 0);
    vidRecorder.start();
  }

  if (ofGetFrameNum() % 10 == 0 && vidRecorder.getVideoQueueSize() != 0) {
    ofLogNotice("ClassRecorder") << "video queue size: " << vidRecorder.getVideoQueueSize();
  }

  TS_START("save color");
  bool success = vidRecorder.addFrame(colorPixels);
  if (!success) {
    ofLogNotice("ClassRecorder") << "This frame was not added!";
  }
  if (vidRecorder.hasVideoError()) {
    ofLogNotice("ClassRecorder") << "The video recorder failed to write some frames!";
  }
  TS_STOP("save color");

  TS_START("save depth");
  auto timeSinceLastDepthFrame = ofGetElapsedTimeMillis() - timeLastDepthFrameWritten;
  if (timeSinceLastDepthFrame >= (1000 / 10.0f) ) {
    ofLogNotice("ClassRecorder") << "writing depth frame #" << ofGetFrameNum();
    timeLastDepthFrameWritten = ofGetElapsedTimeMillis();
    ofBuffer depthBuf((char*)depthPixels.getData(), depthPixels.getTotalBytes());
    ogzstream depthFileOf(depthImagePath.c_str());
    depthFileOf << depthBuf;
    depthFileOf.close();
  }
  TS_STOP("save depth");
}

void ClassRecorder::draw() {
  if (depthImage.isAllocated()) {
    depthImage.draw(0,0);
  }
}
