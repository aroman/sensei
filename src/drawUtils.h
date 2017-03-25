#pragma once

#include "ofTrueTypeFont.h"
#include "ofColor.h"
#include "ofRectangle.h"

void drawBoundBox(ofRectangle r, ofColor color);

void drawStringCentered(ofTrueTypeFont font, string s, int xc, int yc, ofColor boxColor, ofColor textColor);

void drawStringTopLeft(ofTrueTypeFont font, string s, int xl, int yl, ofColor boxColor, ofColor textColor);

void drawStringTopRight(ofTrueTypeFont font, string s, int xr, int yr, ofColor boxColor, ofColor textColor);

void drawStringBottomLeft(ofTrueTypeFont font, string s, int xl, int yl, ofColor boxColor, ofColor textColor);

void drawStringBottomRight(ofTrueTypeFont font, string s, int xr, int yr, ofColor boxColor, ofColor textColor);

void scaleDepthPixelsForDrawing(ofFloatPixels *depthPixels);
