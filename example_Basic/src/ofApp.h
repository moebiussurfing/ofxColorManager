#pragma once
#include "ofMain.h"

#include "ofxColorManager.h"

#include "ofxGui.h"

class ofApp: public ofBaseApp{

public:
    void setup();
	void draw();
    void keyPressed(int key);
	//void dragEvent(ofDragInfo dragInfo);

    ofxColorManager colorManager;
	
	//-

    // local colors
	// auto-updated as targets
	// registered to the addon colors
    vector<ofColor> palette;
    ofColor colorPick;
    ofColor colorBg;
	std::string name;

	//-

	// test scene
	ofxPanel gui;
	bool bGui = false;
	ofParameter<bool> bDraw_ofApp{ "Debug", false };
	void draw_TEST_ofApp();
};
