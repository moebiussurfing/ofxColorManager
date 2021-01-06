#pragma once
#include "ofMain.h"

/*

TODO:
+ how make it functional?
	+ to select a preset from the kit

*/


#include "ofxImGui.h"
#define BUTTON_BIG_HEIGHT 50
#define BUTTON_SLIM_HEIGHT 14

#include "presets/PresetManager.h"
#include "presets/PresetPalette.h"

//class ImGui_PalettesPicker
namespace ImGui_PalettesPicker
{
	//--------------------------------------------------------------
	inline int gui_Palettes(vector<PaletteData> kit)
	{
		static bool MODE_Slim = false;
		int indexPick = -1;

		ofxImGui::Settings mainSettings = ofxImGui::Settings();

		ImGuiColorEditFlags _flagw;
		_flagw = ImGuiTreeNodeFlags_None;

		if (ImGui::CollapsingHeader("Kit", _flagw))
		{
			ImGuiColorEditFlags _flags;

			_flags =
				ImGuiColorEditFlags_NoAlpha |
				ImGuiColorEditFlags_NoPicker |
				ImGuiColorEditFlags_NoTooltip;

			//--

			float _spc = ImGui::GetStyle().ItemInnerSpacing.x;
			float _w = ImGui::GetWindowContentRegionWidth() - (3 * _spc);
			float _hb = BUTTON_BIG_HEIGHT;

			int _hhB;
			//_hhB = 0.7 * BUTTON_BIG_HEIGHT;//button height
			_hhB = BUTTON_SLIM_HEIGHT;

			//--

			// color boxes

			for (int p = 0; p < kit.size(); p++)
			{
				// colors in each palette
				int _sizeP = kit[p].palette.size();

				ImGui::Dummy(ImVec2(0, 10));
				ImGui::Text(kit[p].name.c_str());

				//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, 0);

				for (int c = 0; c < _sizeP; c++)
				{
					if (c != 0) ImGui::SameLine();

					ImGui::PushID(c);

					// same size for each color
					int _wwB = _w / _sizeP - _spc;

					std::string name = (kit[p].name + "_" + ofToString(p) + "_" + ofToString(c));

					//-

					// colored box

					if (ImGui::ColorButton(
						name.c_str(),
						kit[p].palette[c],
						_flags,
						ImVec2(_wwB, _hhB)))
					{
						ofLogNotice(__FUNCTION__) << p << "," << c;

						indexPick = p;
					}

					ImGui::PopID();
				}

				//ImGui::PopStyleVar(1);
			}

			//ofxImGui::EndTree(mainSettings);
		}

		return indexPick;
	}


};

