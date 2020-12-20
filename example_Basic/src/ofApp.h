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

    //-

    // received targeted palette from addon
    vector<ofColor> palette;
    ofColor color_TARGET;

    // live control
    float control;
    bool direction = true;

    bool hide = false;
};
