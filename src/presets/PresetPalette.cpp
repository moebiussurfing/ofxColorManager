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
	color_BACK = _background;
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
bool PresetPalette::preset_load(std::string p)
{
	ofLogNotice(__FUNCTION__) << p;
	std::string path = path_Presets + p + ".json";

	ofFile file(path);
	bool b = file.exists();

	if (b)
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
		ofLogNotice(__FUNCTION__) << "File '" << path << "' NOT FOUND";
	}

	return b;
}

//--------------------------------------------------------------
void PresetPalette::preset_save(std::string p)
{
	ofLogNotice(__FUNCTION__) << p;
	std::string path = path_Presets + p + ".json";

	presetData.background = color_BACK;
	//presetData.background = (*background_BACK);

	presetData.name = (*name_BACK);
	presetData.curveName = (*curveName_BACK);
	presetData.palette = (*palette_BACK);

	ofLogNotice(__FUNCTION__) << "name       : " << presetData.name;
	ofLogNotice(__FUNCTION__) << "curveName  : " << presetData.curveName;
	ofLogNotice(__FUNCTION__) << "palette    : " << ofToString(presetData.palette);
	ofLogNotice(__FUNCTION__) << "background : " << presetData.background;

	//curvesTool.save(path_Presets+presetData.curveName+".yml");

	ofFile file(path, ofFile::WriteOnly);
	jsonout jo(file);
	jo << presetData;

	ofLogNotice(__FUNCTION__) << "DONE! preset_save: " << p;
}

//--------------------------------------------------------------
PaletteData PresetPalette::preset_LoadPalette(std::string name)//TODO: not elegant.. bc uses the target object..
{
	ofLogNotice(__FUNCTION__) << name;

	std::string path = path_Presets + name + ".json";
	ofFile file(path);
	PaletteData pd;

	if (file.exists())
	{
		jsonin ji(file);
		ji >> pd;

		ofLogNotice(__FUNCTION__) << "name       : " << pd.name;
		ofLogNotice(__FUNCTION__) << "palette    : " << ofToString(pd.palette);
		ofLogNotice(__FUNCTION__) << "background : " << pd.background;
	}
	else
	{
		ofLogNotice(__FUNCTION__) << "File '" << path << "' NOT FOUND";
	}

	return pd;
}

//--------------------------------------------------------------
void PresetPalette::palette_load(std::string name)
{
	ofLogNotice(__FUNCTION__) << name;

	std::string path = path_Palettes + name + ".json";
	ofFile file(path);

	if (file.exists())
	{
		jsonin ji(file);
		ji >> paletteData;

		ofLogNotice(__FUNCTION__) << "name       : " << paletteData.name;
		ofLogNotice(__FUNCTION__) << "palette    : " << ofToString(paletteData.palette);
		ofLogNotice(__FUNCTION__) << "background : " << paletteData.background;

		(*name_BACK) = paletteData.name;
		(*palette_BACK) = paletteData.palette;
		color_BACK = paletteData.background;
		//+required pointer to be able to set background back!
	}
	else
	{
		ofLogNotice(__FUNCTION__) << "File '" << path << "' NOT FOUND";
	}
}

//--------------------------------------------------------------
void PresetPalette::palette_save(std::string p)
{
	ofLogNotice(__FUNCTION__) << p;
	std::string path = path_Palettes + p + ".json";

	paletteData.name = (*name_BACK);
	paletteData.palette = (*palette_BACK);
	paletteData.background = color_BACK;

	ofLogNotice(__FUNCTION__) << "name       : " << paletteData.name;
	ofLogNotice(__FUNCTION__) << "palette    : " << ofToString(paletteData.palette);
	ofLogNotice(__FUNCTION__) << "background : " << paletteData.background;

	ofFile file(path, ofFile::WriteOnly);
	jsonout jo(file);
	jo << paletteData;
}

