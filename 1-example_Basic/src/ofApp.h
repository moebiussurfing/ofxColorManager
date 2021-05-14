#pragma once

#include "ofMain.h"

#include "ofxColorManager.h"

class ofApp : public ofBaseApp
{
public:

	void setup();

	ofxColorManager colorManager;

	// colors
	vector<ofColor> palette;
	ofColor colorPick;

	//void dragEvent(ofDragInfo dragInfo);
};
