#pragma once

#include "ofxCv.h"
#include "ofMain.h"

struct Space {
  ofRectangle r;

  ofPixels colorPixels;
  ofFloatPixels depthPixels;
  float *depthMap; // raw meters

  float minDepth;
  float maxDepth;
  float avgDepth;
  float avgDepthDiff;

  void doDepthAverage(ofRectangle r);
  void doDepthMinMax(ofRectangle r);

  void updateDepthPixels(const ofFloatPixels &newDepthPixels);
  void updateColorPixels(const ofPixels &newColorPixels);
};

struct Person {
  explicit Person(ofRectangle bbox);

  Space f; // face
  Space h; // hand-raise area

  bool raisedHand = false;

  void drawFrontalView() const;
  void drawBirdseyeView() const;
  void update(const ofPixels &newColorPixels, const ofFloatPixels &newDepthPixels);
};
