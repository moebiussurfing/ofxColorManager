//
// Created by moebiussurfing on 7/31/19.
//

#ifndef INC_3_COLORMANAGER_PRESETPALETTE_H
#define INC_3_COLORMANAGER_PRESETPALETTE_H

#include "ofMain.h"

#include "ofxColorGradient.h"
#include "ofxCurvesTool.h"
#include "ofxCereal.h"

//--

// JSON SERIALIZER

using namespace ofxCereal;

struct PaletteData
{
    string name;
    vector<ofColor> palette;

OFX_CEREAL_DEFINE(CEREAL_NVP(name), CEREAL_NVP(palette))
};

struct PresetData_ {
    string name;
    vector<ofColor> palette;
    string curveName;
    ofColor background;

OFX_CEREAL_DEFINE(CEREAL_NVP(name), CEREAL_NVP(curveName), CEREAL_NVP(palette), CEREAL_NVP(background))
};

//--

class PresetPalette {

public:

    // constants

    string preset_path = "user_kits/presets/";
    string path_palettes = "user_kits/palettes/";

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

    ofColor backCol;

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

    // palette (only colors)

//    PaletteData data;//palette
//    void palette_save(string p);
//    void palette_load(string p);
//    string path_palettes = "assets/palettes/";


    // preset (colors, gradient, curve)

    PresetData_ presetData;//bundle preset: palette+curve+gradient+background
    void preset_save(string p);

    void preset_load(string p);

    //--
    void palette_save(string p);

    void palette_load(string p);



};


#endif //INC_3_COLORMANAGER_PRESETPALETTE_H
