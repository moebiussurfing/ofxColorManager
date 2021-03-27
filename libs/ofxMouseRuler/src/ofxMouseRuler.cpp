#include "ofxMouseRuler.h"

ofxMouseRuler::ofxMouseRuler() {

	setupDone = false;
	visible = false;

	orig_x = 0;//coordinate axis
	orig_y = 0;
	pad_x = +20,//text info
		pad_y = +8;
}

void ofxMouseRuler::setOrig(int x, int y) {
	orig_x = x;
	orig_y = y;
}

void ofxMouseRuler::setup() {
	visible = true;
	ofRegisterMouseEvents(this);
	ofAddListener(ofEvents().draw, this, &ofxMouseRuler::draw, std::numeric_limits<int>::max());
	setupDone = true;
}

void ofxMouseRuler::draw(ofEventArgs &) {

	if (visible) {

		ofPushStyle();

		//ofPoint mPos(ofGetMouseX(), ofGetMouseY());
		ofPoint mPos(ofGetMouseX() - orig_x, ofGetMouseY() - orig_y);

		//draw awis
		if (bShowAvis)
		{
			ofSetColor(ofColor(ofColor::white), 64);
			ofSetLineWidth(1);
			ofDrawLine(0, ofGetMouseY(), ofGetWidth(), ofGetMouseY());
			ofDrawLine(ofGetMouseX(), 0, ofGetMouseX(), ofGetHeight());
		}

		std::string info;
		//if(!dragging)
		{
			info += "x: " + ofToString(mPos.x) + " y: " + ofToString(mPos.y);
		}
		if (dragging) {//distance line
			info += "\n";
			int _fn = ofGetFrameNum();
			int _sp = 10;
			bool _b = (_fn % _sp) > _sp / 2;
			if (_b) ofSetColor(255, 0, 0, 255);
			else ofSetColor(255, 0, 0, 128);
			ofSetLineWidth(1);

			//            ofDrawLine(mPos, dragStart);
			ofDrawLine(mPos + ofPoint(orig_x, orig_y), dragStart + ofPoint(orig_x, orig_y));

			info += ofToString((mPos - dragStart).length());
		}

		ofSetColor(255);
		ofFill();
		//ofDrawRectangle(font.getBoundingBox(info, mPos.x +12, mPos.y + 30));
		ofDrawRectangle(font.getBoundingBox(info, mPos.x + pad_x, mPos.y + pad_y));

		ofSetColor(0);
		//ofDrawBitmapString(info, mPos.x +12, mPos.y + 30);
		ofDrawBitmapString(info, mPos.x + pad_x, mPos.y + pad_y);

		ofPopStyle();
	}

}

void ofxMouseRuler::show() {
	visible = true;
}

void ofxMouseRuler::hide() {
	visible = false;
}

void ofxMouseRuler::toggleVisibility() {
	visible = !visible;
}

ofxMouseRuler::~ofxMouseRuler() {
	if (setupDone) {
		ofUnregisterMouseEvents(this);
		ofRemoveListener(ofEvents().draw, this, &ofxMouseRuler::draw, OF_EVENT_ORDER_AFTER_APP + 1);
	}
}


bool ofxMouseRuler::mousePressed(ofMouseEventArgs &args) {
	//	dragStart = ofPoint(args.x,args.y);
	dragStart = ofPoint(args.x - orig_x, args.y - orig_y);

	dragging = true;
	return false;
}

bool ofxMouseRuler::mouseReleased(ofMouseEventArgs &args) {
	dragging = false;
	return false;
}
