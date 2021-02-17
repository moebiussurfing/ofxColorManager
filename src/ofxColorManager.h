
#pragma once

#include "ofMain.h"

/*

TODO:

+ theory and range selectors do not update demo2 svg
+ fix theory: sometimes not upadting demo2 svg colors
+ should clean: disable al and enable the required only

+ palette editor broken

+ fix colour lovers growing gui
+ fix pick color: lovers, quantizer..
+ add callback disabler for engines when using presets?

*/

//#define USE_SIMPLE_PRESET_PALETTE

//----------

// OPTIONAL

//#define USE_RECTANGLE_INTERFACES // TODO: a custom final user size ofxInterface

//TODO:
// an extended preset format
//#define USE_BUNDLE_TYPE_PRESET

//	modules
// can't be disabled now..
#define USE_COLOR_LOVERS
#define USE_IMAGE_QUANTIZER
#define USE_OFX_COLOR_BROWSER
#define USE_EXTRA_LIBRARIES

//	layout
#define MAX_PALETTE_COLORS 10
#define INCLUDE_IMGUI_CUSTOM_THEME_AND_FONT
#define BUTTON_BIG_HEIGHT 50
#define BUTTON_COLOR_SIZE 40
#define COLOR_STRIP_COLOR_HEIGHT 15
#define PANEL_WIDGETS_WIDTH 200
#define PANEL_WIDGETS_HEIGHT 500

// extra
//#define USE_UNDO_ENGINE
#define USE_DEBUG_LAYOUT // includes mouse ruler
//#define USE_SUPER_LOG
//#define USE_OFX_GUI

#define AUTO_DRAW_CALLBACK

#define LINK_TCP_MASTER_CLIENT
#ifdef LINK_TCP_MASTER_CLIENT
#include "ofxNetwork.h"
#endif

//------

#include <random>

//#include "presets/PresetManager.h"
#include "presets/PresetPalette.h"

#include "demo/DEMO_Scene.h"
#include "demo/DEMO_Svg.h"

#include "client/PreviewPaletteMini.h"
//using namespace ofxColorClient;

//-

#include "ofxColorPalette.h"

#ifdef USE_OFX_COLOR_BROWSER
#include "ofxColorsBrowser.h"
#endif

#ifdef USE_COLOR_LOVERS
#include "ofxColourLoversHelper.h"
#endif

#include "ColorWheelScheme.h"
#include "ColorWheelSchemes.h"

#include "ofxColorMorph.h"
using namespace ofxColorMorph;
using namespace ofxColorTheory;

#ifdef USE_IMAGE_QUANTIZER
#include "ofxColorQuantizerHelper.h"
#endif

//--

#ifdef USE_UNDO_ENGINE
#include "ofxUndoSimple.h"
#endif

#ifdef USE_DEBUG_LAYOUT
#include "ofxMouseRuler.h"
#endif

#ifdef USE_SUPER_LOG
#include "ofxSuperLog.h"
#endif

#include "ofxSerializer.h"
#include "ofxSurfingHelpers.h"
#include "ofxSurfing_ImGui.h"
using namespace ofxSurfingHelpers;

//-

// NOTE: all color theory stuff it's badly spreaded a bit..
// bc I am using some different addons combined..
// this should be improved, and it's a kind of drawback now.
#define NUM_COLOR_THEORY_TYPES_G1 8
#define NUM_COLOR_THEORY_TYPES_G2 7
#define NUM_TYPES_RANGES 12

//-

// gui

#ifdef USE_OFX_GUI
#include "ofxGui.h"
#endif

#include "ofxImGui.h"

//TODO:
//for a custom user gui
#ifdef USE_RECTANGLE_INTERFACES
#include "ofxInterface.h"
#include "interface/ButtonPaletteSelector.h"
#include "interface/ButtonExample.h"
#endif

#include "ImGui_PalettesPicker.h"
using namespace ImGui_PalettesPicker;

#include "ofxInteractiveRect.h" // engine to move the gui. TODO: add resize by mouse too.

#include "GradientEngine.h"

//--

class ofxColorManager : public ofBaseApp
{
#ifdef LINK_TCP_MASTER_CLIENT
	ofxTCPServer TCP;
	//ofTrueTypeFont  mono;
	//ofTrueTypeFont  monosm;
	vector <string> storeText;
	uint64_t lastCheckLink;
	int port = 6666;
	std::string host = "127.0.0.1";
	void updateLink();
	void setupLink();
	void drawLink();
#endif

