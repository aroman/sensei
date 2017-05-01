#pragma once

#include "ClassComponent.h"
#include "ofImage.h"
#include "ofxVideoRecorder.h"

class ClassRecorder : public ClassComponent {

public:
  ClassRecorder();
  ~ClassRecorder();

  void update();
  void draw();

private:

  uint64_t timeLastDepthFrameWritten = 0;
  ofxVideoRecorder vidRecorder;

  ofDirectory *colorDirectory;
  ofDirectory *depthDirectory;
  ofFloatImage depthImage;
  ofImage colorImage;

  void recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args);
  void readDepthFile(std::string path);

};
