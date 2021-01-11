#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofEnableAlphaBlending();
	ofSetFrameRate(60);

	colorManager.setup();

	// we subscribe using pointers references
	colorManager.setColor_TARGET(color); // color to be autoupdated
	colorManager.setPalette_TARGET(palette); // palette to be autoupdated

	//--

	gui.setup("ofApp");
	gui.setPosition(500, 500);

	gui.add(bDrawOfApp);

	ofParameterGroup pg{ "ofxColorManager" };
	pg.add(colorManager.SHOW_Scene);
	pg.add(colorManager.SHOW_ALL_GUI);
	pg.add(colorManager.SHOW_MINI_Preview);
	gui.add(pg);
}

//--------------------------------------------------------------
void ofApp::draw()
{
	if (bDrawOfApp) drawOfApp();

	//colorManager.draw_MiniPreview();// internal palette preview

	gui.draw();
}

//--------------------------------------------------------------
void ofApp::drawOfApp()
{
	//  draw test scene
	ofPushStyle();
	ofPushMatrix();
	{
		int sz = 70;

		ofTranslate(ofGetWidth() * 0.5 - sz * palette.size() * 0.5, ofGetHeight() - 2 * sz - 30);
		//ofTranslate(10, 10);

		ofDrawBitmapStringHighlight(colorManager.getPaletteName(), 4, -7, ofColor::black, ofColor::white);

		ofFill();

		ofSetColor(color);// the picker color

		ofDrawRectangle(0, 0, sz, sz);
		ofTranslate(0, sz);
		for (auto p : palette) 
		{
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