	//--

private:
	GradientEngine gradientEngine;

	//--

private:
	PreviewPaletteMini miniPreview;

	DEMO_Svg DEMO2_Svg;

	//TODO:
	shared_ptr<ColorWheelScheme> _scheme;

	bool bExportFlag = false;//flag to avoid multiple overflow of calls.. we read on update()

	//TODO:
	enum typeEngine
	{
		TYPE_PRESET = 0,
		TYPE_LOVER,
		TYPE_THEORY,
		TYPE_RANGE,
	};

	//--

public:
	ofxColorManager();
	~ofxColorManager();

	void setup();
	void startup();

	//TODO: 
	//to allow auto update we must set the order priority layer/drawing
#ifndef AUTO_DRAW_CALLBACK
	void draw();
#endif
#ifdef AUTO_DRAW_CALLBACK
	void draw(ofEventArgs & args);
#endif

	void update(ofEventArgs & args);
	//void update();

	//-

public:
	void exit();
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);

	//-

	//OF callbacks
public:
	void keyPressed(ofKeyEventArgs &eventArgs);
	void keyReleased(ofKeyEventArgs &eventArgs);

	void mouseDragged(ofMouseEventArgs &eventArgs);
	void mousePressed(ofMouseEventArgs &eventArgs);
	void mouseReleased(ofMouseEventArgs &eventArgs);
	void mouseScrolled(ofMouseEventArgs &eventArgs);

private:
	void addKeysListeners();
	void removeKeysListeners();
	void addMouseListeners();
	void removeMouseListeners();

	//--

	void draw_Info();

	//--

private:
	float dt;
	float fps;

	//-

	// number of colors. 
	// must be even sometimes to get same size in all palettes
private:
	ofParameter<int> numColors_Engines;
	ofParameter<int> numColors_Range;
	ofParameter<int> numColors_Theory_G1;
	ofParameter<int> numColors_Theory_G2;

	//--

private:
	ofParameter<bool> auto_pilot{ "autoPilot", false };
	int auto_pilot_timer;
	ofParameter<int> auto_pilot_Duration{ "Time", 1, 1, 20 };

	ofParameter<bool> SHOW_ImGui{ "ImGui", true };

	ofParameter<bool> ENABLE_HelpInfo;
	std::string helpInfo;//key commands

private:
	ofParameter<bool> ENABLE_keys{ "KEYS", true };
	bool ENABLE_keys_PRE;

	//-

	bool bTextInputActive = false;

	//-

public:
	// public basic params
	ofParameterGroup params_Debug{ "ofxColorManager" };
	ofParameter<bool> SHOW_MINI_Preview;
	ofParameter<bool> SHOW_ALL_GUI;
	ofParameter<bool> SHOW_Scene;

	ofParameterGroup &getParameters_Debug() {
		params_Debug.add(SHOW_Scene);
		params_Debug.add(SHOW_ALL_GUI);
		params_Debug.add(SHOW_MINI_Preview);

		return params_Debug;
	}

private:

	ofParameter<bool> SHOW_ColourLovers;
	ofParameter<bool> SHOW_ColourLovers_searcher;
	ofParameter<bool> SHOW_BrowserColors;
	ofParameter<bool> SHOW_debugText;
	ofParameter<bool> SHOW_Panels;
	ofParameter<bool> SHOW_Export;
	ofParameter<bool> SHOW_Presets;
	ofParameter<bool> SHOW_Kit;
	ofParameter<bool> SHOW_Demos;
	ofParameter<bool> SHOW_BackGround;
	ofParameter<bool> SHOW_Picker;
	ofParameter<bool> SHOW_Library;
	ofParameter<bool> SHOW_Range;
	ofParameter<bool> SHOW_UserPaletteFloating;
	ofParameter<bool> SHOW_UserPaletteEditor;
	ofParameter<bool> SHOW_Theory;
	ofParameter<bool> SHOW_Quantizer;
	ofParameter<bool> SHOW_Gradient;
	ofParameter<bool> AutoScroll;

	//-

	// app modes
	ofParameter<int> AppMode;
	ofParameter<std::string> AppMode_name;
#define NUM_APP_MODES 5
	enum Element { Element_0, Element_1, Element_2, Element_3, Element_4, Element_COUNT };
	const char* element_names[Element_COUNT] = { "NONE", "THEORY", "RANGE", "LOVERS", "PICTURE" };
	int current_element = Element_0;

	//--

	// settings paths
