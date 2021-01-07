#pragma once
#include "ofMain.h"

/*

TODO:
+ how make it functional ?
	+ to select a preset from the kit
	+ use pair or retur the struct data ?
*/

//----
#define BUTTON_BIG_HEIGHT 50
#define BUTTON_SLIM_HEIGHT 14

#include "ofxImGui.h"

#include "presets/PresetManager.h"
#include "presets/PresetPalette.h"

namespace ImGui_PalettesPicker
{
	//--------------------------------------------------------------
	inline int gui_Palettes(vector<PaletteData> &kit, int indexExt = -1)
	{
		static bool MODE_Slim = false;
		int indexPick = -1;

		float linew_Pick = 2.5;
		ImVec4 color_Pick{ 0,0,0,0.5 };
		//ImVec4 color_Pick{ 1,1,1,0.5 };

		//--

		ofxImGui::Settings mainSettings = ofxImGui::Settings();

		//ImGuiColorEditFlags _flagw;
		//_flagw = ImGuiTreeNodeFlags_None;
		//if (ImGui::CollapsingHeader("Kit", _flagw))
		{
			ImGuiColorEditFlags _flags;

			_flags =
				ImGuiColorEditFlags_NoAlpha |
				ImGuiColorEditFlags_NoPicker |
				ImGuiColorEditFlags_NoTooltip;

			//--

			//float _spc = ImGui::GetStyle().ItemInnerSpacing.x;
			//float _w = ImGui::GetWindowContentRegionWidth() - (1.0 * _spc);

			float _spc = 0;
			float _w = ImGui::GetWindowContentRegionWidth();
			float _hb = BUTTON_BIG_HEIGHT;
			float _hhB;
			float _spc2 = 6;//spacing between palettes 

			//--

			// color boxes

			for (int p = 0; p < kit.size(); p++)
			{
				//--

				////TODO: trying to make a frame border to all palette elements
				//ImGui::BeginGroup();
				//ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1,0,0,1));
				//ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2);

				//--

				// colors in each palette
				int _sizeP = kit[p].palette.size();

				ImGui::Dummy(ImVec2(0, _spc2));

				ImGui::Text(kit[p].name.c_str());

				//if (p == indexExt)
				//{
				//	ImGui::Separator();
				//}

				//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, 0);

				//--

				bool bDrawBorder = false;
				if (p == indexExt)//highlight selected
				{
					bDrawBorder = true;
					_hhB = 3 * BUTTON_SLIM_HEIGHT;
				}
				else 
				{
					_hhB = 1.0 * BUTTON_SLIM_HEIGHT;
				}
				if (bDrawBorder)
				{
					ImGui::PushStyleColor(ImGuiCol_Border, color_Pick);
					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, linew_Pick);
				}
				//_hhB = 0.7 * BUTTON_BIG_HEIGHT;//button height

				//----

				// each palette colors

				for (int c = 0; c < _sizeP; c++)
				{
					if (c != 0)
					{
						ImGui::SameLine(0, 0);
						//ImGui::SameLine();
						//ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);
					}

					ImGui::PushID(c);

					//-

					// same size for each color
					int _wwB = (_w / _sizeP) - _spc;

					std::string name = (kit[p].name + "_" + ofToString(p) + "_" + ofToString(c));

					//----

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
					
					//----

					ImGui::PopID();
				}

				//--

				// draw border to selected (colors ?)
				if (bDrawBorder)
				{
					ImGui::PopStyleColor();
					ImGui::PopStyleVar(1);
				}

				//ImGui::PopStyleVar(1);

				//--

				////TODO: trying to make a frame border to all palette elements
				//ImGui::PopStyleColor();
				//ImGui::PopStyleVar(1);
				//ImGui::EndGroup();
			}

			//ofxImGui::EndTree(mainSettings);
		}

		return indexPick;
	}
};

