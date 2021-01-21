#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofEnableAlphaBlending();
	ofSetFrameRate(60);
	ofSetWindowPosition(-1920, 18);

	//--

	colorManager.setup();

	// we subscribe using pointers references
	colorManager.setColor_TARGET(color); // color to be autoupdated
	colorManager.setPalette_TARGET(palette); // palette to be autoupdated

	//--

	ofParameterGroup pg{ "ofxColorManager" };
	pg.add(colorManager.SHOW_Scene);
	pg.add(colorManager.SHOW_ALL_GUI);
	pg.add(colorManager.SHOW_MINI_Preview);

	gui.setup("ofApp");
	gui.add(bDrawOfApp);
	gui.add(pg);
	gui.setPosition(500, 20);
}

//--------------------------------------------------------------
void ofApp::draw()
{
	if (bDrawOfApp) drawOfApp();

	if (bGui) gui.draw();
}

//--------------------------------------------------------------
void ofApp::drawOfApp()
{
	// draw local (ofApp) test scene
	ofPushStyle();
	ofPushMatrix();
	{
		int sz = 70;// boxes size
		float szhr = 0.25;

		ofTranslate(gui.getShape().getBottomLeft().x, gui.getShape().getBottomLeft().y + 50);
		//ofTranslate(ofGetWidth() * 0.5 - sz * palette.size() * 0.5, 200);
		//ofTranslate(ofGetWidth() * 0.5 - sz * palette.size() * 0.5, ofGetHeight() - 2 * sz - 30);

		ofFill();

		ofSetColor(0, 64);
		int _p = 5;
		ofRectangle _rbg = ofRectangle(-_p, -_p, palette.size() *  sz + 2 * _p, 2 * sz * 0.5 + 2 * _p);
		ofDrawRectangle(_rbg);

		// labels
		ofDrawBitmapStringHighlight("ofApp", 4, -7 - 14, ofColor::black, ofColor::white);
		ofDrawBitmapStringHighlight(colorManager.getPaletteName(), 4, -6, ofColor::black, ofColor::white);

		// the picker color
		ofSetColor(color);
		ofDrawRectangle(0, 0, sz, sz);

		ofTranslate(0, sz * szhr);

		// the palette colors
		for (auto p : palette)
		{
			ofSetColor(p);
			ofDrawRectangle(0, 0, sz, sz * szhr);
			ofTranslate(sz, 0);
		}
	}
	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == 'G')
	{
		bGui = !bGui;
	}

	// we can get the palette 
	// (we need to use pointers if we want auto refresh!)
	else if (key == '*')
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
