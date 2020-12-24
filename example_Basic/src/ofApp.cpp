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
