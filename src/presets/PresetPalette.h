//
// Created by moebiussurfing on 7/31/19.
//

#ifndef INC_3_COLORMANAGER_PRESETPALETTE_H
#define INC_3_COLORMANAGER_PRESETPALETTE_H

#include "ofMain.h"

#include "ofxColorGradient.h"
#include "ofxCurvesTool.h"
#include "ofxCereal.h"
#include "ofxSurfingHelpers.h"

//----

// JSON SERIALIZER

using namespace ofxCereal;

struct PaletteData
{
	string name;
	vector<ofColor> palette;
	ofColor background;

	OFX_CEREAL_DEFINE(CEREAL_NVP(name), CEREAL_NVP(palette), CEREAL_NVP(background))
};

struct PresetData
{
	string name;
	vector<ofColor> palette;
	string curveName;
	ofColor background;

	OFX_CEREAL_DEFINE(CEREAL_NVP(name), CEREAL_NVP(curveName), CEREAL_NVP(palette), CEREAL_NVP(background))
};

//----

class PresetPalette
{

public:

	PresetPalette() {
		path_Global = "ofxColorManager/";

		//path_Presets = path_Global + "user_kits/presets/";
		//path_Palettes = path_Global + "user_kits/palettes/";

		path_Kits = "kits/";
		path_Presets = path_Global + path_Kits + "presets/";
		path_Palettes = path_Global + path_Kits + "palettes/";

		ofxSurfingHelpers::CheckFolder(path_Global + path_Kits);
		ofxSurfingHelpers::CheckFolder(path_Global);
		ofxSurfingHelpers::CheckFolder(path_Presets);
		ofxSurfingHelpers::CheckFolder(path_Palettes);
	};

	~PresetPalette() {};

	//string path_Presets = "user_kits/presets/";
	//string path_Palettes = "user_kits/palettes/";
	string path_Global;
	string path_Kits;
	string path_Presets;
	string path_Palettes;

	//-

	// local data
	string name;
	vector<ofColor> palette;
	string curveName;
	ofColor background;

	//--

	// outside data by pointers
	string *name_BACK;
	vector<ofColor> *palette_BACK;
	string *curveName_BACK;
	ofColor *background_BACK;

	ofColor backCol;//TODO temp because not pointer

	//--

	// API

	void setName(string &name);
	void setCurveName(string &curve);
	void setPalette(vector<ofColor> &palette);
	void setBackgroundColor(ofColor _background);

	vector<ofColor> getPalette();
	ofColor getBackground();

	//--

	// JSON PALETTES SERIALIZER

	// preset (colors, gradient, curve)
	PresetData presetData;//bundle preset: palette+curve+gradient+background

	void preset_save(string p);
	void preset_load(string p);

	//--

	// palette (only colors)
	PaletteData paletteData;
	void palette_save(string p);
	void palette_load(string p);

	//-
};

#endif //INC_3_COLORMANAGER_PRESETPALETTE_H
