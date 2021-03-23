#include "ofxColourLoversHelper.h"

//--------------------------------------------------------------
ofxColourLoversHelper::ofxColourLoversHelper()
{
	//ofSetLogLevel(OF_LOG_VERBOSE);
	//ofSetLogLevel("ofxColourLovers", OF_LOG_VERBOSE);

	//--

	path_Global = "ofxColourLoversHelper/";
	ofxSurfingHelpers::CheckFolder(path_Global);

	path_AppSettings = path_Global + "ofxColourLoversHelper_Settings.xml";

	//--

	// default settings
	position = glm::vec2(1000, 10);
	size = glm::vec2(200, 400);
	gridPosition = glm::vec2(1210, 10);
	gridSize = glm::vec2(200, ofGetHeight());

	//-

	//// set positions and panel sizes
	//glm::vec2 sizeGui(150, 400);
	//glm::vec2 sizeGrid(150, ofGetHeight());
	//glm::vec2 posGui(ofGetWidth() - (sizeGui.x + sizeGrid.x + 4), 0);
	//glm::vec2 posGrid(posGui.x + sizeGui.x + 2, 0);

	////must be called before setup() to overwrite default settings
	//setGrid(posGrid, sizeGrid);
	//setup(posGui, sizeGui);

	//-

	colorMarked = (ofColor::white);

	//setVisible(true);
	addMouseListeners();

	//setEnableKeys(true);
	addKeysListeners();
	isKeysEnabled = true;
}

//--------------------------------------------------------------
void ofxColourLoversHelper::setToggleVisible()
{
	bIsVisible = !bIsVisible;
}

//--------------------------------------------------------------
void ofxColourLoversHelper::setVisible(bool b)
{
	bIsVisible = b;

	//-

#ifdef USE_OFX_UI
	if (gui_Lab) gui_Lab->setVisible(bIsVisible);
	//gui->setVisible(bIsVisible);
	if (bIsVisible && gui) setVisibleSearcher(bSearcherVisible);
#endif

	//-

	if (bIsVisible)
	{
		addMouseListeners();

		addKeysListeners();
		isKeysEnabled = true;
		//setEnableKeys(true);
	}
	else
	{
		removeMouseListeners();

		removeKeysListeners();
		isKeysEnabled = false;
		//setEnableKeys(false);
	}
}

#ifdef USE_OFX_IM_GUI

//--------------------------------------------------------------
void ofxColourLoversHelper::drawImGuiMain()
{
	float ww, hh;
	ww = PANEL_WIDGETS_WIDTH;
	hh = PANEL_WIDGETS_HEIGHT;

	ImGuiWindowFlags flags = auto_resize1 ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;
	if (!auto_resize1) ImGui::SetWindowSize(ImVec2(ww, hh));

	mainSettings = ofxImGui::Settings();

	//--

#ifndef USE_OFX_IM_GUI_EXTERNAL
	gui_ImGui.begin();
#endif

	//--

	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(PANEL_WIDGETS_WIDTH, PANEL_WIDGETS_HEIGHT));

	//----

	//static bool open_popup = false;
	//open_popup = MODE_Search;

	if (MODE_Search)
	{
		static bool auto_resize2 = true;
		ImGuiWindowFlags flagsw2;
		flagsw2 = auto_resize2 ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;

		//flagsw2 |= flagsWindows;
		//flagsw2 |= ImGuiWindowFlags_NoTitleBar;
		//flagsw2 |= ImGuiWindowFlags_NoBackground;
		//flagsw2 |= ImGuiWindowFlags_NoDecoration;

		if (ofxImGui::BeginWindow("COLOUR-LOVERS QUERY", mainSettings, flagsw2))
		{
			float _h = BUTTON_BIG_HEIGHT;
			float _hb = _h * 0.5;
			float _w100 = ImGui::GetContentRegionAvail().x;

			ImGui::PushItemWidth(-1);

			// search
			ImGui::Text("Type Keyword:");

			char tab1[32];
			strncpy(tab1, textInput_temp1.c_str(), sizeof(tab1));
			tab1[sizeof(tab1) - 1] = 0;

			if (ImGui::InputText("", tab1, IM_ARRAYSIZE(tab1), ImGuiInputTextFlags_EnterReturnsTrue))
			{
				textInput_temp1 = ofToString(tab1);
				ofLogNotice(__FUNCTION__) << "input: " << textInput_temp1;

				if (textInput_temp1 != textInput_temp1_PRE)
				{
					textInput_temp1_PRE = textInput_temp1;
				}
			}

			//--

			//TODO:
			//to disable all other key commands
			bool b = bTextInputActive;
			bTextInputActive = ImGui::IsItemActive();
			//changed
			if (bTextInputActive != b) ofLogNotice(__FUNCTION__) << "TextInput : " << (bTextInputActive ? "ACTIVE" : "DISABLED");

			//--

			//blink when a new preset is editing
			float freq = 0.15;//speed freq
			float min = 0.20;
			float max = 0.80;
			float a = ofMap(glm::sin(freq * ofGetFrameNum()), -1, 1, min, max);
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.5f, 0.0f, 0.0f, a));

			if (ImGui::Button("SEARCH", ImVec2(_w100, _hb * 2)))
			{
				ofLogNotice(__FUNCTION__) << "searchPalettes: " << textInput_temp1_PRE;

				ofxColourLovers::searchPalettes(textInput_temp1_PRE, amountResults);

				lastSearch = textInput_temp1_PRE;

				MODE_Search = false;//close
			}
			ImGui::PopItemWidth();
			ImGui::PopStyleColor();

			ImGui::PushItemWidth(-65);
			ImGui::SliderInt("Amnt Max", &amountResults, 10, 200);
			ImGui::PopItemWidth();

			//----

			//hide to debug and simplify
			//#define EXTEND_SEARCH
#ifdef EXTEND_SEARCH
			//lover
			ImGui::Text("Lover Id:");
			std::string textInput_temp2 = "";

			char tab2[32];
			strncpy(tab2, textInput_temp2.c_str(), sizeof(tab2));
			tab2[sizeof(tab2) - 1] = 0;

			if (ImGui::InputText("", tab2, IM_ARRAYSIZE(tab2)))
			{
				ofLogNotice(__FUNCTION__) << "InputText:" << ofToString(tab2);
				textInput_temp2 = ofToString(tab2);
				ofLogNotice(__FUNCTION__) << "textInput_temp2:" << textInput_temp2;

				lastSearch = textInput_temp2;
				std::string s = textInput_temp2;
				ofStringReplace(s, " ", "%20");
				ofxColourLovers::getTopPalettesForLover(s, amountResults);
			}

			//-

			// palette
			ImGui::Text("Palette Id:");
			std::string textInput_temp3 = "";

			char tab3[32];
			strncpy(tab3, textInput_temp3.c_str(), sizeof(tab3));
			tab3[sizeof(tab3) - 1] = 0;

			if (ImGui::InputText("", tab3, IM_ARRAYSIZE(tab3)))
			{
				ofLogNotice(__FUNCTION__) << "InputText:" << ofToString(tab3);
				textInput_temp3 = ofToString(tab3);
				ofLogNotice(__FUNCTION__) << "textInput_temp2:" << textInput_temp3;

				lastSearch = textInput_temp3;
				ofxColourLovers::getPalette(lastSearch);
			}
