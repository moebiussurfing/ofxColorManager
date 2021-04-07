#include "ofApp.h"

// hide console
#ifdef MAKE_RELEASE_VERSION
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

int main(){

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:

	ofSetupOpenGL(1920, 1080, OF_WINDOW);

	ofRunApp(new ofApp());
}
