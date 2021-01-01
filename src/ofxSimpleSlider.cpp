/*
 *  ofxSimpleSlider.cpp
 *  Created by Golan Levin on 2/24/12.
 *  Updated by Dan Wilcox 2012.
 */

#include "ofxSimpleSlider.h"

 //----------------------------------------------------
ofxSimpleSlider::ofxSimpleSlider() {
	bWasSetup = false;
}

//----------------------------------------------------
ofxSimpleSlider::~ofxSimpleSlider() {
	clear();
}

//-----------------------------------------------------------------------------------------------------------------------
void ofxSimpleSlider::setup(float inx, float iny, float inw, float inh, float loVal, float hiVal, float initialValue, bool bVert, bool bDrawNum) {
	bWasSetup = false; 
	clear();

	x = inx;
	y = iny;
	width = inw;
	height = inh;
	box.set(x, y, width, height);
	numberDisplayPrecision = 2;

	bVertical = bVert;
	bDrawNumber = bDrawNum;
	bHasFocus = false;

	lowValue = loVal;
	highValue = hiVal;
	percent = ofMap(initialValue, lowValue, highValue, 0, 1);
	percent = ofClamp(percent, 0, 1);

	labelString = "";

	if (!bWasSetup) {
		ofAddListener(ofEvents().draw, this, &ofxSimpleSlider::draw);
		ofAddListener(ofEvents().mouseMoved, this, &ofxSimpleSlider::mouseMoved);
		ofAddListener(ofEvents().mousePressed, this, &ofxSimpleSlider::mousePressed);
		ofAddListener(ofEvents().mouseReleased, this, &ofxSimpleSlider::mouseReleased);
		ofAddListener(ofEvents().mouseDragged, this, &ofxSimpleSlider::mouseDragged);
		bWasSetup = true;
	}
}

//----------------------------------------------------
void ofxSimpleSlider::clear() {
	if (bWasSetup) {
		ofRemoveListener(ofEvents().draw, this, &ofxSimpleSlider::draw);
		ofRemoveListener(ofEvents().mouseMoved, this, &ofxSimpleSlider::mouseMoved);
		ofRemoveListener(ofEvents().mousePressed, this, &ofxSimpleSlider::mousePressed);
		ofRemoveListener(ofEvents().mouseReleased, this, &ofxSimpleSlider::mouseReleased);
		ofRemoveListener(ofEvents().mouseDragged, this, &ofxSimpleSlider::mouseDragged);
	}
	bWasSetup = false;
}

//----------------------------------------------------
void ofxSimpleSlider::setLabelString(string str) {
	labelString = str;
}


