#pragma once

#include "ofMain.h"


#define MAX_MILLIMETERS ((float)4500.0)
#define MIN_MILLIMETERS ((float)500.0)


void drawBoundBox(ofRectangle r, ofColor color);

void drawStringCentered(ofTrueTypeFont font, string s, int xc, int yc, ofColor boxColor, ofColor textColor);


void drawStringTopLeft(ofTrueTypeFont font, string s, int xl, int yl, ofColor boxColor, ofColor textColor);

void drawStringTopRight(ofTrueTypeFont font, string s, int xr, int yr, ofColor boxColor, ofColor textColor);

void drawStringBottomLeft(ofTrueTypeFont font, string s, int xl, int yl, ofColor boxColor, ofColor textColor);

void drawStringBottomRight(ofTrueTypeFont font, string s, int xr, int yr, ofColor boxColor, ofColor textColor);

void scaleDepthPixelsForDrawing(ofFloatPixels *depthPixels);
