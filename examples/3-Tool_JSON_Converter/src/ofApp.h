#pragma once

#include "ofMain.h"


/*

	NOTE

	Example tool to convert JSON files templates that have palettes configurations.
	You should use this as a template and to convert your code to import other templated JSON files with palettes of colors.
	You will generate palette presets files compatible with Paletto.
	Also you can generate compatible colors libs like the bundled Pantone library.

*/


#include "ofxSerializer.h"

/*
 These string to hex conversions aren't trivial.
 */
 //--------------------------------------------------------------
static int stringToHex(string hex) {
	int aHex;
	stringstream convert(hex);
	convert >> std::hex >> aHex;
	return aHex;
}
//--------------------------------------------------------------
static void hexToColor(ofColor &col, string hex) { // must remember to remove # char!
	string r = hex.substr(0, 2);
	int ri = stringToHex(r);
	string g = hex.substr(2, 2);
	int gi = stringToHex(g);
	string b = hex.substr(4, 2);
	int bi = stringToHex(b);
	col.set(ri, gi, bi);
}

// tricky types that could be simplified...
//--------------------------------------------------------------
class paletteType {
public:
	vector<ofColor> palette;
};

//--------------------------------------------------------------
class kitType {
public:
	vector<paletteType> palettes;
};

//--------------------------------------------------------------
class ofApp : public ofBaseApp {

public:

	kitType kit;

	void setup();
	void update();
	void draw();

	void keyPressed(int key);

	ofJson js;

	ofTrueTypeFont ttf;
	std::string text;
	std::stringstream ss;
};
