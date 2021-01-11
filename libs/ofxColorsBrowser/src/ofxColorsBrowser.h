#pragma once
#include "ofMain.h"


//----

//	OPTIONAL

//uncomment to run the bundled example!
//#define USE_OFX_COLOR_BROWSER_INTERFACE // include boxes interface

//----


#include "ofxOpenColor.h"
#include "ofxSurfingHelpers.h"

#ifdef USE_OFX_COLOR_BROWSER_INTERFACE
#include "ofxRectangleUtils.h"
#include "ofxRectangle.h"
#include "ofxFontStash.h"
using namespace ofx;
#endif

#include "ofxGui.h"

// internal shorcuts no add listeners or remove
#define KEY_SHORTCUTS_ENABLE

// sortings
enum
{
	SORTING_ORIGINAL,
	SORTING_NAME,
	SORTING_HUE,
	SORTING_BRIGHTNESS,
	SORTING_SATURATION
};

// palettes
enum
{
	OFX_PANTONE_COLORS,
	OFX_COLOR_NATIVE,
	OFX_OPEN_COLOR
};

// color struct
typedef struct
{
	std::string name;
	ofColor color;
	int position;//original position
} colorMapping_STRUCT;

//--

class ofxColorsBrowser
{

private:
#ifdef USE_OFX_COLOR_BROWSER_INTERFACE
	ofxFontStash font;
#endif

	ofTrueTypeFont font2;
	string helpInfo;

	ofxPanel gui;
	bool bGui = false;

	//-

public:
	ofxColorsBrowser();
	~ofxColorsBrowser();

	void setup();
	void update();
	void draw();
	void exit();

	//----

	// API

	void load_Pantone_JSON();

	void setPosition(glm::vec2 p);
	void setPositionHelper(glm::vec2 p)
	{
		positionHelper = p;
	};

	void switch_palette_Type();
	void nextSortType();
	void set_palette_Type(int p);
	void set_sorted_Type(int p);

	// TODO: resize buttons to fit..
	//void setSize(glm::vec2 size);

	void setRowsSize(int rows);
	void setBoxSize(float size);

	void setVisible(bool b);
	void setVisibleDebug(bool b);
	void setToggleVisibleDebug();

	ofParameter<bool> SHOW_Gui{ "GUI", true };

	//--------------------------------------------------------------
	bool isVisible() {
		return SHOW_Gui.get();
	}

	//--------------------------------------------------------------
	void setToggleVisible()
	{
		SHOW_Gui = !SHOW_Gui.get();
		ofLogNotice(__FUNCTION__) << "SHOW_Gui: " << SHOW_Gui;

		setVisible(SHOW_Gui);

		//SHOW_InterfaceColors = !SHOW_InterfaceColors;
		//setVisible(SHOW_InterfaceColors);
	}

	//--------------------------------------------------------------
	void setEnableInterfaceClicks(bool b)
	{
		ENABLE_clicks = b;

#ifdef USE_OFX_COLOR_BROWSER_INTERFACE
		if (b)
		{
			//addKeysListeners();
			addMouseListeners();
		}
		else
		{
			//removeKeysListeners();
			removeMouseListeners();
		}
#endif
	}

	//--------------------------------------------------------------
	void setToggleEnableKeys() {
		ENABLE_keys = !ENABLE_keys;

		setEnableKeys(ENABLE_keys);
	}

	//--------------------------------------------------------------
	void setEnableKeys(bool b)
	{
		ENABLE_keys = b;

#ifdef USE_OFX_COLOR_BROWSER_INTERFACE
		if (ENABLE_keys) addKeysListeners();
		else removeKeysListeners();
#endif
	}

	//--------------------------------------------------------------
	void setActive(bool b)
	{
		ENABLE_keys = b;

#ifdef USE_OFX_COLOR_BROWSER_INTERFACE
		if (ENABLE_keys) addKeysListeners();
		else removeKeysListeners();
#endif

		setVisible(b);
	}

	//----

	// pointer back
	void setup_colorBACK(ofFloatColor &c);


public:
	// main palette getter
	vector<ofColor> getPalette();
	vector<std::string> getNames();
	int getSize();
	int getLibIndex() {
		return LibraryColors_Index.get();
	}
	int getSizeCards() {
		return cardSize.get();
	}
	std::string getNameLib() {
		return LibraryColors_Index_name.get();
	}

public:
	vector<std::string> colors_PantoneNames;

	//--

private:

