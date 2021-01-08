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
	WindowApp.setPositionDebugInfo(ofxWindowApp::DEBUG_POSITION_TOP);
#else
	ofSetFrameRate(fps);
#endif

	colorManager.setup();
	colorManager.setColor_TARGET(color); // we subscribe to the color to be autoupdated (by reference from color picker)
}

//--------------------------------------------------------------
void ofApp::draw()
{
	colorManager.draw();

	//-

	//  draw test scene
	ofPushStyle();
	ofPushMatrix();
	{
		int sz = 100;
		ofTranslate(10, 10);
		ofFill();
		ofSetColor(color);
		ofDrawRectangle(0, 0, sz, sz);
		ofTranslate(0, sz);
		for (auto p : palette) {
			ofSetColor(p);
			ofDrawRectangle(0, 0, sz, sz);
			ofTranslate(sz, 0);
		}
	}
	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (false) {}

	else if (key == 'g')
	{
		colorManager.setToggleVisible();
	}

	// we can get the palette (without using pointers)
	else if (key == OF_KEY_TAB)
	{
		palette = colorManager.getPalette();

		// print colors
		ofLogNotice(__FUNCTION__) << "palette: ";
		int c = 0;
		for (auto p : palette) {
			ofLogNotice(__FUNCTION__) << "color " << c << ": " << ofToString(p);
			c++;
		}
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
