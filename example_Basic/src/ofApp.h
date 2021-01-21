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
	
	//-

    // local palette and color 
	// as target registered pointers
    vector<ofColor> palette;
    ofColor colorPick;
    ofColor colorBg;

	//-

	ofxPanel gui;
	bool bGui = false;
	ofParameter<bool> bDrawOfApp{ "Draw TEST-ofApp", false };
	void draw_TESTofApp();// test scene
};
