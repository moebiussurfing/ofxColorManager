#pragma once

#include "ofMain.h"

// window manager
#define USE_OFX_WINDOWAPP // comment to exclude. addon non required 
#ifdef USE_OFX_WINDOWAPP
#include "ofxWindowApp.h"
#endif

#include "ofxColorManager.h"

class ofApp : public ofBaseApp 
{

public:

	void setup();

#ifdef USE_OFX_WINDOWAPP
	ofxWindowApp windowApp;
#endif

	//--
	
	// colors

	vector<ofColor> palette;
	ofColor colorPick;

	ofxColorManager colorManager;
};
