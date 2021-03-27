#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
#ifdef USE_OFX_WINDOWAPP
	windowApp.setFrameRate(60);
	windowApp.setVerticalSync(true);
	windowApp.setShowDebug(false);
#else
	ofSetFrameRate(60);
#endif

	//--

	// we subscribe the local colors
	// using pointers/references
	// to be auto-updated

	colorManager.setLinkPalette(palette); // palette colors
	colorManager.setLinkColorPick(colorPick); // picked color 

	colorManager.setup();
}