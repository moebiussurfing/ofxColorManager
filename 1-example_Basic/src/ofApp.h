#pragma once

#include "ofMain.h"

// window manager
// comment to exclude. optional 
#define USE_OFX_WINDOWAPP

#ifdef USE_OFX_WINDOWAPP
#include "ofxWindowApp.h"
#endif

#include "ofxColorManager.h"

class ofApp : public ofBaseApp
{

public:

	void setup();

	//--

	// colors

	vector<ofColor> palette;
	ofColor colorPick;

	ofxColorManager colorManager;

	//--

#ifdef USE_OFX_WINDOWAPP
	ofxWindowApp windowApp;
#endif
};
