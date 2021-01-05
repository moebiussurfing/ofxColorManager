//
// Created by moebiussurfing on 7/31/19.
//

#include "PresetPalette.h"

//--------------------------------------------------------------
void PresetPalette::setName(std::string &_name)
{
	name_BACK = &_name;
}

//--------------------------------------------------------------
void PresetPalette::setCurveName(std::string &_curve)
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
	return palette;
	//return (*palette_BACK);
}

//--------------------------------------------------------------
ofColor PresetPalette::getBackground()
{
	return presetData.background;
}

//--------------------------------------------------------------
void PresetPalette::preset_load(std::string p)
{
	ofLogNotice(__FUNCTION__) << p;
	std::string path = path_Presets + p + ".json";

	ofFile file(path);

	if (file.exists())
	{
		jsonin ji(file);
		ji >> presetData;

		//ofLogNotice(__FUNCTION__) << "ji: " << endl << ji;
		//ofLogNotice(__FUNCTION__) << "presetData: " << endl << presetData;

		ofLogNotice(__FUNCTION__) << "-------- presetData --------";
		ofLogNotice(__FUNCTION__) << "name       : " << presetData.name;
		ofLogNotice(__FUNCTION__) << "curveName  : " << presetData.curveName;
		ofLogNotice(__FUNCTION__) << "background : " << presetData.background;

		//curvesTool.load(path_Presets + presetData.curveName + ".yml");
		//
		//if (color_BackGround_AutoSet)
		//{
		//    color_BackGround = presetData.background;
		//}

		ofLogNotice(__FUNCTION__) << "presetData.palette.size(): " << presetData.palette.size();

		palette.clear();
		for (int i = 0; i < presetData.palette.size(); i++)
		{
			ofColor c = presetData.palette[i];
			ofLogNotice(__FUNCTION__) << "+ color [" << i << "] " << ofToString(c);
			palette.push_back(c);
		}

		//ofLogNotice("PresetPalette::preset_load") << "DONE! preset_load  :" << p;
		//ofLogNotice("PresetPalette::preset_load") << "palette.size()     :" << palette.size()<<endl;
		//ofLogNotice("PresetPalette::preset_load") << "btns_palette.size():" << btns_palette.size()<<endl;

		(*name_BACK) = presetData.name;
		(*curveName_BACK) = presetData.curveName;
		(*palette_BACK) = presetData.palette;
	}
	else
	{
		ofLogNotice(__FUNCTION__) << "FILE '" << path << "' NOT FOUND";
	}
}

//--------------------------------------------------------------
void PresetPalette::preset_save(std::string p)
{
	ofLogNotice(__FUNCTION__) << p;
	std::string path = path_Presets + p + ".json";

	presetData.background = backCol;
	//presetData.background = (*background_BACK);

	presetData.name = (*name_BACK);
	presetData.curveName = (*curveName_BACK);
	presetData.palette = (*palette_BACK);

	ofLogNotice(__FUNCTION__) << "presetData.name: " << presetData.name;
	ofLogNotice(__FUNCTION__) << "presetData.curveName: " << presetData.curveName;
	ofLogNotice(__FUNCTION__) << "presetData.palette: " << ofToString(presetData.palette);
	ofLogNotice(__FUNCTION__) << "presetData.background: " << presetData.background;

	//curvesTool.save(path_Presets+presetData.curveName+".yml");

	ofFile file(path, ofFile::WriteOnly);
	jsonout jo(file);
	jo << presetData;

	ofLogNotice(__FUNCTION__) << "DONE! preset_save: " << p;
}

//--------------------------------------------------------------
void PresetPalette::palette_load(std::string p)
{
	ofLogNotice(__FUNCTION__) << p;

	std::string path = path_Palettes + p + ".json";
	ofFile file(path);
	if (file.exists())
	{
		jsonin ji(file);
		ji >> paletteData;

		ofLogNotice(__FUNCTION__) << "paletteData.name: " << paletteData.name;
		ofLogNotice(__FUNCTION__) << "paletteData.palette: " << ofToString(paletteData.palette);
		ofLogNotice(__FUNCTION__) << "paletteData.background: " << paletteData.background;

		(*name_BACK) = paletteData.name;
		(*palette_BACK) = paletteData.palette;
		backCol = paletteData.background;
		//+required pointer to be able to set background back!
	}
	else
	{
		ofLogNotice(__FUNCTION__) << "FILE '" << path << "' NOT FOUND";
	}
}

//--------------------------------------------------------------
void PresetPalette::palette_save(std::string p)
{
	ofLogNotice(__FUNCTION__) << p;
	std::string path = path_Palettes + p + ".json";

	paletteData.name = (*name_BACK);
	paletteData.palette = (*palette_BACK);
	paletteData.background = backCol;

	ofLogNotice(__FUNCTION__) << "paletteData.name: " << paletteData.name;
	ofLogNotice(__FUNCTION__) << "paletteData.palette: " << ofToString(paletteData.palette);
	ofLogNotice(__FUNCTION__) << "paletteData.background: " << paletteData.background;

	ofFile file(path, ofFile::WriteOnly);
	jsonout jo(file);
	jo << paletteData;
}

