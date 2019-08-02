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

struct PresetData_
{
    string name;
    vector<ofColor> palette;
    string curveName;
    ofColor background;

OFX_CEREAL_DEFINE(CEREAL_NVP(name), CEREAL_NVP(curveName), CEREAL_NVP(palette), CEREAL_NVP(background))
};

//--

class PresetPalette {

public:

    string name;
    vector<ofColor> palette;
    string curveName;
    ofColor background;

    string *name_BACK;
    vector<ofColor> *palette_BACK;
    string *curveName_BACK;
//    ofColor *background_BACK;
//    ofFloatColor *background_BACK;

    void setName(string &name);
    void setCurveName(string &curve);
//    void setBackgroundColor(ofColor &background);
//    void setBackgroundColor(ofParameter<ofFloatColor> &background);
    void setPalette(vector<ofColor> &palette);

    string preset_path = "assets/presetsCLASS/";

    //--

    // JSON PALETTES SERIALIZER

//    CustomData data;//palette
//    void palette_save(string p);
//    void palette_load(string p);
//    string path_palettes = "assets/palettes/";

    PresetData_ presetData;//bundle preset: palette+curve+gradient+background
    void preset_save(string p);
    void preset_load(string p);

    //--

};


#endif //INC_3_COLORMANAGER_PRESETPALETTE_H