#endif
			//ImGui::Dummy(ImVec2(0, 2));
			//ImGui::PushItemWidth(-40);
			//ImGui::SliderInt("Amnt Max", &amountResults, 25, 100);
			//ImGui::PopItemWidth();

		}
		ofxImGui::EndWindow(mainSettings);
	}

	//----

	if (ofxImGui::BeginWindow("COLOUR-LOVERS", mainSettings, flags))
	{
		float _h = BUTTON_BIG_HEIGHT;
		float _hb = _h * 0.5;
		float _spc = ImGui::GetStyle().ItemSpacing.x;
		float _w100 = ImGui::GetContentRegionAvail().x;
		float _w99 = _w100 - _spc;
		float _w50 = _w99 / 2;// -_spc;

		//-

		ofxSurfingHelpers::AddBigToggle(MODE_Search, _w100, _h);

		//-

#ifndef USE_FLOATING_SEARCH_WINDOW_WORAROUND
		if (ImGui::CollapsingHeader("HTTP SEARCH"))
		{
			//open_popup = true;

			ImGui::Dummy(ImVec2(0, 5));

			//----

			// search
			ImGui::Text("Type Keyword:");
			//std::string textInput_temp1 = "";

			if (ImGui::InputText("", tab1, IM_ARRAYSIZE(tab1)))
			{
				textInput_temp1 = ofToString(tab1);
				ofLogNotice(__FUNCTION__) << "input: " << textInput_temp1;

				//ofLogNotice(__FUNCTION__) << "InputText:" << ofToString(tab1);
				//lastSearch_PRE = lastSearch;

				if (textInput_temp1 != textInput_temp1_PRE)
				{
					textInput_temp1_PRE = textInput_temp1;
				}
			}

			//ImGui::SameLine();
			if (ImGui::Button("SEARCH", ImVec2(_w100, _hb * 2)))
			{
				ofLogNotice(__FUNCTION__) << "searchPalettes: " << textInput_temp1_PRE;

				ofxColourLovers::searchPalettes(textInput_temp1_PRE, amountResults);

				lastSearch = textInput_temp1_PRE;

				//textInput_temp1_PRE = textInput_temp1 = "";//clear
			}

			//-

			//hide to debug and simplify
			//#define EXTEND_SEARCH
#ifdef EXTEND_SEARCH
			//lover
			ImGui::Text("Lover Id:");
			std::string textInput_temp2 = "";

			char tab2[32];
			strncpy(tab2, textInput_temp2.c_str(), sizeof(tab2));
			tab2[sizeof(tab2) - 1] = 0;

			if (ImGui::InputText("", tab2, IM_ARRAYSIZE(tab2)))
			{
				ofLogNotice(__FUNCTION__) << "InputText:" << ofToString(tab2);
				textInput_temp2 = ofToString(tab2);
				ofLogNotice(__FUNCTION__) << "textInput_temp2:" << textInput_temp2;

				lastSearch = textInput_temp2;
				std::string s = textInput_temp2;
				ofStringReplace(s, " ", "%20");
				ofxColourLovers::getTopPalettesForLover(s, amountResults);
			}

			//-

			// palette
			ImGui::Text("Palette Id:");
			std::string textInput_temp3 = "";

			char tab3[32];
			strncpy(tab3, textInput_temp3.c_str(), sizeof(tab3));
			tab3[sizeof(tab3) - 1] = 0;

			if (ImGui::InputText("", tab3, IM_ARRAYSIZE(tab3)))
			{
				ofLogNotice(__FUNCTION__) << "InputText:" << ofToString(tab3);
				textInput_temp3 = ofToString(tab3);
				ofLogNotice(__FUNCTION__) << "textInput_temp2:" << textInput_temp3;

				lastSearch = textInput_temp3;
				ofxColourLovers::getPalette(lastSearch);
	}
#endif
			//ImGui::Dummy(ImVec2(0, 2));

			ImGui::PushItemWidth(-40);
			ImGui::SliderInt("Amnt Max", &amountResults, 25, 100);
			ImGui::PopItemWidth();
}
		else open_popup = false;
#endif

		//----


		if (ImGui::CollapsingHeader("BROWSE", ImGuiWindowFlags_NoCollapse))
		{
			_w100 = ImGui::GetContentRegionAvail().x;
			_w99 = _w100 - _spc;
			_w50 = _w99 / 2;

			// mode selectors

			ofxSurfingHelpers::AddBigToggle(bFavorites, _w50, _hb * 2);
			ImGui::SameLine();
			ofxSurfingHelpers::AddBigToggle(bHistory, _w50, _hb * 2);

			//--

			if (ImGui::CollapsingHeader("ADVANCED", ImGuiWindowFlags_None))
			{
				_w100 = ImGui::GetContentRegionAvail().x;
				_w99 = _w100 - _spc;
				_w50 = _w99 / 2;

				{
					if (ImGui::Button("ADD FAVORITE", ImVec2(_w100, _hb)))
					{
						ofxSurfingHelpers::CheckFolder(path_Global + "favorites/");

						std::string str = path_Global + "favorites/" + palettes[currPalette].id + ".xml";

						palettes[currPalette].save(str);
						ofLogNotice(__FUNCTION__) << "saved favorite: " << str;
					}

					//--

					if (ImGui::Button("CLEAR FAVORITES", ImVec2(_w50, _hb))) ImGui::OpenPopup("CLEAR FAVORITES?");

					if (ImGui::BeginPopupModal("CLEAR FAVORITES?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
					{
						ImGui::Text("All Favorite palettes will be deleted.\nThis operation cannot be undone!\n\n");
						ImGui::Separator();

						static bool dont_ask_me_next_timeFavs = false;
						ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
						ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_timeFavs);
						ImGui::PopStyleVar();

						if (ImGui::Button("OK", ImVec2(_w99, 0))) {
							clearFavourites();
							//workflow
							loadFavorites();

							ImGui::CloseCurrentPopup();
						}
						ImGui::SetItemDefaultFocus();
						ImGui::SameLine();
						if (ImGui::Button("Cancel", ImVec2(_w99, 0))) { ImGui::CloseCurrentPopup(); }
						ImGui::EndPopup();
					}
				}
				//ImGui::PopItemWidth();
				ImGui::SameLine();
				{
					if (ImGui::Button("CLEAR HISTORY", ImVec2(_w50, _hb))) ImGui::OpenPopup("CLEAR HISTORY?");

					if (ImGui::BeginPopupModal("CLEAR HISTORY?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
					{
						ImGui::Text("All History will be deleted.\nThis operation cannot be undone!\n\n");
						ImGui::Separator();

						static bool dont_ask_me_next_time = false;
						ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
						ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
						ImGui::PopStyleVar();

						if (!dont_ask_me_next_time) {
							if (ImGui::Button("OK", ImVec2(_w99, 0))) {
								clearHistory();
								//workflow
								loadHistory();

								ImGui::CloseCurrentPopup();
							}
							ImGui::SetItemDefaultFocus();
							ImGui::SameLine();
							if (ImGui::Button("Cancel", ImVec2(_w99, 0))) { ImGui::CloseCurrentPopup(); }
						}
						else {
							clearHistory();
							//workflow
							loadHistory();

							//ImGui::CloseCurrentPopup();
						}
						ImGui::EndPopup();
					}
				}

				//--

				ImGui::Dummy(ImVec2(0, 5));

				ofxSurfingHelpers::AddBigToggle(SHOW_BrowserPalettes, _w100, _hb);

				if (SHOW_AdvancedLayout)
				{
					ImGui::SameLine();
					ofxImGui::AddParameter(SHOW_AdvancedLayout);
				}
			}

			//----

			// scrollable list

			ImGui::Dummy(ImVec2(0, 5));

			if (palettes.size() != 0)
			{
				int _i = currPalette;

				ImGui::PushItemWidth(_w99 - 10);

				if (ofxImGui::VectorCombo(" ", &_i, pNames))
				{
					ofLogNotice(__FUNCTION__) << "_i: " << ofToString(_i);

					if (_i < pNames.size())
					{
						currPalette = _i;
						pName = pNames[_i];
						ofLogNotice(__FUNCTION__) << "Combo select: " << _i;

						if (pNames.size() > 0 && currPalette < pNames.size())
						{
							refreshPalette();

							if (MODE_PickColor_BACK)
							{
								// set BACK color clicked
								if (myColor_BACK != nullptr)
								{
									myColor_BACK->set(palettes[currPalette].colours[0]);//auto get first color from palette beacuse there's no color click! just key pressed

									// flag updater color ready
									if (bUpdated_Color_BACK != nullptr && MODE_PickColor_BACK)
									{
										(*bUpdated_Color_BACK) = true;
									}
								}
							}

							if (!AutoScroll) AutoScroll = true;
						}
					}
				}

				ImGui::PopItemWidth();
			}
		}

		//-

		ImGui::Dummy(ImVec2(0, 2));

		//ImGui::Text(lastSearch.c_str());

		//// history or favorites
		//std::string ss;
		//ss = lastSearch;
		////ss = lastSearch + "      " + s;
		//ImGui::Text(ss.c_str());
		////ImGui::Text(s.c_str());

		//// name
		//ImGui::Text(lastPaletteName.get().c_str());

		//-

		// index/max
		std::string s;
		if (_files.numFiles() > 0)
		{
			s = ofToString(currPalette) + "/" + ofToString(palettes.size() - 1);
		}
		else
		{
			s = ofToString("-1/0");
		}
		ImGui::Text(s.c_str());

		//-

		ImGui::PushButtonRepeat(true);
		{
			if (ImGui::Button("Previous", ImVec2(_w50, _hb * 2)))
			{
				prevPalette();

				//-

				// worfklow
				//if (!AutoScroll) AutoScroll = true;
			}

			ImGui::SameLine();

			if (ImGui::Button("Next", ImVec2(_w50, _hb * 2)))
			{
				nextPalette();

				//-

				// worfklow
				//if (!AutoScroll) AutoScroll = true;
			}
		}
		ImGui::PopButtonRepeat();

		//-

		// palette colors

		ImGuiColorEditFlags colorEdiFlags =
			ImGuiColorEditFlags_NoAlpha |
			ImGuiColorEditFlags_NoPicker |
			ImGuiColorEditFlags_NoTooltip;

		//macOS
		if (palettes.size() > 0) {
			vector<ofColor> p = palettes[currPalette].colours;
			//        const auto p = palettes[currPalette].colours;
			//         auto p = palettes[currPalette].colours;

					//myPalette_BACK

					// fit width
			float wb = (_w100 / (int)p.size());// -_spc;
			float hb = BUTTON_BIG_HEIGHT;

			for (int n = 0; n < p.size(); n++)
			{
				ImGui::PushID(n);

				// fit width
				if (n != 0) ImGui::SameLine(0, 0);

				// box colors
				if (ImGui::ColorButton("##paletteLover", p[n], colorEdiFlags, ImVec2(wb, hb))) {}

				ImGui::PopID();
			}
		}
		//-

		if (ImGui::Button("Randomize", ImVec2(_w100, 2 * _hb)))
		{
			randomPalette();

			//-

			// worfklow
			//if (!AutoScroll) AutoScroll = true;
		}

		//-

		if (SHOW_AdvancedLayout)
		{
			if (ImGui::CollapsingHeader("Advanced"))
			{
				_w100 = ImGui::GetContentRegionAvail().x;
				_w99 = _w100 - _spc;
				_w50 = _w100 / 2;// -_spc;

				ofxImGui::AddParameter(MODE_PickPalette_BACK);
				ofxImGui::AddParameter(MODE_PickColor_BACK);
				ofxImGui::AddParameter(ENABLER_Keys);

				if (ImGui::CollapsingHeader("Layout"))
				{
					//if (ImGui::Checkbox("Fixed Width", &MODE_FixedSize))
					bool bPre = MODE_FixedSize;
					if (ofxImGui::AddParameter(MODE_FixedSize))
					{
						if (MODE_FixedSize != bPre)
						{
							build_Gui_Lab();
						}
					}
					if (ofxImGui::AddParameter(MODE_Slim))
					{
					}

					// layout
					ImGui::Checkbox("Focus", &bfocus);
					ImGui::Checkbox("Auto-resize Main", &auto_resize1);
					ImGui::Checkbox("Auto-resize Kit", &auto_resize2);
				}
			}
		}

		if (SHOW_BrowserPalettes) ofxImGui::AddParameter(AutoScroll);
	}
	ofxImGui::EndWindow(mainSettings);

	ImGui::PopStyleVar();

	//----

	// 2. kit palettes browser
	drawImGuiBrowseKits();

	//---

#ifndef USE_OFX_IM_GUI_EXTERNAL
	gui_ImGui.end();
#endif
}

//--------------------------------------------------------------
void ofxColourLoversHelper::drawImGuiBrowseKits()
{
	//auto_resize = false;

	// palette rows with all color boxes
	if (SHOW_BrowserPalettes)
	{
		ImGuiWindowFlags flags = auto_resize2 ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(PANEL_WIDGETS_WIDTH, PANEL_WIDGETS_HEIGHT));

		if (ofxImGui::BeginWindow("LOVER PALETTES", mainSettings, flags))
		{
			float _spc2 = 2;//spacing between palettes 

			////label: favorites or history
			//std::string ss = lastSearch;
			//ImGui::Text(ss.c_str());

			//--

			//TODO:
			// tween sizes
			bool btween = false;
			static float alpha;
			static int indexExt_PRE;
			const float step = 0.1f;
			if (btween)
			{
				if (currPalette != indexExt_PRE)
				{
					indexExt_PRE = currPalette;
					alpha = 1;
				}
				else
				{
					if (alpha > 0) alpha -= step;
					alpha = ofClamp(alpha, 0, 1);
				}
				//ofLogNotice(__FUNCTION__) << "alpha:  " << alpha;
			}

			//--

			float _spc = ImGui::GetStyle().ItemSpacing.x;
			float _w = ImGui::GetContentRegionAvail().x;
			float _hhB;//applied
			float _hb;

			// border style

			// blink when selected
			float freq = 0.15;//speed freq
			float min = 0.20;
			float max = 0.80;
			float a = ofMap(glm::sin(freq * ofGetFrameNum()), -1, 1, min, max);

			float borderLineWidth = 2.0;
			ImVec4 borderLineColor{ 0, 0, 0, a };
			auto color_Pick32 = IM_COL32(borderLineColor.x*255.f, borderLineColor.y*255.f, borderLineColor.z*255.f, borderLineColor.w*255.f);

			//-

			ImGuiColorEditFlags _flags =
				ImGuiColorEditFlags_NoAlpha |
				ImGuiColorEditFlags_NoPicker |
				ImGuiColorEditFlags_NoTooltip;

			//--

			for (int i = 0; i < palettes.size(); i++)
			{
				// group border
				//auto pos1 = ImGui::GetCursorScreenPos();

				//-

				// 1. spacing
				ImGui::Dummy(ImVec2(0, _spc2));

				// autoscroll
				if (i == currPalette) if (AutoScroll) ImGui::SetScrollHereY(0.0f); // 0.0f:top, 0.5f:center, 1.0f:bottom

				// 2. label tittle
				std::string _name = palettes[i].title;

				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 2));
				ImGui::Text(_name.c_str());

				ImGui::PopStyleVar();

				//-

				// set color heights
				if (MODE_Slim.get()) _hb = BUTTON_SLIM_HEIGHT;
				else _hb = BUTTON_BIG_HEIGHT2;

				////TODO:
				////check offset..
				//float _offset = 0;
				//for (int c = 0; c < _sizeP; c++) {
				//	_offset += palettes[i].colorWidths[c];
				//}

				// 3. colors in each palette

				int _sizeP = palettes[i].colours.size();

				for (int c = 0; c < _sizeP; c++)
				{
					if (c != 0) ImGui::SameLine(0, 0);

					//-

					ImGui::PushID(c);

					float _wwB;
					if (MODE_FixedSize)
					{
						// same size for each color
						_wwB = (_w / _sizeP) - _spc;
					}
					else
					{
						// different sizes with original colourLover Palettes
						_wwB = (palettes[i].colorWidths[c] * _w);
						//_wwB = (palettes[i].colorWidths[c] * _w) - _spc;
					}

					//-

					//////TODO:
					//////check offset..
					//if (_offset != 1.0f) {
					//	if (c == _sizeP - 1) {//add the offset to the last color box
					//		if (_offset != 1.0f) {
					//			_wwB = _wwB + _offset;
					//		}
					//	}
					//}
					////// adjust better to fit to border..
					////if (_sizeP % 2 == 0)// || _sizeP % 3 == 0)
					////{
					////	_wwB = _wwB + (_spc / _sizeP);
					////}

					//-

					std::string name = ("CL_" + ofToString(i) + "_" + ofToString(c));

					//-

					// draw border to selected color/palette
					bool bDrawBorder = false;
					float _scale = 1.0f;

					if (i == indexExt_PRE)
					{
						if (btween) {
							if (bfocus) _scale = ofMap(alpha, 1, 0, 1.75f, 1.0f, true);
						}
						else {
							if (bfocus) _scale = 1.0f;
						}

						_hhB = _hb * _scale;
					}

					else if (i == currPalette)// highlight selected
					{
						bDrawBorder = true;

						//if (!MODE_Slim) 
						//{
						//	_scale = 0.45f;
						//}
						//else {
						//	_scale = 1.0f;
						//}

						if (btween) {
							if (bfocus) _scale = ofMap(alpha, 1, 0, 1.75f, 1.0f, true);
						}
						else {
							if (bfocus) _scale = 1.75f;
						}

						_hhB = _hb * _scale;

						//_hhB = _hb;

						//-

						ImGui::PushStyleColor(ImGuiCol_Border, borderLineColor);
						ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, borderLineWidth);
						//ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, 3.0);

						//-

						////group border
						//ImGui::BeginGroup();
					}
					else
					{
						_hhB = _hb * _scale;
					}

					//------------

					// colored box

					//ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, 1.0);

					if (ImGui::ColorButton(name.c_str(), palettes[i].colours[c], _flags, ImVec2(_wwB, _hhB)))
					{
						//-

						// 1. filter name

						std::string whatList = name.substr(0, 2);
						std::string whatColId = name.substr(3, name.length() - 3);

						vector<std::string> seg = ofSplitString(name, "_");
						int pId = ofToInt(seg[1]);
						int cId = ofToInt(seg[2]);
						ColourLovePalette p = palettes[pId];

						lastPaletteName = p.title;

#ifdef USE_OFX_UI
						lastPaletteName_UI->setLabel(lastPaletteName);
#endif
						//-

						// 2. pick palette and color

						lastColor_clicked = p.colours[cId];
						//lastColor_clicked = palettes[i].colours[c];

						// set BACK color clicked
						if (myColor_BACK != nullptr && MODE_PickColor_BACK)
						{
							myColor_BACK->set(lastColor_clicked);
						}

						// flag updater color ready
						if (bUpdated_Color_BACK != nullptr && MODE_PickColor_BACK)
						{
							(*bUpdated_Color_BACK) = true;
						}

						//-

						// 3. set palette

						//currPalette = i;
						currPalette = pId;
						setPalette(currPalette);

						ofLogNotice(__FUNCTION__) << "palette Id : " << pId;
						ofLogNotice(__FUNCTION__) << "currPalette: " << currPalette;

						refreshPalette();

						//-

						// worfklow
						if (AutoScroll) AutoScroll = false;
					}

					//ImGui::PopStyleVar(1);

					//-

					if (bDrawBorder)
					{
						ImGui::PopStyleColor();
						ImGui::PopStyleVar(1);

						////group border
						//ImGui::EndGroup();
						//auto pos2 = ImGui::GetCursorScreenPos();
						//float pad = 2.0f;
						//ImGui::GetWindowDrawList()->AddRect(ImVec2(pos1.x - pad, pos1.y), ImVec2(pos1.x + _w + pad, pos2.y + pad), color_Pick32);
					}

					//-

					ImGui::PopID();
				}
			}

			//--

			//TODO:
			//disable scroll
			auto &io = ImGui::GetIO();
			if (io.MouseHoveredViewport)
			{
				float wheel = io.MouseWheel;
				if (wheel != 0)
				{
					if (AutoScroll) AutoScroll = false;
				}
			}
		}
		ofxImGui::EndWindow(mainSettings);

		ImGui::PopStyleVar();
	}
}
#endif

