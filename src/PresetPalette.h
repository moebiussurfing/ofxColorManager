//
// Created by moebiussurfing on 7/31/19.
//

#ifndef INC_3_COLORMANAGER_PRESETPALETTE_H
#define INC_3_COLORMANAGER_PRESETPALETTE_H

#include "ofMain.h"

//#include "ofxColourLoversHelper.h"
#include "ofxImGui.h"
#include "ofxColorGradient.h"
#include "ofxCurvesTool.h"
#include "ofxCereal.h"

//--

// JSON SERIALIZER

using namespace ofxCereal;

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

    void setup();
    void update();
    void draw();

    //--

    // GUI

    ofxImGui::Gui gui2;
    bool gui_imGui();
    bool guiVisible;
    bool mouseOverGui;
    void gui_setup_layout();
    void gui_imGui_theme();

    //--

    string name;
    vector<ofColor> palette;
    string curveName;
    ofColor background;

    //--

    string *name_BACK;
    vector<ofColor> *palette_BACK;
    string *curveName_BACK;
    ofColor *background_BACK;
//    ofFloatColor *background_BACK;

    ofColor backCol;

//--

    void setName(string &name);
    void setCurveName(string &curve);
    void setPalette(vector<ofColor> &palette);

    void setBackgroundColor(ofColor _background);
//    void setBackgroundColor(ofColor &background);
//    void setBackgroundColor(ofParameter<ofFloatColor> &background);

    string preset_path = "assets/presetsCLASS/";

    vector<ofColor> getPalette();
    ofColor getBackground();

    //--

    // JSON PALETTES SERIALIZER

    // palette

//    CustomData data;//palette
//    void palette_save(string p);
//    void palette_load(string p);
//    string path_palettes = "assets/palettes/";


    // preset

    PresetData_ presetData;//bundle preset: palette+curve+gradient+background
    void preset_save(string p);
    void preset_load(string p);

    //--

};


#endif //INC_3_COLORMANAGER_PRESETPALETTE_H
