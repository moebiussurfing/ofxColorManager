#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableAlphaBlending();
    ofSetFrameRate(30);
    ofSetLogLevel(OF_LOG_NOTICE);

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

    if (key == 'h')
    {
        hide = !hide;
        ColorManager.setVisible(!hide);
    }

    if (key == OF_KEY_TAB)
    {
        palette = ColorManager.getPalette();
        ofLogNotice("ofApp") << "getPalette";

        for (int i=0; i<palette.size(); i++)
        {
            ofLogNotice("ofApp") << ofToString(palette[i]);
        }
    }
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