//--------------------------------------------------------------
void ofxColourLoversHelper::setup()
{
	//--

#ifndef USE_OFX_IM_GUI_EXTERNAL
#ifdef USE_OFX_IM_GUI
	ofxSurfingHelpers::ImGui_FontCustom();

	gui_ImGui.setup();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::GetIO().MouseDrawCursor = false;
	ImGui::GetIO().ConfigWindowsResizeFromEdges = true;

	////daan
	//gui_ImGui.setup(nullptr, ImGuiConfigFlags_DockingEnable, true, true);

	ofxSurfingHelpers::ImGui_ThemeMoebiusSurfing();
#endif
#endif

	//--

#ifdef USE_OFX_UI

	if (gui_Lab)
	{
		ofRemoveListener(gui_Lab->newGUIEvent, this, &ofxColourLoversHelper::Changed_Gui_Lab);
		delete gui_Lab;
		gui_Lab = 0;
	}
	if (gui)
	{
		ofRemoveListener(gui->newGUIEvent, this, &ofxColourLoversHelper::Changed_Gui);

		ofRemoveListener(ColourLoveEvent::events, this, &ofxColourLoversHelper::Changed_ColourLovers);
		delete gui;
		gui = 0;
	}

	//-

	int width = size.x;
	int xInit = 6;
	int dim = 30;

	gui = new ofxUICanvas(position.x, position.y, size.x, size.y);

	// custom bacground color
	ofFloatColor colorBg;
	//colorBg.set(0.06f, 0.06f, 0.06f, 0.94f);
	colorBg.set(0.06f, 0.06f, 0.06f, 0.8f);
	gui->setColorBack(ofColor(colorBg));

	gui->setFont("assets/fonts/mono.ttf");
	gui->setFontSize(OFX_UI_FONT_LARGE, 9);
	gui->setFontSize(OFX_UI_FONT_MEDIUM, 7);
	gui->setFontSize(OFX_UI_FONT_SMALL, 6);

	gui->addWidgetDown(new ofxUISpacer(width - xInit, 0));
	gui->addWidgetDown(new ofxUILabel("COLOUR LOVERS", OFX_UI_FONT_LARGE));
	gui->addWidgetDown(new ofxUISpacer(width - xInit, 2));

	ofxUITextInput *textinput = new ofxUITextInput("search", "Search keyword", width - xInit, OFX_UI_FONT_MEDIUM);
	textinput->setTriggerOnClick(false);
	textinput->setAutoClear(true);

	gui->addWidgetDown(textinput);
	textinput = new ofxUITextInput("loverId", "Lover id", width - xInit, OFX_UI_FONT_MEDIUM);
	textinput->setTriggerOnClick(false);

	gui->addWidgetDown(textinput);
	textinput = new ofxUITextInput("paletteId", "Palette id", width - xInit, OFX_UI_FONT_MEDIUM);
	textinput->setTriggerOnClick(false);
	gui->addWidgetDown(textinput);

	gui->addWidgetDown(new ofxUILabelButton("FAVS", false, 0.5 * width - xInit, dim, OFX_UI_FONT_MEDIUM));
	gui->addWidgetRight(new ofxUILabelButton("HISTORY", false, 0.5 * width - xInit, dim, OFX_UI_FONT_MEDIUM));
	gui->addWidgetDown(new ofxUISpacer(width - xInit, 0));

	int tgSize1 = 10;
	int tgSize2 = 4;
	gui->addWidgetDown(new ofxUIToggle("FIXED WIDTHS", MODE_FixedSize, tgSize1, tgSize1, tgSize2));
	gui->addWidgetDown(new ofxUISpacer(width - xInit, 0));

	gui->addWidgetDown(new ofxUISpacer(width - xInit, 2));

	gui->addWidgetDown(new ofxUISpacer(width - xInit, 0));
	gui->addWidgetDown(new ofxUILabel("PALETTE NAME:", OFX_UI_FONT_MEDIUM));
	lastPaletteName_UI = new ofxUILabel(lastPaletteName.get(), OFX_UI_FONT_SMALL);
	gui->addWidgetDown(lastPaletteName_UI);
	lastPaletteName_UI->setLabel(lastPaletteName.get());

	gui->addWidgetDown(new ofxUISpacer(width - xInit, 0));

	gui->addWidgetDown(new ofxUILabelButton("ADD FAVOURITE", false, width - xInit, dim));
	gui->addWidgetDown(new ofxUILabelButton("REMOVE FAVS", false, width - xInit, dim, OFX_UI_FONT_SMALL));
	gui->addWidgetDown(new ofxUILabelButton("REMOVE HISTORY", false, width - xInit, dim, OFX_UI_FONT_SMALL));

	gui->addWidgetDown(new ofxUISpacer(width - xInit, 0));

	tgSize1 = 10;
	tgSize2 = 4;
	gui->addWidgetDown(new ofxUIToggle("PALETTE PICK", MODE_PickPalette_BACK, tgSize1, tgSize1, tgSize2));
	gui->addWidgetDown(new ofxUIToggle("COLOR PICK", MODE_PickColor_BACK, tgSize1, tgSize1, tgSize2));

	//TODO
	//getTopPalettesForLover
	//searchPalettes

	//-

	//callbacks
	ofAddListener(gui->newGUIEvent, this, &ofxColourLoversHelper::Changed_Gui);
#endif

	//-

	ofAddListener(ColourLoveEvent::events, this, &ofxColourLoversHelper::Changed_ColourLovers);

	//-

	params.setName("ofxColourLoversHelper");
	params.add(MODE_PickPalette_BACK);
	params.add(MODE_PickColor_BACK);
	params.add(MODE_FixedSize);
	params.add(MODE_Slim);
	params.add(ENABLER_Keys);
	params.add(SHOW_BrowserPalettes);
	params.add(AutoScroll);
	params.add(lastMenuTab);
	//params.add(lastSearch);
	params.add(bFavorites);
	params.add(bHistory);

	//bFavorites.setSerializable(false);
	//bHistory.setSerializable(false);

	//ofxSurfingHelpers::loadGroup(params, path_AppSettings);

	//----

	// startup

	currPalette = -1;
	updateFlag = false;

#ifdef USE_OFX_UI
	gui_Palette = 0;
	gui_Lab = 0;
#endif

	//-


	//--------------------------------------------------------------
	listener_bFavorites = bFavorites.newListener([this](bool &b) {
		ofLogNotice("ofxColorManager > bFavorites: ") << b;

		bHistory = !bFavorites;
		if (b) loadFavorites();
	});

	//--------------------------------------------------------------
	listener_bHistory = bHistory.newListener([this](bool &b) {
		ofLogNotice("ofxColorManager > bHistory: ") << b;

		bFavorites = !bHistory;
		if (b) loadHistory();
	});


	if (lastMenuTab.get() == "FAVORITES") loadFavorites();
	else if (lastMenuTab.get() == "HISTORY") loadHistory();
	loadFavorites();

	//--

	// auto load first palette of favourites
	if (palettes.size() > 0)
	{
		currPalette = 0;
		//updateFlag = true;
		//setPalette(currPalette);
		refreshPalette();
	}

	//loadHistory();
	//// auto load first palette of favourites
	//if (palettes.size()>0)
	//{
	// currPalette = 0;
	//// updateFlag = 1;
	//// setPalette(currPalette);
	// refreshPalette();
	// }

	ofxSurfingHelpers::loadGroup(params, path_AppSettings);
	}


