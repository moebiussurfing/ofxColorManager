#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofEnableAlphaBlending();
	ofSetFrameRate(60);

	colorManager.setup();

	colorManager.setColor_TARGET(color); // we subscribe to the color to be autoupdated (by reference from color picker)
	palette = colorManager.getPalette(); // get current default palette. Press TAB key to refresh!

	//--

	gui.setup("ofApp");
	gui.add(bScene);
	gui.add(colorManager.SHOW_Scene);
	gui.add(colorManager.SHOW_ALL_GUI);
	gui.add(colorManager.SHOW_MINI_Preview);
	gui.setPosition(500, 500);
}

//--------------------------------------------------------------
void ofApp::draw()
{
	if (bScene) drawTest();

	//colorManager.draw_MiniPreview();// internal palette preview

	gui.draw();
}

//--------------------------------------------------------------
void ofApp::drawTest()
{
	//  draw test scene
	ofPushStyle();
	ofPushMatrix();
	{
		int sz = 70;
		ofTranslate(10, 10);
		ofFill();
		
		ofSetColor(color);// the picker color

		ofDrawRectangle(0, 0, sz, sz);
		ofTranslate(0, sz);
		for (auto p : palette) {
		
			ofSetColor(p);// the palette colors
			
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

	// we can get the palette 
	// (we need to use pointers if we want auto refresh!)
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
