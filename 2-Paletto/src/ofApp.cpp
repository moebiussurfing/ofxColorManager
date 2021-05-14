#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	// we subscribe the local colors
	// using pointers/references
	// to be auto-updated

	colorManager.setLinkPalette(palette); // palette colors
	colorManager.setLinkColorPick(colorPick); // picked color 

	colorManager.setup();
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo info) {
	colorManager.dragEvent(info);
}