//--------------------------------------------------------------
void ofxColourLoversHelper::setup(glm::vec2 _position, glm::vec2 _size)
{
	position = _position;
	size = _size;

	setup();
}

//--------------------------------------------------------------
void ofxColourLoversHelper::setPosition(glm::vec2 _position, glm::vec2 _size)
{
	position = _position;
	size = _size;

#ifdef USE_OFX_UI
	gui->setPosition(position.x, position.y);
#endif

}

//--------------------------------------------------------------
void ofxColourLoversHelper::setGrid(glm::vec2 _position, glm::vec2 _size)
{
	gridPosition = _position;
	gridSize = _size;
}

//--------------------------------------------------------------
void ofxColourLoversHelper::Changed_ColourLovers(ColourLoveEvent &e)
{
	ofLogNotice(__FUNCTION__) << ofToString(e.message);

	palettes = e.palettes;

	if (!(palettes.size() > 0))
	{
		return;
	}

	// save results into history after succesfuly query search
	for (int i = 0; i < palettes.size(); i++)
	{
		e.palettes[i].save(path_Global + "history/" + e.palettes[i].id + ".xml");
	}

	//return;
	//bg = palettes[0].sortedColours[0];
	//bgLabel->setLabel("BG: "+ofxColourLovers::hexToWeb(bg));

	updateFlag = true;
}

