#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_NOTICE);
    ofDisableArbTex();
    ofSetBackgroundAuto(false);
    ofSetVerticalSync(true);
    ofSetFrameRate(60);

	ColorManager.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
	ColorManager.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ColorManager.draw();
}

//--------------------------------------------------------------
void ofApp::exit(){
	ColorManager.exit();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