//----------------------------------------------------
void ofxSimpleSlider::draw(ofEventArgs& event) {

	if (isEnabled) {

		ofPushStyle();

		ofPushMatrix();
		ofTranslate(x, y, 0);

		// Use different alphas if we're actively maniupulating me.
		float sliderAlpha = (bHasFocus) ? 128 : 64;
		float spineAlpha = (bHasFocus) ? 192 : 128;
		float thumbAlpha = (bHasFocus) ? 255 : 200;

		// draw box outline
		ofNoFill();
		ofSetLineWidth(1.0);
		ofSetColor(200, 200, 200, sliderAlpha);
		//ofSetColor(200, 200, 200);
		ofSetRectMode(OF_RECTMODE_CORNER);
		ofDrawRectangle(0, 0, width, height);

		// draw spine
		ofSetLineWidth(1.0);
		ofSetColor(255, 255, 255, spineAlpha);
		//ofSetColor(255, 255, 255);
		if (bVertical) {
			ofDrawLine(width / 2, 0, width / 2, height);
		}
		else {
			ofDrawLine(0, height / 2, width, height / 2);
		}

		// draw thumb
		ofSetLineWidth(5.0);
		ofSetColor(255, 255, 255, thumbAlpha);
		//ofSetColor(255, 255, 255);
		if (bVertical) {
			float thumbY = ofMap(percent, 0, 1, height, 0, true);
			ofDrawLine(0, thumbY, width, thumbY);
		}
		else {
			float thumbX = ofMap(percent, 0, 1, 0, width, true);
			ofDrawLine(thumbX, 0, thumbX, height);
		}

		////move label up left
		//ofTranslate(-77, -7);
		////move label down left
		//ofTranslate(-32, 33);
		////move label down left
		//ofTranslate(-30, 20);
		ofTranslate(10, -7);

		for (int i = 0; i < 2; i++) {
			if (bHasFocus) {
				if (i == 0) ofSetColor(0, 255);
				else if (i == 1) {
					ofSetColor(255, 255);
					ofTranslate(-1, -2);
				}
			}
			else {
				if (i == 0) ofSetColor(0, 200);
				else if (i == 1) {
					ofSetColor(255, 200);
					ofTranslate(-1, -2);
				}
			}

			// draw numeric value
			if (bDrawNumber) {
				if (bVertical) {
					ofDrawBitmapString(ofToString(getValue(), numberDisplayPrecision), width + 5, height);
					ofDrawBitmapString(labelString, width + 5, height - 14);
				}
				else {
					ofDrawBitmapString(ofToString(getValue(), numberDisplayPrecision), width + 5, height / 2 + 4);
					float labelStringWidth = labelString.size();
					ofDrawBitmapString(labelString, 0 - labelStringWidth * 8 - 5, height / 2 + 4);
				}
			}

		}
		ofPopMatrix();
		ofSetLineWidth(1.0);

		ofPopStyle();
	}
}

//----------------------------------------------------
float ofxSimpleSlider::getValue() {
	// THIS IS THE MAIN WAY YOU GET THE VALUE FROM THE SLIDER!
	float out = ofMap(percent, 0, 1, lowValue, highValue, true);
	return out;
}


//----------------------------------------------------
// Probably not used very much. 
float ofxSimpleSlider::getLowValue() {
	return lowValue;
}
float ofxSimpleSlider::getHighValue() {
	return highValue;
}
float ofxSimpleSlider::getPercent() {
	return percent;
}

//----------------------------------------------------
// Probably not used very much. 
void ofxSimpleSlider::setLowValue(float lv) {
	lowValue = lv;
}
void ofxSimpleSlider::setHighValue(float hv) {
	highValue = hv;
}
void ofxSimpleSlider::setPercent(float p) {
	// Set the slider's percentage from the outside. 
	p = ofClamp(p, 0, 1);
	percent = p;
}
void ofxSimpleSlider::setNumberDisplayPrecision(int prec) {
	numberDisplayPrecision = prec;
}

//----------------------------------------------------
void ofxSimpleSlider::mouseMoved(ofMouseEventArgs& event) {
	if (isEnabled)
		bHasFocus = false;
}
void ofxSimpleSlider::mouseDragged(ofMouseEventArgs& event) {
	if (isEnabled) {
		if (bHasFocus) {
			updatePercentFromMouse(event.x, event.y);
		}
	}
}
void ofxSimpleSlider::mousePressed(ofMouseEventArgs& event) {
	if (isEnabled) {
		bHasFocus = false;
		if (box.inside(event.x, event.y)) {
			bHasFocus = true;
			updatePercentFromMouse(event.x, event.y);
		}
	}
}
void ofxSimpleSlider::mouseReleased(ofMouseEventArgs& event) {
	if (isEnabled) {
		if (bHasFocus) {
			if (box.inside(event.x, event.y)) {
				updatePercentFromMouse(event.x, event.y);
			}
		}
		bHasFocus = false;
	}
}

//----------------------------------------------------
void ofxSimpleSlider::updatePercentFromMouse(int mx, int my) {
	if (isEnabled) {
		// Given the mouse value, compute the percentage.
		if (bVertical) {
			percent = ofMap(my, y, y + height, 1, 0, true);
		}
		else {
			percent = ofMap(mx, x, x + width, 0, 1, true);
		}
	}
}

//--------------------------------------------------------------
void ofxSimpleSlider::setVisible(bool b)
{
	isEnabled = b;
}