private:
	std::string path_Global;
	std::string path_Kits;
	std::string path_FileExport;
	std::string path_AppState;
	std::string path_Presets;
	std::string path_Palettes;
	//export
	std::string path_Name_ExportColor;
	ofParameter <std::string> path_Folder_ExportColor_Custom;
	ofParameter <std::string> path_Folder_ExportColor_Data;

	////gradient
	//std::string path_Folder_Gradient;
	//std::string path_Layout;
	//std::string path_Gradient_LUT;
	//std::string path_Gradient_Preset;
	//std::string path_Name_Gradient;

	//--

	// colors library layout
private:
	ofParameter<bool> lib_Responsive_Mode;
	ofParameter<bool> bPagerized;
	ofParameter<int> sizeLibColBox;
	ofParameter<bool> bResponsive_Presets;
	ofParameter<int> sizePaletteBox;

	//-

private:
	ofEventListener listener_LoverName;

	//--

	// gui info display

	std::string name_Theory = "";
	std::string name_Range = "";

	//--

	// current selectors states on user session

	ofParameter<int> last_Index_Theory_PickPalette;
	ofParameter<int> last_Index_Theory{ "Last Theory Index", -1, 0, NUM_COLOR_THEORY_TYPES_G1 - 1 };//selected theory algorithm
	ofParameter<int> last_Index_Range{ "Last Range Index", -1, 0, NUM_TYPES_RANGES - 1 };//selected range algorithm
	ofParameter<int> last_Index_ColorPalette{ "Selected", -1, 0, 0 };//selected color on palette on editing
	ofParameter<int> last_Index_Preset{ "Preset Index", 0, 0, 0 };//selected preset
	int last_Lib_Index = -1;//last library picked color

	//--

private:
	// text labels
	//to help workflow of what's the last engine used...Not using now.
	std::string messageInfo;
	ofTrueTypeFont font;
	ofTrueTypeFont fontSmall;
	ofTrueTypeFont fontMedium;
	ofTrueTypeFont fontBig;
	//bool txt_lineActive[4];//to help workflow of what's the last engine used...Not using now.

	ofParameter<int> last_Index_Type{ "Last Type Index", -1, 0, 3 };//to help workflow of what's the last engine used...Not using now.
	//0 preset name
	//1 lover name
	//2 theory name
	//3 range name

	//--

	// range
private:
	void setup_Range();
	void generate_Range(ofColor col1, ofColor col2);
	bool bRange_Intitiated = false;
	void refresh_Range_AutoUpdate();
	void palette_FromRange(int index);

private:
	ofParameter<ofColor> color_1_Range;
	ofParameter<ofColor> color_2_Range;
	std::vector<ofColor> palette_Range;
	std::vector<std::string> names_Types_Range;
	ofParameter<bool> autoGenerate_Range;
	ofParameter<bool> bAuto_Color1_FromPicker_Range;
	ofParameter<bool> bAuto_Color2_FromPicker_Range;
	ofParameter<bool> bGetPalette_From_Range;
	ofParameterGroup params_Ranges;

	ofParameter<bool> types_Range[NUM_TYPES_RANGES];
	void Changed_Range(ofAbstractParameter &e);

	//extra
	ofParameter<bool> types_Theory_G2[NUM_COLOR_THEORY_TYPES_G2];
	ofParameterGroup params_ColorTheory_G2;

	//-

	// export 
	// engine 
	// to live reload colors file into client addon/app
private:
	ofParameterGroup params_Export;
	ofParameter<bool> bAutoExportPreset;
	ofParameter<bool> bExportPreset_DefaultPath;
#ifndef USE_SIMPLE_PRESET_PALETTE	
	ofParameter<bool> bModeBundlePreset;
	ofParameter<bool> bModePalettePreset;
#endif
	void exportPalette();

	//--

	// quantizer
#ifdef USE_IMAGE_QUANTIZER
private:
	ofxColorQuantizerHelper colorQuantizer;
#endif

	//-

	// theory
private:
	ofParameter<bool> theory_Types_G1[NUM_COLOR_THEORY_TYPES_G1];
	shared_ptr<ColorWheelScheme> scheme;
	vector<ofColor> colors_Theory_G1[NUM_COLOR_THEORY_TYPES_G1];
	ofParameterGroup params_ColorTheory_G1;
	ofParameter<ofColor> color_TheoryBase;
	ofParameter<int> colorScheme;
	ofParameter<std::string> colorSchemeName;
	ofParameter<bool> bAuto_Theory_FromPicker;

	void Changed_ColorTheory(ofAbstractParameter &e);
	void refresh_Theory_G1();

	//-

	void Changed_ParamsPalette(ofAbstractParameter &e);
	ofParameterGroup params_Palette;

	//-

