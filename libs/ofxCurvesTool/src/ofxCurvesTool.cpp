#include "ofxCurvesTool.h"

ofxCurvesTool::ofxCurvesTool()
:mouseX(0)
,mouseY(0)
,hoverState(false)
,dragState(false)
,curHover(0)
,focus(false)
,drawing(false)
,bKey(true)
,bMouse(true)
,mouseAddsPoint(true)
,mouseMovesPoint(true)
,drawn(false)
,keepFocus(false) {
	ofAddListener(ofEvents().draw, this, &ofxCurvesTool::drawEvent);
	labelPosition = ofPoint(4,18);
    

}

glm::mat4 modelviewMatrix, projectionMatrix;
ofRectangle viewport;
void updateProjectionState() {
	modelviewMatrix = ofGetCurrentMatrix(OF_MATRIX_MODELVIEW);
	projectionMatrix = ofGetCurrentMatrix(OF_MATRIX_PROJECTION);
	viewport = ofGetCurrentViewport();
}

ofVec3f worldToScreen(ofVec3f world) {
	updateProjectionState();
	glm::vec4 viewport_ = glm::vec4(viewport.x, viewport.y, viewport.width, viewport.height);
	glm::vec3 screen = glm::project((glm::vec3) world, modelviewMatrix, projectionMatrix, viewport_);
	screen.y = ofGetHeight() - screen.y;
	return screen;
}

//void ofxCurvesTool::draw(int x, int y) {
//    
//}

void ofxCurvesTool::draw(int x, int y, float index, int _alpha) {
    draw(x,y,int(index), _alpha);
}

void ofxCurvesTool::draw(int x, int y, int index, int _alpha) {
	drawn = true; // we've made a call to draw
	
	if (keepFocus){
		focus = true;
		hoverState = true;
	}

	ofPushStyle();
	ofPushMatrix();
	ofTranslate(x, y);
	
	drawPosition = worldToScreen(ofVec2f(0, 0));
	
	//drawPosition = ofVec3f(x,y,0);

	ofPushMatrix();
	ofTranslate(0, n);
	ofScale(1, -1);
	
	
	ofSetColor(ofColor(ofColor::black, _alpha));
	ofFill();
	ofDrawRectangle(0, 0, n, n);
    
	
	// grid
	ofSetColor(50);
	for(int i = 0; i < n; i += 64) {
		ofDrawLine(0, i, n, i);
		ofDrawLine(i, 0, i, n);
	}
	
	// diagonal, crosshairs
	ofSetColor(100);
	ofVec2f cur;
	if(hoverState || dragState) {
		cur = controlPoints[curHover];
	} else {
		cur = ofVec2f(mouseX, lut[(int) mouseX]);
	}
	ofDrawLine(0, 0, n, n);
	if(focus) {
		ofDrawLine(0, cur.y, n, cur.y);
		ofDrawLine(cur.x, 0, cur.x, n);
	}
	
	// outline
	ofSetColor(ofColor::white);
	ofNoFill();
	ofDrawRectangle(.5, .5, n - 1, n - 1);
	
	// curve
	ofNoFill();
	ofBeginShape();
	for(int x = 0; x < n; x++) {
		ofVertex(x, lut[x]);
	}
	ofEndShape();
	
	// control points
	int m = controlPoints.size();
	for(int i = 0; i < m; i++) {
		ofPushMatrix();
		ofVec2f& cur = controlPoints[i];
		ofTranslate((int) cur.x, (int) cur.y);
		if(hoverState && curHover == i) {
			if(dragState) {
				ofFill();
			} else {
				ofNoFill();
			}
			ofDrawCircle(0, 0, 3);
		} else {
			ofFill();
			ofDrawCircle(0, 0, 2);
		}
		ofPopMatrix();
	}
	ofPopMatrix();
	
    
    if(index > -1){
        ofSetColor(255);
        ofPushMatrix();
        ofTranslate(index,n - lut[(int) index]);
        ofDrawCircle(0, 0, 5);
        ofPopMatrix();
    }
    
	// info for current point
	if(focus) {
		string label = ofToString(curHover) + ": " + ofToString((int) cur.x) + ", " + ofToString((int) cur.y);
		ofDrawBitmapString(label, labelPosition.x, labelPosition.y);
	}
	ofPopMatrix();
	ofPopStyle();
}

void ofxCurvesTool::save(string filename) {
	ofFile out(filename, ofFile::WriteOnly);
	out << "[";
	int m = controlPoints.size();
	for(int i = 0; i < m; i++) {
		ofVec2f& cur = controlPoints[i];
		out << "[" << (int) cur.x << "," << (int) cur.y << "]";
		if(i + 1 < m) {
			out << ",";
		}
	}
	out << "]";
}

// basic yml list-of-lists parser 
void ofxCurvesTool::load(string filename) {
	if(ofFile(filename).exists()) {
		string in = ofFile(filename).readToBuffer();
		ofStringReplace(in, " ", "");
		vector<string> all = ofSplitString(in, "],[");
		controlPoints.clear();
		for(int i = 0; i < all.size(); i++) {
			ofStringReplace(all[i], "[", "");
			ofStringReplace(all[i], "]", "");
			vector<string> parts = ofSplitString(all[i], ",");
			add(ofVec2f(ofToFloat(parts[0]), ofToFloat(parts[1])));
		}
		update();
	}
}

