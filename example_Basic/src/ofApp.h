#pragma once
#include "ofMain.h"

#include "ofxColorManager.h"

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
};
