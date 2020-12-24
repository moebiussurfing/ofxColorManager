#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetFrameRate(60);
    ofEnableAlphaBlending();

    ColorManager.setup();

    ColorManager.setColor_TARGET(color);
}

//--------------------------------------------------------------
void ofApp::update()
{
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofPushStyle();
	ofFill();
	ofSetColor(color);
	ofDrawRectangle(5, 5, 10, 10);
	ofNoFill();
	ofSetLineWidth(2);
	ofSetColor(0);
	ofDrawRectangle(5, 5, 10, 10);
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    if (key == 'h')
    {
        ColorManager.setToggleVisible();
    }

    if (key == OF_KEY_TAB)
    {
		//get palette
        palette = ColorManager.getPalette();
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
    ColorManager.windowResized(w, h);
}

//--------------------------------------------------------------
void ofApp::exit()
{
	ColorManager.exit();
}
