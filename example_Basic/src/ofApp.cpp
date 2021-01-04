#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofEnableAlphaBlending();
	
	//window
	float fps = 60.0f;
#ifdef USE_OFX_WINDOW_APP
	WindowApp.setFrameRate(fps);
	WindowApp.setVerticalSync(true);
#else
	ofSetFrameRate(fps);
#endif
    
    colorManager.setup();
	colorManager.setColor_TARGET(color); // we get color (using a reference)
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofPushStyle();
	int sz = 50;
	ofFill();
	ofSetColor(color);
	ofDrawRectangle(5, 5, sz, sz);
	ofNoFill();
	ofSetLineWidth(2);
	ofSetColor(0);
	ofDrawRectangle(5, 5, sz, sz);
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (false) { }

	else if (key == 'g')
	{
		colorManager.setToggleVisible();
	}
	
	// we get palette (without using pointers)
    else if (key == OF_KEY_TAB)
    {
        palette = colorManager.getPalette();
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
    colorManager.windowResized(w, h);
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo info) {
	colorManager.dragEvent(info);
}