private:
	// palette layout
	ofParameter<int> boxSizeUser;
	ofParameter<int> boxMaxRows;
	ofParameter<float> scale_ColPalette;
	ofParameter<bool> bFlipUserPalette;
	ofParameter<float> scale_ColRange;

	//-

private:
	void setup_Theory_G1();
	void refresh_Theory_G2_2();

	//-

private:
	std::string scheme_Analogous_name;
	std::string scheme_Complementary_name;
	std::string scheme_SplitComplementary_name;
	std::string scheme_Compound_name;
	std::string scheme_FlippedCompound_name;
	std::string scheme_Monochrome_name;
	std::string scheme_Tetrad_name;
	std::string scheme_Triad_name;

	shared_ptr<ColorWheelScheme> scheme_Analogous;
	shared_ptr<ColorWheelScheme> scheme_Complementary;
	shared_ptr<ColorWheelScheme> scheme_SplitComplementary;
	shared_ptr<ColorWheelScheme> scheme_Compound;
	shared_ptr<ColorWheelScheme> scheme_FlippedCompound;
	shared_ptr<ColorWheelScheme> scheme_Monochrome;
	shared_ptr<ColorWheelScheme> scheme_Tetrad;
	shared_ptr<ColorWheelScheme> scheme_Triad;

	vector<ofColor> colors_Analogous;
	vector<ofColor> colors_Complementary;
	vector<ofColor> colors_SplitComplementary;
	vector<ofColor> colors_Compound;
	vector<ofColor> colors_FlippedCompound;
	vector<ofColor> colors_Monochrome;
	vector<ofColor> colors_Tetrad;
	vector<ofColor> colors_Triad;

	//--

	// DEMO
private:
	DEMO_Scene myDEMO1;

	//--

private:

	std::string last_Lib_NameColor = "";
	int last_ColorPicked_Lib;

	//-

	//layout selected boxes colors
	//ImVec4 color_Pick{ 0, 0, 0, 0.75f };//black
	//float linew_Pick = 3.0f;
	ImVec4 color_Pick{ 1, 1, 1, 0.5f };//white
	float linew_Pick = 1.0f;

	//-

	// colour lovers
#ifdef USE_COLOR_LOVERS
private:
	ofxColourLoversHelper colourLoversHelper;
#endif

	//--

private:
	void build_Palette_Engine();
	void build_Palette_Preset();
	void build_Palette_Flip();

	//--

public:
	//--------------------------------------------------------------
	void setFps(float _fps)//for the demo speed only
	{
		fps = _fps;
		dt = 1. / fps;
	}

	//----

	// pointers backs 
	// to call refresh and link addons 
	// (quantizer+lovers) with addon palette/color
private:
	std::string myPalette_Name_BACK = "";
	ofColor myColor_BACK;
	vector<ofColor> myPalette_BACK;
	bool bUpdated_Palette_BACK = false;
	bool bUpdated_Color_BACK = false;

	//----

public:

	// API 

	// pointers back to autoupdate 
	// ofApp project local variables, registered
	// must be initialized before setup

	void setName_TARGET(std::string &s);
	std::string *name_TARGET = NULL;

	void setColor_TARGET(ofColor &c);
	ofColor *color_TARGET = NULL;//backwards pointer to ofApp picker color

	void setColorBg_TARGET(ofColor &c);
	ofColor *colorBg_TARGET = NULL;//backwards pointer to ofApp background color

	void setPalette_TARGET(vector<ofColor> &p);
	vector<ofColor> *palette_TARGET = NULL;//backwards pointer to ofApp palette

private:
	void refresh_Palette_TARGET(vector<ofColor> &p);

	//-----------------------------------------------------------

public:

	// API 

	//-

	// getters

	vector<ofColor> getPalette();
	std::string getPaletteName();
	ofColor getColor(int index = -1);

	//get color from gradient at prc
	ofColor getColorAtPercent(float control)
	{
		gradientEngine.getColorAtPercent(control);
	}

	//-

	// setters

public:
	//void setControl_Gradient(float control);
	void setVisible(bool b);
	void setToggleVisible();
	void setVisible_GUI_MINI(bool b);
	void setVisible_debugText(bool b);

	//-----------------------------------------------------------

#ifdef USE_DEBUG_LAYOUT
private:
	ofxMouseRuler mouseRuler;
