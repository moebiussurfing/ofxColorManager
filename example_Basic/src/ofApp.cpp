#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	bDrawOfApp = true;
	bGui = true;

	//-

	ofEnableAlphaBlending();
	ofSetFrameRate(60);
	ofSetWindowPosition(-1920, 18);

	//--

	colorManager.setup();

	// we subscribe using pointers references
	colorManager.setColorBg_TARGET(colorBg); // color to be autoupdated
	colorManager.setColor_TARGET(colorPick); // color to be autoupdated
	colorManager.setPalette_TARGET(palette); // palette to be autoupdated

	//--

	ofParameterGroup pg{ "ofxColorManager" };
	pg.add(colorManager.SHOW_Scene);
	pg.add(colorManager.SHOW_ALL_GUI);
	pg.add(colorManager.SHOW_MINI_Preview);

	gui.setup("ofApp");
	gui.add(bDrawOfApp);
	gui.add(pg);
	gui.setPosition(250, 50);
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
		int sz2 = sz - 4;// boxes size
		float szhr = 0.25;

		ofTranslate(gui.getShape().getBottomLeft().x + 5, gui.getShape().getBottomLeft().y + 50);
		//ofTranslate(ofGetWidth() * 0.5 - sz * palette.size() * 0.5, 200);
		//ofTranslate(ofGetWidth() * 0.5 - sz * palette.size() * 0.5, ofGetHeight() - 2 * sz - 30);

		ofFill();

		// bg
		ofSetColor(0, 128);
		int _p = 5;
		int _pp = 2 * _p;
		ofRectangle _rbg = ofRectangle(-_p, -_p, palette.size() *  sz + _pp, 2 * (sz * szhr) + _pp);
		ofDrawRectangle(_rbg);

		// labels
		ofDrawBitmapStringHighlight("ofApp", 4, -7 - 20, ofColor::black, ofColor::white);
		ofDrawBitmapStringHighlight(colorManager.getPaletteName(), 4, -10, ofColor::black, ofColor::white);

		// the picker color
		ofSetColor(colorPick);
		ofDrawRectangle(0, 0, sz2, sz2 * szhr);

		// the bg color
		ofSetColor(colorBg);
		ofDrawRectangle(sz, 0, sz * (palette.size() - 1) - 4, sz2 * szhr);

		ofTranslate(0, sz * szhr);

		// the palette colors
		for (auto p : palette)
		{
			ofSetColor(p);
			ofDrawRectangle(0, 0, sz2, sz * szhr);
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
