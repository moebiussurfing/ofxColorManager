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
	void draw();
    void keyPressed(int key);
    void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);

    ofxColorManager colorManager;

    // local palette and color (target pointers)
    vector<ofColor> palette;
    ofColor color;
	void drawTest();

#ifdef USE_OFX_WINDOW_APP
	ofxWindowApp WindowApp;
#endif
};