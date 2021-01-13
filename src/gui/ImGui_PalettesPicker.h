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
	inline int gui_GridPalettes(vector<PaletteData> &kit, int indexExt = -1, bool autoscroll = true)
	{
		static bool MODE_Slim = false;
		int indexPick = -1;

		//border style
		float linew_Pick = 2.5;
		ImVec4 color_Pick{ 0,0,0,0.5 };
		auto color_Pick32 = IM_COL32(color_Pick.x*255.f, color_Pick.y*255.f, color_Pick.z*255.f, color_Pick.w*255.f);

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

				//group border
				auto pos1 = ImGui::GetCursorScreenPos();

				//-

				//ImGui::SetScrollFromPosY(ImGui::GetCursorStartPos().y + 200, 0.5f);
				//ImGui::SetScrollHereY(0.5f); // 0.0f:top, 0.5f:center, 1.0f:bottom

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

					if (autoscroll)
						ImGui::SetScrollHereY(0.5f); // 0.0f:top, 0.5f:center, 1.0f:bottom
				}
				else
				{
					_hhB = 1.0 * BUTTON_SLIM_HEIGHT;
				}

				if (bDrawBorder)
				{
					ImGui::PushStyleColor(ImGuiCol_Border, color_Pick);
					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, linew_Pick);

					//-
					
					//group border
					ImGui::BeginGroup();
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

					//ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 255, 0, 255));

					//group border
					ImGui::EndGroup();
					auto pos2 = ImGui::GetCursorScreenPos();
					float pad = 2.0f;
					ImGui::GetWindowDrawList()->AddRect(ImVec2(pos1.x - pad, pos1.y), ImVec2(pos1.x + _w + pad, pos2.y + pad), color_Pick32);
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

