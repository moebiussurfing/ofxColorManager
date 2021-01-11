#pragma once
#include "ofMain.h"

#include "ofxColorManager.h"

#include "ofxGui.h"

class ofApp: public ofBaseApp{

public:
    void setup();
	void draw();
    void keyPressed(int key);
    void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);

    ofxColorManager colorManager;

    // local palette and color 
	// as target registered pointers
    vector<ofColor> palette;
    ofColor color;

	ofxPanel gui;
	ofParameter<bool> bScene{ "Show Scene", false };

	void drawTest();// test scene
};
