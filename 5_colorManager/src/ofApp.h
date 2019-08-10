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
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    ofxColorManager ColorManager;

    //-

    // received targeted palette from addon
    vector<ofColor> palette;
    ofColor color_TARGET;

    // live control
    float control;
    bool direction = true;

    bool hide = false;

    //-
};