#endif

	//--

	// manager

private:
	ofParameter<bool> bEditPalette;
	ofParameter<bool> bRandomColor;
	ofParameter<bool> bAddColor;
	ofParameter<bool> bRemoveColor;
	ofParameter<bool> bClearPalette;

	//--

	// parameters

private:
	//ofParameterGroup params_data;
	ofParameterGroup params_engines;
	ofParameterGroup params_color;
	ofParameterGroup params_Theory;
	ofParameterGroup params_control;
	//ofParameterGroup params_GradientC;

	void Changed_Controls(ofAbstractParameter &e);

	//--

	// algorithmic palettes

private:
	ofxColorPalette complement;
	ofxColorPalette complementBrightness;
	ofxColorPalette triad;
	ofxColorPalette complementTriad;
	ofxColorPalette monochrome;
	ofxColorPalette monochromeBrightness;
	ofxColorPalette analogue;
	ofxColorPalette random;
	//ofxColorPalette::ColorChannel mode;

	ofParameter<float> analogSpread;

	////TODO:
	////global modificators
	////not implemented yet..
	//ofParameter<int> BRIGHTNESS;
	//ofParameter<int> SATURATION;

	ofParameter<bool> bRandomPalette;
	ofParameter<bool> bAuto_Build_Palette;//trig last used algo palette on click or change color
	//ofParameter<bool> bLock_palette;

private:
	void refresh_Theory_G2();
	void palettes_Resize();

	//----

	// GUI

private:
	ofxImGui::Gui gui;
	ImFont* customFont = nullptr;

	ofxImGui::Settings mainSettings = ofxImGui::Settings();

	bool mouseOverGui;
	bool mouseOverGui_PRE;
	bool bCheckMouseOverTextInput = true;//flag to return mouse over any gui panel or over text input only!

	bool guiVisible;

	bool draw_Gui();

	void gui_Theory();
	void gui_Picker();
	void gui_Library();
	void gui_Background();
	void gui_Range();
	void gui_Panels();
	void gui_Demo();
	void gui_Export();
	void gui_Gradient();

	//--

	// colors

private:
	void refresh_Background();
	ofParameter<ofFloatColor> color_BackGround;//main bg color
	ofParameter<bool> color_backGround_SET;
	ofParameter<bool> AutoSet_BackGround_Color;
	ofParameter<bool> color_BackGround_DarkerMode;
	ofParameter<bool> color_BackGround_GradientMode;
	ofParameter<bool> color_BackGround_Lock;
	ofParameter<float> color_BackGround_AmtDarker;
	ofParameter<bool> background_Draw_ENABLE{ "Draw", false };

	void setBackground_ENABLE(bool b);

	bool ENABLE_Callbacks_cPickers = true;
	bool ENABLE_Callbacks_Engines = true;

	void refresh_Pick_ToHSB();
	void refresh_FromPicked();

	// main color
	ofParameter<ofFloatColor> color_Picked;

	//--

	// undo engine
#ifdef USE_UNDO_ENGINE
	ofxUndoSimple<ofFloatColor> color_Undo;
#endif

	//--

	// color randomizers

	ofParameter<int> color_HUE;
	ofParameter<int> color_SAT;
	ofParameter<int> color_BRG;

	ofParameter<int> color_HUE_0;
	ofParameter<int> color_SAT_0;
	ofParameter<int> color_BRG_0;

	ofParameter<float> color_HUE_Power;
	ofParameter<float> color_SAT_Power;
	ofParameter<float> color_BRG_Power;

	ofParameter<bool> bColor_HUE;
	ofParameter<bool> bColor_SAT;
	ofParameter<bool> bColor_BRG;

	//--

	//TODO: required?
	//TODO: pointer color to get click from button class
	// color clicked comes from theory palette colors
	ofFloatColor color_Clicked2;
	ofFloatColor color_Clicked2_PRE;

	ofFloatColor color_BACK;
	ofFloatColor color_BACK_PRE;

	//--

	// color pickers

private:
	ofParameter<ofFloatColor> color_Clicked;

	void Changed_ColorPicked(ofFloatColor &color);
	void Changed_ColorClicked(ofFloatColor &color);

	void refresh_Pick_ToEngines();

	//--

	// manager

private:
	vector<ofColor> palette;// main user palette

	void palette_AddColor(ofColor c);
	void palette_RemoveColorLast();
	void palette_RemoveColor(int c);
	void palette_Clear();

	void palette_FromTheory(int p);
	void palette_FromColourLovers();
	void palette_FromQuantizer();
	//void palette_FromGradient();

	//----

	// libs library palette
	// using colorBrowser