//--------------------------------------------------------------
void ofxColourLoversHelper::build_Gui_Lab()
{
	ofLogNotice(__FUNCTION__);

	// populate color boxes of all palettes 

#ifdef USE_OFX_UI

	if (gui_Lab)
	{
		ofRemoveListener(gui_Lab->newGUIEvent, this, &ofxColourLoversHelper::Changed_Gui_Lab);
		delete gui_Lab;
		gui_Lab = 0;
	}

	int xInit = 6;
	int padding = 5;
	int width;
	width = gridSize.x;

	gui_Lab = new ofxUIScrollableCanvas(gridPosition.x, gridPosition.y, width, gridSize.y);

	gui_Lab->setFont("assets/fonts/mono.ttf");
	gui_Lab->setFontSize(OFX_UI_FONT_LARGE, 9);
	gui_Lab->setFontSize(OFX_UI_FONT_MEDIUM, 7);
	gui_Lab->setFontSize(OFX_UI_FONT_SMALL, 6);
	gui_Lab->setScrollableDirections(false, true);
	gui_Lab->setScrollAreaToScreenHeight();
	gui_Lab->addWidgetDown(new ofxUISpacer(width - xInit, 0));

	ofAddListener(gui_Lab->newGUIEvent, this, &ofxColourLoversHelper::Changed_Gui_Lab);

	int cdim;
	int cdist = 1;
	int col = 0;
	int row = 0;
	int startY = 50;
	float w_Gui = width - 2;

	// height color/palette boxes
	//cdim = 20;
	//maybe some palette have less than 5 colors, ie: 4, and then can cause problems...
	//so we fix to 5 as usual they have..
	cdim = w_Gui / 5.;

	//gui_Lab->centerWidgets();
	gui_Lab->addWidgetDown(new ofxUILabel(lastSearch, OFX_UI_FONT_MEDIUM));
	gui_Lab->addWidgetDown(new ofxUISpacer(width - xInit, 2));

	//-

	for (int i = 0; i < palettes.size(); i++)
	{
		int currX = 1;
		int currW = 0;

		// colors in each palette
		int _sizeP = palettes[i].colours.size();
		for (int c = 0; c < _sizeP; c++)
		{
			if (!MODE_FixedSize)
			{
				// different sizes with original colourLover Palettes
				currW = palettes[i].colorWidths[c] * w_Gui;
			}
			else
			{
				// same size for each color
				currW = w_Gui / _sizeP;
			}

			std::string butName = ("CL_" + ofToString(i) + "_" + ofToString(c));
			ofxUIButton *btn = new ofxUIButton(butName, false,
				currW, cdim,
				currX, i * (cdim + 4) + startY);

			btn->setLabelVisible(0);
			gui_Lab->addWidget(btn);

			// For set colour issues, make sure to set fill colour after widget been added
			// color filled box
			btn->setDrawFill(true);
			btn->setColorFill(palettes[i].colours[c]);
			btn->setColorBack(palettes[i].colours[c]);
			btn->setDrawBack(true);

			// mark selector settings colors borders & disable
			btn->setColorOutline(colorMarked);
			btn->setDrawOutline(false);

			buttonColoursPalette.push_back(btn);

			currX += currW;
}
}

	gui_Lab->getRect()->setHeight(palettes.size() * (cdim + 4) + startY);
	gui_Lab->setSnapping(0);

	updateFlag = false;
#endif

	//-

//#ifdef USE_OFX_UI
	//workflow
	currPalette = 0;
	setPalette(currPalette);
	refreshPalette();
	//#endif
}

