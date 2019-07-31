//
// Created by moebiussurfing on 7/31/19.
//

#ifndef INC_3_COLORMANAGER_PRESETPALETTE_H
#define INC_3_COLORMANAGER_PRESETPALETTE_H

#include "ofMain.h"

#include "ofxColourLoversHelper.h"
#include "ofxImGui.h"
#include "ofxColorGradient.h"
#include "ofxCurvesTool.h"
#include "ofxCereal.h"

//#include "ofxMouseRuler.h"
//#include "ofxInterface.h"
//#include "ButtonPaletteSelector.h"
//#include "ButtonExample.h"
//#include "ofxColorPalette.h"
//#include "ofxColorsBrowser.h"
//#include "ofxSimpleSlider.h"

//--

// JSON SERIALIZER

using namespace ofxCereal;

//struct CustomData
//{
//    string name;
//    vector<ofColor> palette;
//
//OFX_CEREAL_DEFINE(CEREAL_NVP(name), CEREAL_NVP(palette))
//};

struct PresetData
{
    string name;
    vector<ofColor> palette;
    string curveName;
    ofColor background;

OFX_CEREAL_DEFINE(CEREAL_NVP(name), CEREAL_NVP(curveName), CEREAL_NVP(palette), CEREAL_NVP(background))
};

//--

class PresetPalette {

    //--

    // JSON PALETTES SERIALIZER

//    CustomData data;//palette
//    void palette_save(string p);
//    void palette_load(string p);
//    string path_palettes = "assets/palettes/";

    PresetData presetData;//bundle preset: palette+curve+gradient+background
    void preset_save(string p);
    void preset_load(string p);
    string preset_path = "assets/presets/";

    //--

};


#endif //INC_3_COLORMANAGER_PRESETPALETTE_H
