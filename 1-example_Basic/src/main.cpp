#include "ofApp.h"

// hide console
#ifndef USE_DEBUG_LAYOUT
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

int main(){

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:

	ofSetupOpenGL(1920, 1080, OF_WINDOW);

	ofRunApp(new ofApp());
}