//--------------------------------------------------------------
void ofxColourLoversHelper::update()
{
	//bool b = gui->bInsideCanvas;
	//cout << b << endl;

	if (updateFlag)
	{
		build_Gui_Lab();

		updateFlag = false;
	}
}

//--------------------------------------------------------------
bool ofxColourLoversHelper::draw()
{
	bCheckMouseOverTextInputLovers = bTextInputActive;

	//--

#ifdef USE_OFX_IM_GUI
	if (bIsVisible)	drawImGuiMain();
	else bCheckMouseOverTextInputLovers = false;
#endif

	////draw raw palettes without gui
	//if(palettes.size()>0){
	//    for(int i=0;i<palettes.size();i++){
	//         palettes[i].draw(500,25*i,500,20);
	//    }
	//}

	return bCheckMouseOverTextInputLovers;
}

#ifdef USE_OFX_UI
//--------------------------------------------------------------
void ofxColourLoversHelper::Changed_Gui(ofxUIEventArgs &e)
{
	std::string name = e.widget->getName();
	int kind = e.widget->getKind();
	ofLogNotice(__FUNCTION__) << name;

	if (name == "search")
	{
		ofxUITextInput *textinput = (ofxUITextInput *)e.widget;
		lastSearch = textinput->getTextString();
		ofxColourLovers::searchPalettes(textinput->getTextString(), amountResults);
		ofLogNotice(__FUNCTION__) << "textinput: " << textinput->getTextString();
	}

	else if (name == "loverId")
	{
		ofxUITextInput *textinput = (ofxUITextInput *)e.widget;
		lastSearch = textinput->getTextString();
		std::string s = textinput->getTextString();
		ofStringReplace(s, " ", "%20");
		ofxColourLovers::getTopPalettesForLover(s, amountResults);
		ofLogNotice(__FUNCTION__) << "loverId: " << textinput->getTextString();
	}

	else if (name == "paletteId")
	{
		ofxUITextInput *textinput = (ofxUITextInput *)e.widget;
		lastSearch = textinput->getTextString();
		ofxColourLovers::getPalette(lastSearch);
		ofLogNotice(__FUNCTION__) << "paletteId: " << textinput->getTextString();

	}

	else if (name == "ADD FAVOURITE" && currPalette > -1)
	{
		ofxUIButton *but = e.getButton();
		if (but->getValue())
		{
			std::string str = path_Global + "favorites/" + palettes[currPalette].id + ".xml";
			palettes[currPalette].save(str);
			ofLogNotice(__FUNCTION__) << "saved favorite: " << str;
		}
	}

	//-

	else if (name == "FAVS")
	{
		ofxUIButton *but = e.getButton();
		if (but->getValue())
			loadFavorites();
	}
	else if (name == "HISTORY")
	{
		ofxUIButton *but = e.getButton();
		if (but->getValue())
			loadHistory();
	}
	else if (name == "REMOVE FAVS")
	{
		ofxUIButton *but = e.getButton();
		if (but->getValue())
			clearFavourites();
	}
	else if (name == "REMOVE HISTORY")
	{
		ofxUIButton *but = e.getButton();
		if (but->getValue())
			clearHistory();
	}

	else if (name == "FIXED WIDTHS")
	{
		bool MODE_fixedSize_PRE = MODE_FixedSize;
		ofxUIToggle *toggle = e.getToggle();
		MODE_FixedSize = toggle->getValue();
		if (MODE_FixedSize != MODE_fixedSize_PRE)
		{
			build_Gui_Lab();
		}
	}
	else if (name == "PALETTE PICK")
	{
		ofxUIToggle *toggle = e.getToggle();
		MODE_PickPalette_BACK = toggle->getValue();
	}
	else if (name == "COLOR PICK")
	{
		ofxUIToggle *toggle = e.getToggle();
		MODE_PickColor_BACK = toggle->getValue();
	}

	//TODO: disable keys outside handler
	if (kind == OFX_UI_WIDGET_TEXTINPUT)
	{
		ofLogWarning(__FUNCTION__) << "TEXTINPUT FOCUS!";
		//ofLogWarning(__FUNCTION__) << "SHOULD DISABLE KEYS !";
		ofxUITextInput *ti = (ofxUITextInput *)e.widget;

		if (ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_FOCUS)
		{
			ofLogWarning(__FUNCTION__) << "OFX_UI_TEXTINPUT_ON_FOCUS";
			//unfocusAllTextInputs(ti);
			ENABLER_Keys = false;
		}
		else if (ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_LOAD)
		{
			ofLogWarning(__FUNCTION__) << "OFX_UI_TEXTINPUT_ON_LOAD";
			ENABLER_Keys = false;
		}
		else if (ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER)
		{
			ofLogWarning(__FUNCTION__) << "OFX_UI_TEXTINPUT_ON_ENTER";
			ENABLER_Keys = true;
		}
		else if (ti->getInputTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
		{
			ofLogWarning(__FUNCTION__) << "OFX_UI_TEXTINPUT_ON_UNFOCUS";
			ENABLER_Keys = true;
}

		ofLogWarning(__FUNCTION__) << "ENABLER_Keys: " << (ENABLER_Keys ? "TRUE" : "FALSE");
	}

	//-

	//TODO: disabled beacause dont know what does and blocks next/prev by keys
	//currPalette=-1;
}
#endif


// sends back pointers color/palette/name and mark gui selected..
//--------------------------------------------------------------
void ofxColourLoversHelper::refreshPalette()
{
	ofLogNotice(__FUNCTION__) << currPalette;

	//TODO:
	//Windows

	ColourLovePalette p;
	if (palettes.size() > 0)
	{
		p = palettes[currPalette];
		lastPaletteName = p.title;

#ifdef USE_OFX_UI
		lastPaletteName_UI->setLabel(lastPaletteName);
#endif
	}

	//ColourLovePalette p = palettes[currPalette];
	//lastPaletteName = p.title;
	//lastPaletteName_UI->setLabel(lastPaletteName);

	//-

	// get and set palette colors and name BACK

	int sizePalette = p.colours.size();
	if (sizePalette > 0 && myPalette_BACK != nullptr && MODE_PickPalette_BACK)
	{
		// set BACK name clicked
		if (myPalette_Name_BACK != nullptr)
		{
			(*myPalette_Name_BACK) = p.title;
		}

		//-

		// set BACK palette colors
		myPalette_BACK->clear();
		myPalette_BACK->resize(sizePalette);
		(*myPalette_BACK) = p.colours;

		// mark update flag
		if (bUpdated_Palette_BACK != nullptr)
		{
			(*bUpdated_Palette_BACK) = true;
		}
	}

	//-

	//TODO: workflow...auto trig last color too... (it's done before too..but require when controlling by keyboard next/prev palette)

	//    if (MODE_PickPalette_BACK && MODE_PickColor_BACK)
	////    if (MODE_PickColor_BACK)
	//    {
	//        ofColor c;
	//        c = p.colours[0];
	//
	//        // set BACK color clicked
	//        if (myColor_BACK!=nullptr)
	//        {
	////            myColor_BACK->set( lastColor_clicked );
	//            myColor_BACK->set( c );
	//        }
	//
	//        // flag updater color ready
	//        if (bUpdated_Color_BACK!=nullptr && MODE_PickColor_BACK)
	//        {
	//            (*bUpdated_Color_BACK) = true;
	//        }
	//    }

	//--

#ifdef USE_OFX_UI
	// mark borders in all colors in palette as active about currPalette

	for (int i = 0; i < palettes.size(); i++)
	{
		int _sizeP = palettes[i].colours.size();

		for (int c = 0; c < _sizeP; c++)
		{
			std::string butName = ("CL_" + ofToString(i) + "_" + ofToString(c));

			auto e = gui_Lab->getWidget(butName);
			ofxUIButton *btn = (ofxUIButton *)e;

			if (i == currPalette)
			{
				btn->setDrawOutline(true);
			}
			else
			{
				btn->setDrawOutline(false);
			}
}
}
#endif
}

//--------------------------------------------------------------
void ofxColourLoversHelper::randomPalette()
{
	if (palettes.size() == 0)return;

	currPalette = (int)ofRandom(palettes.size());
	ofLogNotice(__FUNCTION__) << currPalette;

	refreshPalette();

	if (MODE_PickColor_BACK)
	{
		// set BACK color clicked
		if (myColor_BACK != nullptr)
		{
			myColor_BACK->set(palettes[currPalette].colours[0]);//auto get first color from palette beacuse there's no color click! just key pressed

			// flag updater color ready
			if (bUpdated_Color_BACK != nullptr && MODE_PickColor_BACK)
			{
				(*bUpdated_Color_BACK) = true;
			}
		}
	}
}

//--------------------------------------------------------------
void ofxColourLoversHelper::nextPalette(bool cycled)
{
	ofLogNotice(__FUNCTION__) << currPalette;

	if (currPalette == -1 || palettes.size() == 0)
	{
		ofLogWarning(__FUNCTION__) << "PALETTE NOT LOADED. SKIP";
		return;
	}

	if (!cycled)//clamped to last
	{
		if (currPalette >= palettes.size() - 1)
		{
			currPalette = 0;
		}
		else currPalette++;
	}
	else//cycled browse
	{
		currPalette++;
		if (currPalette > palettes.size() - 1)
			currPalette = 0;
	}

	ofLogNotice(__FUNCTION__) << currPalette;

	refreshPalette();

	//-

	//TODO: 
	//workflow
	//...auto trig last color too... 
	//(it's done before too..but require when controlling by keyboard next/prev palette)

	//if (MODE_PickPalette_BACK && MODE_PickColor_BACK)
	//if (MODE_PickPalette_BACK && MODE_PickColor_BACK)
	if (MODE_PickColor_BACK)
	{
		// set BACK color clicked
		if (myColor_BACK != nullptr)
		{
			myColor_BACK->set(palettes[currPalette].colours[0]);//auto get first color from palette beacuse there's no color click! just key pressed

			// flag updater color ready
			if (bUpdated_Color_BACK != nullptr && MODE_PickColor_BACK)
			{
				(*bUpdated_Color_BACK) = true;
			}
		}
	}

	if (!AutoScroll) AutoScroll = true;
}

//--------------------------------------------------------------
void ofxColourLoversHelper::prevPalette()
{
	ofLogNotice(__FUNCTION__) << currPalette;
	if (currPalette == -1 || palettes.size() == 0)
	{
		ofLogWarning(__FUNCTION__) << "PALETTE NOT LOADED. SKIP";
		return;
	}

	currPalette--;
	if (currPalette < 0) currPalette = palettes.size() - 1;

	ofLogNotice(__FUNCTION__) << currPalette;

	refreshPalette();

	//-

	//TODO: workflow...auto trig last color too... (it's done before too..but require when controlling by keyboard next/prev palette)
	//if (MODE_PickPalette_BACK && MODE_PickColor_BACK)

	if (MODE_PickColor_BACK)
	{
		// set BACK color clicked
		if (myColor_BACK != nullptr)
		{
			myColor_BACK->set(palettes[currPalette].colours[0]);//auto get first color from palette beacuse there's no color click! just key pressed

			// flag updater color ready
			if (bUpdated_Color_BACK != nullptr && MODE_PickColor_BACK)
			{
				(*bUpdated_Color_BACK) = true;
			}
		}
	}

	if (!AutoScroll) AutoScroll = true;
}

#ifdef USE_OFX_UI
//--------------------------------------------------------------
void ofxColourLoversHelper::Changed_Gui_Lab(ofxUIEventArgs &e)
{
	std::string name = e.widget->getName();
	int kind = e.widget->getKind();
	int uid = e.widget->getID();

	//-

	// filter mousePressed only to pick a palette/color

	bool isButtonColor_click = false;
	if (kind == OFX_UI_WIDGET_BUTTON)
	{
		ofxUIButton *but = e.getButton();
		if (but->getValue())
		{
			isButtonColor_click = true;
			return;
		}
	}

	//-

	// 1. filter name

	std::string whatList = name.substr(0, 2);
	std::string whatColId = name.substr(3, name.length() - 3);

	vector<std::string> seg = ofSplitString(name, "_");
	int pId = ofToInt(seg[1]);
	int cId = ofToInt(seg[2]);
	ColourLovePalette p = palettes[pId];

	ofLogNotice(__FUNCTION__) << name << " " << kind << " " << uid << " colour: " << p.colours[cId] << " name: " << p.title;

	lastPaletteName = p.title;
	lastPaletteName_UI->setLabel(lastPaletteName);

	//-

	// 2. pick color

	lastColor_clicked = p.colours[cId];

	// set BACK color clicked
	if (myColor_BACK != nullptr)
	{
		myColor_BACK->set(lastColor_clicked);
	}

	// flag updater color ready
	if (bUpdated_Color_BACK != nullptr && MODE_PickColor_BACK)
	{
		(*bUpdated_Color_BACK) = true;
	}

	//-

	// 3. set palette
	//setPalette(pId);

	currPalette = pId;
	setPalette(currPalette);

	ofLogNotice(__FUNCTION__) << "palette Id : " << pId;
	ofLogNotice(__FUNCTION__) << "currPalette: " << currPalette;

	refreshPalette();

	//-
		}
#endif

//--------------------------------------------------------------
void ofxColourLoversHelper::setPalette(int pId)
{
	ofLogNotice(__FUNCTION__) << pId;

	//skip if its already loaded?
	if (currPalette == pId)
	{
		return;
	}
	currPalette = pId;

	//    ColourLovePalette p = palettes[pId];
	////    lastPaletteName = p.title;
	//
	//    //--
	//
	//    // get palettes BACK
	//
	////            //TODO: not required?
	////    int sizePalette = p.colours.size();
	////    if (sizePalette>0 && myPalette_BACK!= nullptr)
	////    {
	////        myPalette_BACK->clear();
	////        myPalette_BACK->resize(sizePalette);
	////        (*myPalette_BACK) = p.colours;
	//////        lastPaletteName = p.title;
	////    }
}

#ifdef USE_OFX_UI
//--------------------------------------------------------------
void ofxColourLoversHelper::Changed_ColourPalette(ofxUIEventArgs &e)
{
	//    std::string name = e.widget->getName();
	//    int kind = e.widget->getKind();
	//    int uid = e.widget->getID();
	//
	//    //TODO: add button with same name
	//    if(name=="ADD FAVOURITE" && currPalette>-1)
	//    {
	//        std::string str = "palettes/favourites/"+palettes[currPalette].id+ ".xml";
	//        palettes[currPalette].save(str);
	//        ofLogNotice(__FUNCTION__)<<"saved favorite: "<<str;
	//    }
	//    else
	//    {
	//        vector<std::string> seg = ofSplitString(name,", ");
	//        int r = ofToInt(seg[0]);
	//        int g = ofToInt(seg[1]);
	//        int b = ofToInt(seg[2]);
	//
	//        ofLogNotice(__FUNCTION__)<<"Changed_ColourPalette: "<<r<<" g "<<g <<" b "<<b;
	//    }
}
#endif

//-

//--------------------------------------------------------------
void ofxColourLoversHelper::loadFavorites()
{
	ofLogNotice(__FUNCTION__);

	//ofDirectory _files(path_Global + "favorites");
	_files.reset();
	_files.open(path_Global + "favorites");
	_files.listDir();
	palettes.clear();
	pNames.clear();

	for (int i = 0; i < _files.numFiles(); i++)
	{
		ColourLovePalette cp;
		cp.load(path_Global + "favorites/" + _files.getName(i));
		palettes.push_back(cp);
		pNames.push_back(cp.title);
	}

	lastSearch = "FAVORITES";
	lastMenuTab = lastSearch;

	if (!bFavorites) bFavorites = true;

	//lastSearch_PRE = lastSearch;

	//TODO:
	//BUG: CRASHES IF EMPTY FOLDER
	if (_files.numFiles() > 0) build_Gui_Lab();

	////TODO: startup
	//currPalette = 0;
	////updateFlag = 1;
	//setPalette(currPalette);
	//refreshPalette();
}

//--------------------------------------------------------------
void ofxColourLoversHelper::clearFavourites()
{
	ofLogNotice(__FUNCTION__);

	//ofDirectory _files(path_Global + "favorites");
	_files.reset();
	_files.open(path_Global + "favorites");

	ofxSurfingHelpers::CheckFolder(path_Global + "favorites");

	_files.listDir();

	for (int i = 0; i < _files.numFiles(); i++)
	{
		_files[i].remove();
	}
}

//--------------------------------------------------------------
void ofxColourLoversHelper::loadHistory()
{
	ofLogNotice(__FUNCTION__);

	ofxSurfingHelpers::CheckFolder(path_Global + "history");

	//ofDirectory _files(path_Global + "history");
	_files.reset();
	_files.open(path_Global + "history");
	_files.listDir();
	palettes.clear();
	pNames.clear();

	for (int i = 0; i < _files.numFiles(); i++)
	{
		ColourLovePalette cp;
		cp.load(path_Global + "history/" + _files.getName(i));
		palettes.push_back(cp);
		pNames.push_back(cp.title);
	}

	lastSearch = "HISTORY";
	lastMenuTab = lastSearch;

	if (!bHistory) bHistory = true;

	//lastSearch_PRE = lastSearch;

	//TODO: 
	//BUG: CRASHES IF EMPTY FOLDER
	//if (_files.numFiles() > 0) build_Gui_Lab();

	build_Gui_Lab();

	////TODO: startup
	//currPalette = 1;
	////updateFlag = 1;
	//setPalette(currPalette);
	//refreshPalette();
}

//--------------------------------------------------------------
void ofxColourLoversHelper::clearHistory()
{
	ofLogNotice(__FUNCTION__);

	ofxSurfingHelpers::CheckFolder(path_Global + "history");

	//ofDirectory _files(path_Global + "history");
	_files.reset();
	_files.open(path_Global + "history");
	_files.listDir();

	for (int i = 0; i < _files.numFiles(); i++)
	{
		_files[i].remove();
	}
}

//-

// pointers back to 'communicate externally'

//--------------------------------------------------------------
void ofxColourLoversHelper::setColor_BACK(ofColor &c)
{
	myColor_BACK = &c;
}

//--------------------------------------------------------------
void ofxColourLoversHelper::setPalette_BACK(vector<ofColor> &p)
{
	myPalette_BACK = &p;
}

//--------------------------------------------------------------
void ofxColourLoversHelper::setPalette_BACK_Refresh(bool &b)
{
	bUpdated_Palette_BACK = &b;
}

//--------------------------------------------------------------
void ofxColourLoversHelper::setColor_BACK_Refresh(bool &b)
{
	bUpdated_Color_BACK = &b;
}

//--------------------------------------------------------------
void ofxColourLoversHelper::setPalette_BACK_Name(std::string &n)
{
	myPalette_Name_BACK = &n;
}

//--------------------------------------------------------------
void ofxColourLoversHelper::keyPressed(ofKeyEventArgs &eventArgs)
{
	if (ENABLER_Keys && !bCheckMouseOverTextInputLovers)
	{
		const int key = eventArgs.key;

		// modifiers
		bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
		bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
		bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
		bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);

		ofLogNotice(__FUNCTION__) << " : " << key;

		//-

		if (key == 'g')
		{
			setToggleVisible();
		}

		// browse
		if (key == OF_KEY_DOWN /*|| key == ' ' && !mod_CONTROL*/)
		{
			nextPalette();
		}
		if (key == OF_KEY_UP /*|| key == ' ' && mod_CONTROL*/)
		{
			prevPalette();
		}

		if (key == OF_KEY_BACKSPACE)
		{
			bFavorites = !bFavorites;
		}

		if (key == OF_KEY_RETURN)
		{
			randomPalette();
		}

		//if (key == 'f')
		//{
		//    std::string str = "palettes/favourites/"+palettes[currPalette].id+ ".xml";
		//    palettes[currPalette].save(str);
		//    ofLogNotice(__FUNCTION__)<<"saved favorite: "<<str;
		//}
	}
}

