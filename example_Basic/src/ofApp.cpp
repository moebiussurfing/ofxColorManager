#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	bDraw_ofApp = true;
	bGui = true;

	//-

	ofEnableAlphaBlending();
	ofSetFrameRate(60);
	ofSetWindowPosition(-1920, 18);

	//--

	// we subscribe our local colors
	// using pointers/references
	// to be all autoupdated (as targets)
	colorManager.setPalette_TARGET(palette); // 1. the palette
	colorManager.setColorBg_TARGET(colorBg); // 2. the background color 
	colorManager.setColor_TARGET(colorPick); // 3. the picked color 
	colorManager.setName_TARGET(name); // 4. the name of preset or enigne

	colorManager.setup();

	//--

	gui.setup("ofApp");
	gui.add(colorManager.getParameters_Debug());
	gui.add(bDraw_ofApp);
	gui.setPosition(320, 170);
	gui.getGroup(colorManager.getParameters_Debug().getName()).minimize();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	if (bDraw_ofApp) draw_TEST_ofApp();
	if (bGui) gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == 'G')
	{
		bGui = !bGui;
	}

	// we can get the palette at any precise time
	// (we need to use pointers if we want autoupdate the palette)
	else if (key == '*')
	{
		palette = colorManager.getPalette();

		// print colors
		ofLogNotice(__FUNCTION__) << "Palette: ";
		int c = 0;
		for (auto p : palette)
		{
			ofLogNotice(__FUNCTION__) << "color #" << c++ << ": " << ofToString(p);
		}
	}
}

////--------------------------------------------------------------
//void ofApp::dragEvent(ofDragInfo info) {
//	colorManager.dragEvent(info);
//}

//--------------------------------------------------------------
void ofApp::draw_TEST_ofApp()
{
	// draw local (ofApp) test scene
	ofPushStyle();
	ofPushMatrix();
	{
		int sz = 100;// boxes width size
		float szhr = 0.25;// vertical h/w ratio

		int sz2 = sz - 0;// boxes inner size with padding

		ofTranslate(gui.getShape().getBottomLeft().x + 4, gui.getShape().getBottomLeft().y + 14);

		// labels
		ofDrawBitmapStringHighlight("TEST_ofApp", 0, 0, ofColor::black, ofColor::white);
		ofDrawBitmapStringHighlight(name, 0, 15, ofColor::black, ofColor::white);
		//ofDrawBitmapStringHighlight(colorManager.getPaletteName(), 0, 15, ofColor::black, ofColor::white);

		ofTranslate(1, 22);

		ofFill();

		// bg panel box
		ofSetColor(0, 255);
		int _p = 5;
		int _pp = 2 * _p;
		ofRectangle _rbg(-_p, -_p, (palette.size() *  sz) + _pp, 2 * (sz * szhr) + _pp);
		ofDrawRectRounded(_rbg, 3);
		//ofDrawRectangle(_rbg);

		// the picker color
		ofSetColor(colorPick);
		ofDrawRectangle(0, 0, sz2, sz2 * szhr);

		// the bg color
		ofSetColor(colorBg);
		if (palette.size() > 0) ofDrawRectangle(sz, 0, sz * (palette.size() - 1), sz2 * szhr);
		else ofDrawRectangle(sz, 0, sz, sz2 * szhr);
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