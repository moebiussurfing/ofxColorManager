#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofEnableAlphaBlending();

    float fps = 60;
    ofSetFrameRate(fps);

    ColorManager.setup();
    ColorManager.setFps(fps);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    //if (key == 'h')
    //{
    //    hide = !hide;
    //    ColorManager.setVisible(!hide);
    //}

    //if (key == OF_KEY_TAB)
    //{
    //    palette = ColorManager.getPalette();
    //    ofLogNotice("ofApp") << "getPalette";

    //    for (int i = 0; i < palette.size(); i++)
    //    {
    //        ofLogNotice("ofApp") << ofToString(palette[i]);
    //    }
    //}
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
    ColorManager.windowResized(w, h);
}