	// path for json colors file
	std::string path_Global;
	std::string path_File;
	std::string path_FileSettings;

	//-

private:

	ofParameter<float> boxSize{ "BOX SIZE", 15, 10, 100 };//boxes
	ofParameter<float> boxPad{ "PAD", 1, 0, 10 };
	ofParameter<int> cardSize{ "CARD SIZE", 7, 2, 100 };// minimal card of colors
	ofParameter<int> cardsPerRow{ "CARDS PER ROW", 4, 2, 100 };
	ofParameter<bool> ENABLE_oneCard_MODE{ "ONE CARD MODE", true };
	int perRow = 10;

	int cardNum = 0;
	int cardColor_size = 100;
	int cardColor_pad = 20;
	glm::vec2 cardPos;

	//--

	// main storage

	map<std::string, ofColor> colorNameMap;

	//-

public:
	vector<colorMapping_STRUCT> colors_STRUCT;

	//--

	// pantone colors

private:
	ofJson js;
	vector<ofColor> colors_Pantone;

	//-

	// RECTANGLE MANAGER SYSTEM - OFXRECTANGLE

#ifdef USE_OFX_COLOR_BROWSER_INTERFACE
	//draggable, sortable, align...
	std::vector<ofxRectangle> rectangles;
	std::vector<ofRectangle *> selectedRects;
	ofxRectangle selectedRectsBoundingBox;
	ofxRectangle *draggingRectPtr;
	glm::vec2 dragStart;
	bool isSelecting;
	ofRectangle selectionRect;
	ofAlignHorz hAlign;
	ofAlignVert vAlign;
	ofRectangle *anchorRect;
	std::string keyboardCommands;
	bool showKeyboardCommands;
	std::vector<ofRectangle> packedRects;
#endif

	//-

private:
	void rectangles_update();
	void rectangles_draw();

	//-

	// color converters
	// These string to hex conversions aren't trivial.
	static int stringToHex(std::string hex)
	{
		int aHex;
		stringstream convert(hex);
		convert >> std::hex >> aHex;
		return aHex;
	}

	static void hexToColor(ofColor &col, std::string hex)
	{
		std::string r = hex.substr(0, 2);
		int ri = stringToHex(r);
		std::string g = hex.substr(2, 2);
		int gi = stringToHex(g);
		std::string b = hex.substr(4, 2);
		int bi = stringToHex(b);
		col.set(ri, gi, bi);
	}

	//----

public:
	void keyPressed(ofKeyEventArgs &eventArgs);
	void keyReleased(ofKeyEventArgs &eventArgs);

	void mouseDragged(ofMouseEventArgs &eventArgs);
	void mousePressed(ofMouseEventArgs &eventArgs);
	void mouseReleased(ofMouseEventArgs &eventArgs);

private:
	void addKeysListeners();
	void removeKeysListeners();

	void addMouseListeners();
	void removeMouseListeners();

	//-

	// modes and states

public:
	ofParameter<bool> ENABLE_keys{ "Enable Keys", false };

private:
	bool SHOW_debugText = false;
	bool SHOW_InterfaceColors = true;
	bool ENABLE_clicks = true;
	//bool ENABLE_keys = false;

	bool bShowDebug = false;//for rectangle manager

	//-

	// pointers back

	ofFloatColor color_BACK;
	ofFloatColor color_BACK_PRE;
	ofFloatColor *color_BACK_OFAPP;

	//-

	void buildColors();

	void refresh_Clicks();// to browsing by keys
	void grid_create_boxes();
	void clearInterface();

	//-

	// grid position
	glm::vec2 position;

	// text debug positions
	glm::vec2 positionHelper;

public:

	// 0:PANTONE 1:OFX_NATIVE, 2:OFX_OPEN

	ofParameter<int> LibraryColors_Index{ "Library", 0, 0, 2 };
	ofParameter<std::string> LibraryColors_Index_name{ " ", "" };

	// 0:ORIGINAL, 1:NAME, 2:HUE, 3:BRIGHTNESS, 4:SATURATION, 5:NEXT

	ofParameter<int> MODE_SORTING{ "Sorting Mode", 0, 0, 4 };
	ofParameter<std::string> MODE_SORTING_name{ " ", "" };

	ofParameterGroup params;

	ofEventListener listener_Library;
	ofEventListener listener_ModeSorting;

private:
	// last clicked color box
	std::string currName = "";
	int currColor = -1;
	int currColor_OriginalPos = -1;
};
