//
// Created by moebiussurfing on 7/31/19.
//

#include "PresetPalette.h"


//--------------------------------------------------------------
void PresetPalette::setName(string &_name)
{
    name_BACK = &_name;
}


//--------------------------------------------------------------
void PresetPalette::setCurveName(string &_curve)
{
    curveName_BACK = &_curve;
}

//--------------------------------------------------------------
void PresetPalette::setBackgroundColor(ofColor _background)//TODO: now without pointer...
{
    backCol = _background;
}


//--------------------------------------------------------------
void PresetPalette::setPalette(vector<ofColor> &_palette)
{
    palette_BACK = &_palette;
}


//--------------------------------------------------------------
vector<ofColor> PresetPalette::getPalette()
{
    return (*palette_BACK);
}


//--------------------------------------------------------------
ofColor PresetPalette::getBackground()
{
    return presetData.background;
}


//--------------------------------------------------------------
void PresetPalette::preset_load(string p)
{
    ofLogNotice("PresetPalette") << "preset_load" << p;
    string path = preset_path + p + ".json";

    ofFile file(path);
    if (file.exists())
    {
        jsonin ji(file);
        ji >> presetData;

        ofLogNotice("PresetPalette") << "presetData.name: " << presetData.name;
        ofLogNotice("PresetPalette") << "presetData.curveName: " << presetData.curveName;
        ofLogNotice("PresetPalette") << "presetData.background: " << presetData.background;

        //        curvesTool.load(preset_path + presetData.curveName + ".yml");
        //
        //        if (color_backGround_SETAUTO)
        //        {
        //            color_backGround = presetData.background;
        //        }

        ofLogNotice("PresetPalette") << "presetData.palette.size(): " << presetData.palette.size();

        //        palette_clear();
        //        for (int i=0; i<presetData.palette.size(); i++)
        //        {
        //            ofColor c;
        //            c = presetData.palette[i];
        //            ofLogNotice("PresetPalette::preset_load") << "addColor:" << ofToString(c) <<" ["<<i<<"]";
        //            palette_addColor(c);
        //        }

        //        ofLogNotice("PresetPalette::preset_load") << "DONE! preset_load  :" << p;
        //        ofLogNotice("PresetPalette::preset_load") << "palette.size()     :" << palette.size()<<endl;
        //        ofLogNotice("PresetPalette::preset_load") << "btns_palette.size():" << btns_palette.size()<<endl;


        (*name_BACK) = presetData.name;
        (*curveName_BACK) = presetData.curveName;
        (*palette_BACK) = presetData.palette;
    }
    else
    {
        ofLogNotice("PresetPalette") << "FILE '" << path << "' NOT FOUND";
    }
}


//--------------------------------------------------------------
void PresetPalette::preset_save(string p)
{
    ofLogNotice("PresetPalette") << "preset_save: " << p;
    string path = preset_path + p + ".json";

    presetData.background = backCol;
    //presetData.background = (*background_BACK);

    presetData.name = (*name_BACK);
    presetData.curveName = (*curveName_BACK);
    presetData.palette = (*palette_BACK);

    ofLogNotice("PresetPalette") << "presetData.name: " << presetData.name;
    ofLogNotice("PresetPalette") << "presetData.curveName: " << presetData.curveName;
    ofLogNotice("PresetPalette") << "presetData.palette: " << ofToString(presetData.palette);
    ofLogNotice("PresetPalette") << "presetData.background: " << presetData.background;

    //curvesTool.save(preset_path+presetData.curveName+".yml");

    ofFile file(path, ofFile::WriteOnly);
    jsonout jo(file);
    jo << presetData;

    ofLogNotice("PresetPalette") << "DONE! preset_save: " << p;
    cout << endl;
}


//--------------------------------------------------------------
void PresetPalette::palette_load(string p)
{
    ofLogNotice("PresetPalette") << "palette_load: " << p;

    string path = path_palettes + p + ".json";
    ofFile file(path);
    if (file.exists())
    {
        jsonin ji(file);
        ji >> paletteData;

        ofLogNotice("PresetPalette") << "paletteData.name: " << paletteData.name;
        ofLogNotice("PresetPalette") << "paletteData.palette: " << ofToString(paletteData.palette);
        ofLogNotice("PresetPalette") << "paletteData.background: " << paletteData.background;

        (*name_BACK) = paletteData.name;
        (*palette_BACK) = paletteData.palette;
        backCol = paletteData.background;
        //+required pointer to be able to set background back!
    }
    else
    {
        ofLogNotice("PresetPalette") << "FILE '" << path << "' NOT FOUND";
    }
}


//--------------------------------------------------------------
void PresetPalette::palette_save(string p)
{
    ofLogNotice("PresetPalette") << "palette_save: " << p;
    string path = path_palettes + p + ".json";

    paletteData.name = (*name_BACK);
    paletteData.palette = (*palette_BACK);
    paletteData.background = backCol;

    ofLogNotice("PresetPalette") << "paletteData.name: " << paletteData.name;
    ofLogNotice("PresetPalette") << "paletteData.palette: " << ofToString(paletteData.palette);
    ofLogNotice("PresetPalette") << "paletteData.background: " << paletteData.background;

    ofFile file(path, ofFile::WriteOnly);
    jsonout jo(file);
    jo << paletteData;
}

