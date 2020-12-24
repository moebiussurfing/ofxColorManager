#pragma once
#include "ofMain.h"

#include "ofxColorManager.h"

#define USE_OFX_WINDOW_APP
#ifdef USE_OFX_WINDOW_APP
#include "ofxWindowApp.h"
#endif

class ofApp: public ofBaseApp{

public:
    void setup();
	void update();
	void draw();
	void exit();
    void keyPressed(int key);
    void windowResized(int w, int h);

    ofxColorManager ColorManager;

    // received colors
    vector<ofColor> palette;
    ofColor color;

#ifdef USE_OFX_WINDOW_APP
	ofxWindowApp WindowApp;
#endif
};
