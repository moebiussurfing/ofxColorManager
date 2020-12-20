#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	//macOS: encapsulates /data folder into .app file
    //ofSetDataPathRoot("../Resources/data/");

    //ofSetLogLevel(OF_LOG_SILENT);
    ofSetLogLevel(OF_LOG_NOTICE);

    ofEnableAlphaBlending();

    float fps = 60;
    ofSetFrameRate(fps);

    ColorManager.setup();
    ColorManager.setFps(fps);
}

//--------------------------------------------------------------
void ofApp::update()
{
}

//--------------------------------------------------------------
void ofApp::draw()
{
}

//--------------------------------------------------------------
void ofApp::exit()
{
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    if (key == 'h')
    {
        hide = !hide;
        ColorManager.setVisible(!hide);
    }

    if (key == OF_KEY_TAB)
    {
        palette = ColorManager.getPalette();
        ofLogNotice("ofApp") << "getPalette";

        for (int i = 0; i < palette.size(); i++)
        {
            ofLogNotice("ofApp") << ofToString(palette[i]);
        }
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
    ColorManager.windowResized(w, h);
}