//--------------------------------------------------------------
void ofxColourLoversHelper::keyReleased(ofKeyEventArgs &eventArgs)
{
	if (eventArgs.key == ' ')
	{
	}
}

//--------------------------------------------------------------
void ofxColourLoversHelper::addKeysListeners()
{
	ofAddListener(ofEvents().keyPressed, this, &ofxColourLoversHelper::keyPressed);
}

//--------------------------------------------------------------
void ofxColourLoversHelper::removeKeysListeners()
{
	ofRemoveListener(ofEvents().keyPressed, this, &ofxColourLoversHelper::keyPressed);
}

//--------------------------------------------------------------
void ofxColourLoversHelper::mouseDragged(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;
}

//--------------------------------------------------------------
void ofxColourLoversHelper::mousePressed(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;
}

//--------------------------------------------------------------
void ofxColourLoversHelper::mouseReleased(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;
}

//--------------------------------------------------------------
void ofxColourLoversHelper::addMouseListeners()
{
	ofAddListener(ofEvents().mouseDragged, this, &ofxColourLoversHelper::mouseDragged);
	ofAddListener(ofEvents().mousePressed, this, &ofxColourLoversHelper::mousePressed);
	ofAddListener(ofEvents().mouseReleased, this, &ofxColourLoversHelper::mouseReleased);
}

