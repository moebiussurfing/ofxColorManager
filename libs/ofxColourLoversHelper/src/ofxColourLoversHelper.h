
#pragma once
#include "ofMain.h"

//----
//
// OPTIONAL
//
// You can choice between 2 guis: 
//
// Gui 1: the coolest ImGui based 
#define USE_OFX_IM_GUI
// 
// and uncomment this line only if you want to handle the ImGui instance 
// out of-the-addon, ie: into ofApp instead:
//#define USE_OFX_IM_GUI_EXTERNAL //-> comment required to enable the one only / local ImGui
//
// (Gui 2 outdated but should work)
//#define USE_OFX_UI
//
#define PANEL_WIDGETS_WIDTH 250//this is overwritted by ofxSurfingHelpers..
#define PANEL_WIDGETS_HEIGHT 100
#define BUTTON_BIG_HEIGHT 50
#define BUTTON_BIG_HEIGHT2 35
#define BUTTON_SLIM_HEIGHT 20
//#define BUTTON_SLIM_HEIGHT 14
//
#define USE_FLOATING_SEARCH_WINDOW_WORAROUND
#define MAX_SEARCH_RESULTS 100

//----

#include "ofxSurfingHelpers.h"
#include "ofxSurfing_ImGui.h"

#ifdef USE_OFX_UI
#include "ofxUI.h"
#endif

#ifdef USE_OFX_IM_GUI
#include "ofxImGui.h"
#endif

#include "ofxColourLovers.h"

class ofxColourLoversHelper {

public:

	ofxColourLoversHelper();
	~ofxColourLoversHelper();

	void setup();
	void update();
	bool draw();
	//void draw();
	void exit();
	void windowResized(int w, int h);

private:
	bool bShowSearch = true;
	// shows advancded panels to tweak layout or workflow behaviour

private:
	ofParameter<bool> MODE_Search{ "API SEARCH", false };

	ofParameter<bool> bFavorites{ "FAVORITES", false };
	ofParameter<bool> bHistory{ "HISTORY", false };
	ofParameter<bool> bSearch{ "SEARCH", false };

	ofEventListener listener_bFavorites;
	ofEventListener listener_bHistory;
	ofEventListener listener_bSearch;

	//--

public:
	ofParameterGroup params;

private:
#ifdef USE_OFX_IM_GUI
#ifndef USE_OFX_IM_GUI_EXTERNAL
	ofxImGui::Gui gui_ImGui;
#endif
	void gui_Search();
	void gui_Main();
	void gui_Kit();
	ofxImGui::Settings mainSettings;
#endif

	std::string textInput_temp1 = "";
	std::string textInput_temp1_PRE = "-1";

	ofParameter<bool> SHOW_BrowserPalettes{ "Show Palettes", true };
	ofParameter<bool> AutoScroll{ "AutoScroll", true };

	//--

#ifdef USE_OFX_UI
	ofxUICanvas *gui;
	ofxUIScrollableCanvas *gui_Lab;
	ofxUICanvas *gui_Palette;
	void Changed_Gui(ofxUIEventArgs &e);
	void Changed_Gui_Lab(ofxUIEventArgs &e);
	void Changed_ColourPalette(ofxUIEventArgs &e);
	vector<ofxUIButton *> buttonColoursPalette;
	vector<ofxUIButton *> colourRanges;
	ofxUILabel * lastPaletteName_UI;
#endif

	ofDirectory _files;

private:
	void Changed_ColourLovers(ColourLoveEvent &e);

	//-

	ImVec4 borderLineColor{ 1,1,1,0.5 };
	float borderLineWidth = 2.0;
	bool auto_resize1 = true;
	bool auto_resize2 = false;
	//bool auto_resize = true;
	bool bfocus = false;
	//bool bfocus = true;

public:
	ofParameter<bool> SHOW_AdvancedLayout{ "Show Advanced", false };

	//--

	// API

public:
	void setup(glm::vec2 _position, glm::vec2 _size);
	void setGrid(glm::vec2 _position, glm::vec2 _size);
	void setPosition(glm::vec2 _position, glm::vec2 _size);

	void setToggleVisible();
	bool isVisible() {
		return bIsVisible;
	}

	void setVisible(bool b);
	void setVisibleSearcher(bool b) {
		bSearcherVisible = b;
#ifdef USE_OFX_UI
		gui->setVisible(bSearcherVisible);
#endif
	}

private:
	bool bSearcherVisible = true;

	// colour lovers browsing
public:
	void nextPalette(bool cycled = false);
	void prevPalette();
	void refreshPalette();
	void randomPalette();

	//--

	// pointers back to 'communicate externally'
public:
	void setColor_BACK(ofColor &c);
	void setPalette_BACK(vector<ofColor> &p);
	void setPalette_BACK_Name(std::string &n);
	void setPalette_BACK_Refresh(bool &b);
	void setColor_BACK_Refresh(bool &b);

	// pointers back to 'communicate externally'
private:
	ofColor *myColor_BACK;
	vector<ofColor> *myPalette_BACK;
	std::string *myPalette_Name_BACK;
	bool *bUpdated_Palette_BACK;
	bool *bUpdated_Color_BACK;

	//----

public:
	ofParameter<bool> MODE_PickPalette_BACK{ "Pick Palette", true };
	ofParameter<bool> MODE_PickColor_BACK{ "Pick Color", true };
	ofParameter<std::string> lastMenuTab_Str{ "_MenuTabPick", "" };

	void setEnableKeys(bool b) {
		ENABLER_Keys = b;
		if (b)
		{
			addKeysListeners();
		}
		else
		{
			removeKeysListeners();
		}
	}

private:
	ofParameter<bool> ENABLER_Keys{ "Enable Keys", true };

private:
	ofParameter<bool> MODE_FixedSize{ "Fixed Width", false };
	ofParameter<bool> MODE_Slim{ "Slim", false };

	ofColor lastColor_clicked;

	int amountResults = (int)MAX_SEARCH_RESULTS;
	//ofParameter<int> amountResults{ "Amount Results", MAX_SEARCH_RESULTS, 10, 300 };

	//-

	void setPalette(int p);
	int palettesX;

	int currPalette; //last picked palette/color
	vector<ColourLovePalette> palettes;
	//vector<ColourLovePalette> palettesSearch;

	bool bUpdateSearchFlag;
	void build_Gui_Lab();

	std::string lastSearch_Str;
	std::string lastSearch_PRE = "-1";

public:
	ofParameter<std::string> lastPaletteName{ "PaletteName", "" };
	//std::string lastPaletteName = "";

private:
	void loadFavorites();
	void loadHistory();
	void clearFavourites();
	void clearHistory();

	std::string path_Global;
	std::string path_AppSettings;

	ofColor colorMarked;

	//---

	bool bIsVisible = true;
	bool isKeysEnabled = true;

	glm::vec2 position;
	glm::vec2 size;
	glm::vec2 gridPosition;
	glm::vec2 gridSize;

	void addKeysListeners();
	void removeKeysListeners();

	void mouseDragged(ofMouseEventArgs& eventArgs);
	void mousePressed(ofMouseEventArgs& eventArgs);
	void mouseReleased(ofMouseEventArgs& eventArgs);

	void addMouseListeners();
	void removeMouseListeners();

	vector<std::string> pNames;
	std::string pName;

	bool bCheckMouseOverTextInputLovers = false;
	bool bTextInputActive = false;

	bool bSearching = false;

public:
	void keyPressed(ofKeyEventArgs& eventArgs);
	void keyReleased(ofKeyEventArgs& eventArgs);


};
