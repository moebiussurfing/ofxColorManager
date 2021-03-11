#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetWindowTitle("ofxColorManager");
	ofEnableAlphaBlending();
	//ofSetFrameRate(60);

	//--

	// we subscribe our local colors
	// using pointers/references
	// to be all autoupdated (as targets)
	colorManager.setLinkPalette(palette);		// 1. the palette
	colorManager.setLinkColorBg(colorBg);		// 2. the background color 
	colorManager.setLinkColorPick(colorPick);	// 3. the picked color 
	colorManager.setLinkName(name);				// 4. the name of preset or enigne

	colorManager.setup();

	//--

	// local gui
	gui.setup("ofApp");
	gui.add(colorManager.getParameters_Debug());
	gui.add(bDraw_ofApp);
	gui.setPosition(25, 180);
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
	// if we want to autoupdate the local colors, we need to use pointers.
	else if (key == '*')
	{
		palette = colorManager.getPalette();

		// print colors
		ofLogNotice(__FUNCTION__) << "Palette: ";
		int c = 0;
		for (auto p : palette)
		{
			ofLogNotice(__FUNCTION__) << "Color #" << c++ << ": " << ofToString(p);
		}
	}
}


//---------
/* Scene */

// a testing scene to preview the colors

//--------------------------------------------------------------
void ofApp::draw_TEST_ofApp()
{
	ofPushStyle();
	ofPushMatrix();
	{
		int sz = 50;//boxes width size
		float szhr = 0.5;//h/w ratio
		int sz2 = sz - 0;//boxes inner padded size
		int _p = 5;//padding
		int _pp = 2 * _p;//double padding

		ofFill();
		
		ofTranslate(gui.getShape().getBottomLeft().x + 4, gui.getShape().getBottomLeft().y + 14);

		// bg label name
		ofSetColor(0, 255);
		ofRectangle _rlbg(0, -14, (palette.size() *  sz) + _p + 1, 16);
		ofDrawRectangle(_rlbg);

		// label name
		ofDrawBitmapStringHighlight(name, 0, 0, ofColor::black, ofColor::white);
		
		ofTranslate(1, 7);

		// bg panel box
		ofSetColor(0, 255);
		ofRectangle _rbg(-_p, -_p, (palette.size() *  sz) + _pp, 2 * (sz * szhr) + _pp);
		//ofDrawRectRounded(_rbg, 3);
		ofDrawRectangle(_rbg);

		// 1st row

		// the palette colors
		ofPushMatrix();
		for (auto p : palette)
		{
			ofSetColor(p);
			ofDrawRectangle(0, 0, sz2, sz * szhr);
			ofTranslate(sz, 0);
		}
		ofPopMatrix();

		// 2nd row
		
		ofTranslate(0, sz * szhr);

		// the picker color
		ofSetColor(colorPick);
		ofDrawRectangle(0, 0, sz2, sz2 * szhr);

		// the bg color
		ofSetColor(colorBg);
		if (palette.size() > 0) ofDrawRectangle(sz, 0, sz * (palette.size() - 1), sz2 * szhr);
		else ofDrawRectangle(sz, 0, sz, sz2 * szhr);
	}
	ofPopMatrix();
	ofPopStyle();
}