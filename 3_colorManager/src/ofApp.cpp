#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableAlphaBlending();
    ofSetFrameRate(30);
    ofSetLogLevel(OF_LOG_NOTICE);

    ColorManager.setup();

//    ColorManager.setColor_TARGET(color_TARGET);
//    control = 0;
}


//--------------------------------------------------------------
void ofApp::update(){
    ColorManager.update();

//    float speed = 0.02;
//
////    if (direction)
////        control += speed;
////    else
////        control -= speed;
////    if (control > 1.)
////        direction = false;
////    if (control <= 0.)
////        direction = true;
//
//    control += speed;
//    if (control > 1.)
//        control = 0.;
//
//    control = ofClamp(control, 0., 1.);
//    ColorManager.setControl(control);
}

//--------------------------------------------------------------
void ofApp::draw(){

    ColorManager.draw();

//    if (!hide)
//    {
//        //mini box color on top of addon drawing gui
//        int x, y, w, h;
//        x = 1200;
//        y = 650;
//        w = h = 100;
//        ofRectangle r (0, 0, w, h);
//        ofPushMatrix();
//        ofPushStyle();
//        ofTranslate(x, y);
//        ofSetColor(color_TARGET);
//        ofFill();
//        ofDrawRectangle(r);
//        ofTranslate(0, h + 20);
//        ofDrawBitmapStringHighlight(ofToString(control), 0, 0, ofColor::black, ofColor::white);
//        ofPopStyle();
//        ofPopMatrix();
//    }
//    else
//    {
        //full screen color preview
//        ofBackground(color_TARGET);
//
//        //debug control value
////        ofDrawBitmapStringHighlight(ofToString(control), 0, 20, ofColor::black, ofColor::white);
//
//        //progress bar
//        ofPushStyle();
//        ofColor c;
//        c = (ofColor::black, 32);
//        ofSetColor(c);
//        ofFill();
//        ofDrawRectangle(ofRectangle(0, 0, control*(ofGetWidth()), 6));
//        ofPopStyle();
//    }
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