//--------------------------------------------------------------
void ofxColourLoversHelper::removeMouseListeners()
{
	ofRemoveListener(ofEvents().mouseDragged, this, &ofxColourLoversHelper::mouseDragged);
	ofRemoveListener(ofEvents().mousePressed, this, &ofxColourLoversHelper::mousePressed);
	ofRemoveListener(ofEvents().mouseReleased, this, &ofxColourLoversHelper::mouseReleased);
}

//--------------------------------------------------------------
void ofxColourLoversHelper::exit()
{
	ofxSurfingHelpers::saveGroup(params, path_AppSettings);

	//-

	removeKeysListeners();
	removeMouseListeners();

#ifdef USE_OFX_UI
	ofRemoveListener(gui_Lab->newGUIEvent, this, &ofxColourLoversHelper::Changed_Gui_Lab);
	//delete gui_Lab;

	ofRemoveListener(gui->newGUIEvent, this, &ofxColourLoversHelper::Changed_Gui);
	//delete gui;
#endif

	ofRemoveListener(ColourLoveEvent::events, this, &ofxColourLoversHelper::Changed_ColourLovers);
	}

//--------------------------------------------------------------
ofxColourLoversHelper::~ofxColourLoversHelper()
{
	exit();
}

//--------------------------------------------------------------
void ofxColourLoversHelper::windowResized(int w, int h)
{
	//build_Gui_Lab();
}
