#pragma once
#include "ofMain.h"

#define USE_OFX_WINDOWAPP// window manager

#ifdef USE_OFX_WINDOWAPP
#include "ofxWindowApp.h"
#endif

#include "ofxColorManager.h"

//#include "ofxGui.h"

class ofApp: public ofBaseApp{

public:
    void setup();
	//void draw();
 //   void keyPressed(int key);

    ofxColorManager colorManager;
	
	//--

    // local colors
	// auto-updated as targets
	// registered to the addon colors
    vector<ofColor> palette;
    ofColor colorPick;
    //ofColor colorBg;
	//std::string name;

	//--

	//// test scene
	//ofxPanel gui;
	//bool bGui = false;
	//ofParameter<bool> bDraw_ofApp{ "Debug", false };
	//void draw_TEST_ofApp();

	//--

private:
#ifdef USE_OFX_WINDOWAPP
	ofxWindowApp windowApp;
#endif
};
