#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_NOTICE);
    ofSetFrameRate(30);

    ColorManager.setup();

    ColorManager.setColor_TARGET(color_TARGET);
    control = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
    ColorManager.update();

    if (direction)
        control += 0.02;
    else
        control -= 0.02;
    if (control > 1.)
        direction = false;
    if (control <= 0.)
        direction = true;
    control = ofClamp(control, 0., 1.);
    ColorManager.setControl(control);
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(16);

//    if (!hide)
//    {
        ColorManager.draw();
//    }

    int x, y, w, h;
    x = 1200;
    y = 650;
    w = h = 100;
    ofRectangle r (0, 0, w, h);

    ofPushMatrix();
    ofPushStyle();

    ofTranslate(x, y);
    ofSetColor(color_TARGET);
    ofFill();
    ofDrawRectangle(r);

    ofTranslate(0, h + 20);
    ofDrawBitmapStringHighlight(ofToString(control), 0, 0, ofColor::white, ofColor::black);

    ofPopStyle();
    ofPopMatrix();

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