private:

#ifdef USE_OFX_COLOR_BROWSER
	ofxColorsBrowser colorBrowser;
#endif

	vector<ofColor> palette_Lib_Cols;
	vector<std::string> palette_Lib_Names;

	ofEventListener listener_Library;
	ofEventListener listener_ModeSorting;

	void refresh_Libs();

	int lib_RowSize = 7;//7 colors per row Pantone lib
	int lib_Page_NumColors;
	int lib_TotalColors;
	int lib_Page_Max;

	//gui grid layout
	//rows per page
	ofParameter<int> lib_NumRows{ "Rows Amnt" , 10, 5, 10 * 5 };
	ofParameter<int> lib_Page_Index{ "Page" , 0, 0, lib_Page_Max };
	ofParameter<bool>lib_CardsMode{ "Mode Cards", false };
	ofParameter<int> lib_MaxColumns{ "Columns Max", 7, 1, 7 * 6 };
	ofParameter<float> scale_LibCol{ "Scale", 1, 0.5, 1.5 };

	//--

	// demos
private:
	ofParameter<bool> DEMO1_Test{ "Enable DEMO1", false };
	ofParameter<bool> DEMO2_Test{ "Enable DEMO2", false };
	ofParameter<bool> DEMO2_Edit{ "Edit DEMO2", false };
	ofParameter<float> DEMO2_Scale{ "Scale", 0.2, 0, 1.0f };
	ofParameter<bool> DEMO_Auto{ "Auto Trig", false };
	ofParameter<bool> DEMO_Cam{ "Edit Camera", false };
	ofParameter<float> DEMO_Timer{ "Frequency", 0.5, 0, 1 };
	ofParameter<float> DEMO_Alpha{ "Alpha", 0.8, 0, 1 };
	int Demo_Timer = 0;
	int Demo_Timer_Max = 15000;
	bool bTEST_pause = false;

	//--

	// app settings xml
private:
	ofParameterGroup params_AppState;//without callbacks, just to handle settings
	ofParameterGroup params_Panels{ "PANELS" };
	ofParameterGroup params_Background{ "BACKGROUND" };
	ofParameterGroup params_Demo{ "DEMO" };
	ofParameterGroup params_Picker{ "PICKER" };
	ofParameterGroup params_Library{ "LIBRARY" };
	ofParameterGroup params_Palette2{ "PALETTE" };
	//ofParameterGroup params_Gradient{ "GRADIENT" };

	//-

	//export path system dialog
	void processOpenFileSelection(ofFileDialogResult openFileResult) {};
	std::string originalFileExtension;
	bool bOpen = false;

	//----

private:

	// preset manager

	// addon client
	// preset + editor

	//--

	// presets

private:
	PresetPalette PRESET_Temp;

	//TODO:
	//PresetManager myPresetManager;

private:
	//TODO
	//BUG: should create a default preset because if myPreset is not detected it crashes
	//default preset
	std::string PRESET_Name = "_emptyPreset";
	std::string PRESET_Name_Gradient = "_";

	//--

	// guis
	void gui_Presets();
	void gui_PaletteFloating();
	void gui_PaletteEditor();

	//--

private:

	void preset_Save(std::string p);
	void preset_Load(std::string p);

	//TODO: this is to save an standalone palette
	//void palette_save(std::string p);
	//void palette_load(std::string p);

	//-

	//new preset
private:
	ofParameter<bool> MODE_NewPreset{ "NEW PRESET", false };

	std::string textInput_New = "name..";
	//TODO:
	bool focus_1;
	int has_focus = 0;
	char tab[128];
	void refresh_Files(std::string name);

	//--

	// preset files

	void preset_RefreshFiles();
	std::vector<std::string> files_Names;
	std::vector<ofFile> files;
	std::string textInput_temp = "type name";

	vector<vector<ofColor>> palettesKit;
	vector<PresetData> kit;
	//vector<PaletteData> kit;
};

//--

//private:
//	// Helper to display a little (?) mark which shows a tooltip when hovered.
//	// In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.txt)
//	static void HelpMarker(const char* desc)
//	{
//		ImGui::TextDisabled("(?)");
//		if (ImGui::IsItemHovered())
//		{
//			ImGui::BeginTooltip();
//			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
//			ImGui::TextUnformatted(desc);
//			ImGui::PopTextWrapPos();
//			ImGui::EndTooltip();
//		}
//	}
//};
