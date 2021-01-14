//
// Created by moebiussurfing on 7/31/19.
//

#pragma once
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
	std::string name;
	vector<ofColor> palette;
	ofColor background;

	OFX_CEREAL_DEFINE(CEREAL_NVP(name), CEREAL_NVP(palette), CEREAL_NVP(background))
};

struct PresetData
{
	std::string namePreset;
	vector<ofColor> palette;
	ofColor background;
	std::string nameCurve;

	OFX_CEREAL_DEFINE(CEREAL_NVP(namePreset), CEREAL_NVP(nameCurve), CEREAL_NVP(palette), CEREAL_NVP(background))
};

//TODO: if the json file is modified by hand will be crash on parsing..

//----

class PresetPalette
{

public:

	PresetPalette() 
	{
		path_Global = "ofxColorManager/";

		path_Kits = "kits/";
		path_Presets = path_Global + path_Kits + "presets/";
		path_Palettes = path_Global + path_Kits + "palettes/";

		ofxSurfingHelpers::CheckFolder(path_Global + path_Kits);
		ofxSurfingHelpers::CheckFolder(path_Global);
		ofxSurfingHelpers::CheckFolder(path_Presets);
		ofxSurfingHelpers::CheckFolder(path_Palettes);
	};

	~PresetPalette() {};

	std::string path_Global;
	std::string path_Kits;
	std::string path_Presets;
	std::string path_Palettes;

	//-

	// local data
	std::string name;
	vector<ofColor> palette;
	std::string nameCurve;
	ofColor background;

	//--

	// out-of-scope (ofApp) data by pointers
	std::string *name_BACK;
	vector<ofColor> *palette_BACK;
	std::string *curveName_BACK;
	ofColor *background_BACK;

	ofColor color_BACK;
	//TODO temp because not pointer

	//--

	// API

	void setName(std::string &name);
	void setCurveName(std::string &curve);
	void setPalette(vector<ofColor> &palette);
	void setBackgroundColor(ofColor _background);

	vector<ofColor> getPalette();
	ofColor getBackground();

	//--

	// JSON PALETTES SERIALIZER

	// preset (colors, gradient, curve)
	PresetData presetData;//bundle preset: palette+curve+gradient+background

	void preset_save(std::string p, bool absolutePath = false);
	bool preset_load(std::string p);

	PresetData preset_LoadPalette(std::string name);//TODO: not elegant..

	//--

	// palette (only colors)
	PaletteData paletteData;
	void palette_save(std::string p);
	void palette_load(std::string p);

	//-
};