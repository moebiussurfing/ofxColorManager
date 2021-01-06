#pragma once
#include "ofMain.h"

#include "ofxImGui.h"
#define BUTTON_BIG_HEIGHT 50
#define BUTTON_SLIM_HEIGHT 14

#include "presets/PresetManager.h"
#include "presets/PresetPalette.h"

//class ImGui_PalettesPicker
namespace ImGui_PalettesPicker
{
	//--------------------------------------------------------------
	inline void gui_Palettes(vector<PaletteData> kit)
	{
		static bool MODE_Slim = false;

		ofxImGui::Settings mainSettings = ofxImGui::Settings();

		if (ofxImGui::BeginTree("KIT", mainSettings))
		{
			ImGuiColorEditFlags _flags;

			_flags =
				ImGuiColorEditFlags_NoAlpha |
				ImGuiColorEditFlags_NoPicker |
				ImGuiColorEditFlags_NoTooltip;

			//--

			float _spc = ImGui::GetStyle().ItemInnerSpacing.x;
			float _w = ImGui::GetWindowContentRegionWidth() - 2 * _spc - 10;
			float _hb = BUTTON_BIG_HEIGHT;
			int _hhB = 0.7 * BUTTON_BIG_HEIGHT;//button height

			if (ImGui::Checkbox("Slim", &MODE_Slim))
			{
				_hhB = BUTTON_SLIM_HEIGHT;
			}

			//--

			// color boxes

			for (int p = 0; p < kit.size(); p++)
			{

				// colors in each palette
				int _sizeP = kit[p].palette.size();

				for (int c = 0; c < _sizeP; c++)
				{
					if (c != 0) ImGui::SameLine();

					ImGui::PushID(c);

					// same size for each color
					int _wwB = _w / _sizeP - _spc;

					std::string name = ("Kit_" + ofToString(p) + "_" + ofToString(c));

					//-

					// colored box

					if (ImGui::ColorButton(
						name.c_str(),
						kit[p].palette[c],
						_flags,
						ImVec2(_wwB, _hhB)))
					{
						ofLogNotice(__FUNCTION__) << p << "," << c;
					}

					ImGui::PopID();
				}
			}

			ofxImGui::EndTree(mainSettings);
		}
	}


};

