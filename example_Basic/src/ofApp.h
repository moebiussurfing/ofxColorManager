#pragma once
#include "ofMain.h"

#include "ofxColorManager.h"

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
};
