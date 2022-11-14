#pragma once

//--------
//
// OPTIONS
//

// this file is kind of of a settings file
// that will be loaded from some of the app classes
// to initializa the class/addons with the same settings

//#define MAKE_RELEASE_VERSION

//----

#define GUI_LOCK_BORDERS_PAD 20

#ifdef MAKE_RELEASE_VERSION 
#define APP_RELEASE_NAME "SceneCOLORiZED"
#else
#define APP_RELEASE_NAME "ofxColorClient"
#endif

//
//---------