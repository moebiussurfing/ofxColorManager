#pragma once

#include "ofMain.h"

#include "ofxColorManager.h"

class ofApp : public ofBaseApp
{
	void setup();

	ofxColorManager colorManager;

	// colors
	vector<ofColor> palette;
	ofColor colorPick;

	//TODO:
	//BUG:
	//almost always blocks any dragging into the window..
	void dragEvent(ofDragInfo dragInfo);
};
