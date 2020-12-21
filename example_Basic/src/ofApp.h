#pragma once
#include "ofMain.h"

#include "ofxColorManager.h"

class ofApp: public ofBaseApp{

public:
    void setup();
    void keyPressed(int key);
    void windowResized(int w, int h);

    ofxColorManager ColorManager;

    //-

    // received from the addon
    vector<ofColor> palette;
    ofColor color_TARGET;

    //// live control
    //float control;
    //bool direction = true;
    //bool hide = false;
};