void ofxCurvesTool::updateMouse(ofMouseEventArgs& args) {
	mouseX = args.x - drawPosition.x;
	mouseY = n - (args.y - drawPosition.y);
	focus = dragState;
	if(ofRectangle(0, 0, n, n).inside(mouseX, mouseY)) {
		focus = true;
	} else {
		hoverState = false;
	}
	mouseX = ofClamp(mouseX, 0, n - 1);
	mouseY = ofClamp(mouseY, 0, n - 1);
	int m = controlPoints.size();

	if(focus && !dragState) {
		hoverState = false;
		for(int i = 0; i < m; i++) {
			ofVec2f& cur = controlPoints[i];
			if(abs(cur.x - mouseX) < minDistance) {

				if (curHover != i){
					curHover = i;
					
                    if (bEvents){
                        ofVec3f xyi = ofVec3f(getPoint(curHover).x, getPoint(curHover).y, curHover);
                        ofNotifyEvent(curHoverChange, xyi);
                    }
				}

				hoverState = true;
			}
		}
	}
}

void ofxCurvesTool::mouseMoved(ofMouseEventArgs& args) {
	updateMouse(args);
}

void ofxCurvesTool::mousePressed(ofMouseEventArgs& args) {
	hoverState = false;

	updateMouse(args);
	if(focus) {
		if(!hoverState && mouseAddsPoint) {
			add(ofVec2f(mouseX, mouseY));
			updateMouse(args);
		}
		dragState = true;
		mouseDragged(args);
	}
}

void ofxCurvesTool::mouseDragged(ofMouseEventArgs& args) {
	updateMouse(args);
	if(dragState) {
		if (mouseMovesPoint){
			set(curHover, ofVec2f(mouseX, mouseY));
            if (bEvents){
                ofVec3f xyi = ofVec3f(getPoint(curHover).x, getPoint(curHover).y, curHover);
                ofNotifyEvent(curHoverUpdate, xyi);
            }
		}
	} else {
		focus = false;
	}
}

void ofxCurvesTool::mouseReleased(ofMouseEventArgs& args) {
	updateMouse(args);
	dragState = false;
}

void ofxCurvesTool::keyPressed(ofKeyEventArgs& args) {
	if(bKey && (args.key == OF_KEY_DEL || args.key == OF_KEY_BACKSPACE) && hoverState) {
		remove(curHover);
		hoverState = false;
		dragState = false;
	}
}

void ofxCurvesTool::mouseEntered(ofMouseEventArgs& args){}
void ofxCurvesTool::mouseExited(ofMouseEventArgs& args){}
void ofxCurvesTool::mouseScrolled(ofMouseEventArgs& args){}


/*
 this part controls whether events get through to the object or not. if the
 object is not drawn using draw(), we know here because draw() never set the
 drawn flag. in that case, we unregister events. if it changes from being off
 to on, then we register the events again.
 */
void ofxCurvesTool::drawEvent(ofEventArgs& args) {
	bool prevDrawing = drawing;
	drawing = drawn;
	if(drawing != prevDrawing) {
		if(drawing) {
			ofRegisterMouseEvents(this);
			ofRegisterKeyEvents(this);
		} else {
			ofUnregisterMouseEvents(this);
			ofUnregisterKeyEvents(this);
		}
	}
	drawn = false; // turn the drawn flag off, for draw() to turn back on
}

int ofxCurvesTool::getCurrentHover() {
	return curHover;
}

void ofxCurvesTool::setCurrentHover(int i) {
	if (curHover != i){
		curHover = i;
        if (bEvents){
            ofVec3f xyi = ofVec3f(getPoint(curHover).x, getPoint(curHover).y, curHover);
            ofNotifyEvent(curHoverChange, xyi);
        }
	}

	hoverState = true;
	focus = true;
}

float ofxCurvesTool::getAt(float float_index){
    

    float lower = lut[floor(float_index)];
    float upper = lut[ceil(float_index)];
    
//    ofLog()<<float_index<<" floor "<<floor(float_index)<<" ceil "<<ceil(float_index);
//    ofLog()<<float_index<<" lower "<<lower<<" upper "<<upper;
//    ofLog()<<"result "<<ofMap(float_index,floor(float_index),ceil(float_index),lower,upper,true);
    return ofMap(float_index,floor(float_index),ceil(float_index),lower,upper,true);
}

float ofxCurvesTool::getAtPercent(float i){
    
    i = i * (n-1);
    float lower = lut[floor(i)];
    float upper = lut[ceil(i)];
    
//        ofLog()<<i<<" floor "<<floor(i)<<" ceil "<<ceil(i);
//        ofLog()<<i<<" lower "<<lower<<" upper "<<upper;
//        ofLog()<<"result "<<ofMap(i,floor(i),ceil(i),lower,upper,true);
    return ofMap(i,floor(i),ceil(i),lower,upper,true);
}

void ofxCurvesTool::clear() {
	controlPoints.clear();
}

void ofxCurvesTool::setLabelPosition() {
	labelPosition = ofPoint(4,18);
}
void ofxCurvesTool::setLabelPosition(int x, int y) {
	labelPosition = ofPoint(x,y);
}

void ofxCurvesTool::useKey(bool b){
	bKey = b;
	if (b){
		ofRegisterKeyEvents(this);
	}
	else{
		ofUnregisterKeyEvents(this);
	}
}
void ofxCurvesTool::useMouse(bool b){
	bMouse = b;
	if (b){
		ofRegisterMouseEvents(this);
	}
	else{
		ofUnregisterMouseEvents(this);
	}
}

void ofxCurvesTool::nextPoint() {
	int size = getPointSize();
	if (getCurrentHover() + 1 < size-1)
		setCurrentHover(getCurrentHover() + 1);
	else
		setCurrentHover(size-1);
}

void ofxCurvesTool::prevPoint() {
	int size = getPointSize();
	if (getCurrentHover() - 1 > 0)
		setCurrentHover(getCurrentHover() - 1);
	else
		setCurrentHover(0);
}

void ofxCurvesTool::notifyEvents(bool b) {
    bEvents = b;
}
