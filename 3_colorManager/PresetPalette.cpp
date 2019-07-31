////
//// Created by moebiussurfing on 7/31/19.
////
//
#include "PresetPalette.h"
//
//--------------------------------------------------------------
void PresetPalette::preset_load(string p)
{
//    ofLogNotice("ofxColorManager:preset_load") << p;
//    string path = preset_path+p+".json";
//
//    ofFile file(path);
//    if (file.exists())
//    {
//        jsonin ji(file);
//        ji >> presetData;
//
//        ofLogNotice("ofxColorManager:preset_load") << "presetData.name      : " << presetData.name;
//        ofLogNotice("ofxColorManager:preset_load") << "presetData.curveName : " << presetData.curveName;
//        ofLogNotice("ofxColorManager:preset_load") << "presetData.background: " << presetData.background;
//
//        curvesTool.load(preset_path + presetData.curveName + ".yml");
//
//        if (color_backGround_SETAUTO)
//        {
//            color_backGround = presetData.background;
//        }
//
//        ofLogNotice("ofxColorManager:preset_load") << "presetData.palette.size(): " << presetData.palette.size();
//
//        palette_clear();
//
//        for (int i=0; i<presetData.palette.size(); i++)
//        {
//            ofColor c;
//            c = presetData.palette[i];
//            ofLogNotice("ofxColorManager:preset_load") << "addColor:" << ofToString(c) <<" ["<<i<<"]";
//            palette_addColor(c);
//        }
//        ofLogNotice("ofxColorManager:preset_load") << "DONE! preset_load  : " << p;
//        ofLogNotice("ofxColorManager:preset_load") << "palette.size()     :" << palette.size()<<endl;
//        ofLogNotice("ofxColorManager:preset_load") << "btns_palette.size():" << btns_palette.size()<<endl;
//    }
//    else
//    {
//        ofLogNotice("ofxColorManager:preset_load") << "FILE '" << path << "' NOT FOUND";
//    }
}


//--------------------------------------------------------------
void PresetPalette::preset_save(string p)
{
//    ofLogNotice("ofxColorManager:preset_save") << "preset_save: " << p;
//    string path = preset_path+p+".json";
//
//    presetData.name = "myPreset";//TODO:
//    presetData.curveName = "curve01";//TODO:
//    presetData.background = color_backGround.get();
//
//    ofLogNotice("ofxColorManager:preset_save") << "presetData.name      : " << presetData.name;
//    ofLogNotice("ofxColorManager:preset_save") << "presetData.curveName : " << presetData.curveName;
//    ofLogNotice("ofxColorManager:preset_save") << "presetData.background: " << presetData.background;
//
//    curvesTool.save(preset_path+presetData.curveName+".yml");
//
//    presetData.palette.resize(palette.size());
//    for (int i = 0; i< palette.size(); i++)
//    {
//        presetData.palette[i] = palette[i];
//        ofLogNotice("ofxColorManager:preset_save") << "presetData.palette[" << i << "]: " << ofToString(presetData.palette[i]);
//    }
//
//    ofFile file(path, ofFile::WriteOnly);
//    jsonout jo(file);
//    jo << presetData;
//
//    ofLogNotice("ofxColorManager:preset_save") << "DONE! preset_save: " << p;
}
