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
void PresetPalette::setBackgroundColor(ofColor _background)
{
    backCol = _background;
}

////--------------------------------------------------------------
//void PresetPalette::setBackgroundColor(ofColor &_background)
//{
//    background_BACK = &_background;
//}

////--------------------------------------------------------------
//void PresetPalette::setBackgroundColor(ofParameter<ofFloatColor> &_background)
////void PresetPalette::setBackgroundColor(ofColor &_background)
//{
//    background_BACK = &_background;
//}


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
    ofLogNotice("PresetPalette::preset_load") << p;
    string path = preset_path+p+".json";

    ofFile file(path);
    if (file.exists())
    {
        jsonin ji(file);
        ji >> presetData;

        ofLogNotice("PresetPalette::preset_load") << "presetData.name      : " << presetData.name;
        ofLogNotice("PresetPalette::preset_load") << "presetData.curveName : " << presetData.curveName;
        ofLogNotice("PresetPalette::preset_load") << "presetData.background: " << presetData.background;

        //        curvesTool.load(preset_path + presetData.curveName + ".yml");
        //
        //        if (color_backGround_SETAUTO)
        //        {
        //            color_backGround = presetData.background;
        //        }

        ofLogNotice("PresetPalette::preset_load") << "presetData.palette.size(): " << presetData.palette.size();

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
        ofLogNotice("PresetPalette::preset_load") << "FILE '" << path << "' NOT FOUND";
    }
}


//--------------------------------------------------------------
void PresetPalette::preset_save(string p)
{
    ofLogNotice("PresetPalette::preset_save") << "preset_save: " << p;
    string path = preset_path+p+".json";

    presetData.background = backCol;
    //    presetData.background = (*background_BACK);

    presetData.name = (*name_BACK);
    presetData.curveName = (*curveName_BACK);
    presetData.palette = (*palette_BACK);

    ofLogNotice("PresetPalette::preset_save") << "presetData.background: " << presetData.background;
    ofLogNotice("PresetPalette::preset_save") << "presetData.name      : " << presetData.name;
    ofLogNotice("PresetPalette::preset_save") << "presetData.curveName : " << presetData.curveName;
    ofLogNotice("PresetPalette::preset_save") << "presetData.palette   : " << ofToString(presetData.palette);

    //    curvesTool.save(preset_path+presetData.curveName+".yml");

    ofFile file(path, ofFile::WriteOnly);
    jsonout jo(file);
    jo << presetData;

    ofLogNotice("PresetPalette::preset_save") << "DONE! preset_save: " << p;
    cout << endl;
}



