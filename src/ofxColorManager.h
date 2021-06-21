
#pragma once

#include "ofMain.h"

/*


TODO:

++ add color pictures browser to quantizer (copy from litSphere)
++ fix: alert when saving a preset with an already located filename. ask to overwrite
++ fix disabled keys sometimes bc ImGui over Gui?
+ startup init layout by code, not .ini. check windows positions by code to responsive other elemnts

+ export Adobe ASE
+ undo engine
+ add tween transitions to presets ?
+ add cosine/shift gradients engine https://github.com/soufianekhiat/DearWidgets
+ check theory picker if calls too much callbacks
+ plugins for UE4 / Unity3D / Processing ?


BUGS:

+ colour lovers search fails
+ first palette on kit browser do not fades border on first
+ keys disables sometimes
+ fix text input boxes when docking mode. to avoid floating text input box
+ TCP port number switch, some problems on reconnect bc threading not implemented. should use some sync addon or OSC.
+ ImGui pickers hangs flickering sometimes bc max width


*/


//----------

// some preprocessor directives
// mainly for debug purposes only

//#define MAKE_RELEASE_VERSION // -> To be used for deploy. Disables console and other stuff, for final release version. name to Paletto v1.0

//#define USE_VERSION_CHECKER // TODO: crashes on exit..

//--

//TODO:
//ASE exporter
//#define USE_ASE_MODULE
#ifdef USE_ASE_MODULE
#include "ase_common.h"
#include "ase_writer.h"
#endif

// NOTE: can't be disabled now..
// tcp modes: enable only one of the two modes
#define LINK_TCP_MASTER_CLIENT_OF
//#define LINK_TCP_MASTER_CLIENT_KU

//--

#ifdef MAKE_RELEASE_VERSION 
#define APP_RELEASE_NAME "PALETTO v1.0"
//#define APP_RELEASE_NAME "PALETTO v1.0rc"
#else
#define APP_RELEASE_NAME "ofxColorManager"
#endif

//#ifndef MAKE_RELEASE_VERSION 
#define USE_DEBUG_LAYOUT // includes mouse ruler to help layout design. show app console window
//#endif

// optional 
// window manager
#define USE_OFX_WINDOWAPP

#ifdef USE_OFX_WINDOWAPP
#include "ofxWindowApp.h"
#endif

#include "ofxAppUpdateNotifier.h"

//--

#define USE_VIEWPORTS // allow out-of-OF-window
#define MAX_PALETTE_COLORS 20
//#define USE_SVG_MASK // TODO: ofxScene-SVG using masked B&W background. 

//--

// layout dimensions constants
#define COLOR_STRIP_COLOR_HEIGHT 40
#define BUTTON_COLOR_SIZE 40
#define BUTTON_BIG_HEIGHT 50
#define PANEL_WIDGETS_WIDTH 250
#define PANEL_WIDGETS_HEIGHT 500
//#define MODAL_WIDTH 60.0f
#define MAX_PICKER_WIDTH 200
#define LAYOUT_WINDOW_WIDTH 150

//----------

#define INCLUDE_IMGUI_CUSTOM_THEME_AND_FONT

// modules
// NOTE: can't be disabled now..
#define USE_COLOR_LOVERS
#define USE_IMAGE_QUANTIZER
#define USE_OFX_COLOR_BROWSER

// NOTE: all color theory stuff it's badly spreaded a bit..
// bc I am using some different addons combined..
// this should be improved, and it's a kind of drawback now.
#define NUM_COLOR_THEORY_TYPES_G1 8
#define NUM_COLOR_THEORY_TYPES_G2 7
#define NUM_TYPES_RANGES 12

// comment for advanced mode. uncomment for simple user mode.
// hide some widgets for layout customization to minimize elements: keys, layout customize and theme editor
//#define USE_MINIMAL_GUI 

#define MODE_TEXT_INPUT_WORKAROUND// independent floating panel to solve ImGui docking textinput bugs

// extra
//#define MODE_BACKGROUND //TODO: show bg color (gradient picker engine) on mode no background
//#define USE_UNDO_ENGINE //TODO:
//#define USE_RECTANGLE_INTERFACES //TODO: a custom final user size ofxInterface
//#define USE_BUNDLE_TYPE_PRESET //TODO: an extended preset format: +bg, pick states, name...etc
//#define USE_EXTRA_LIBRARIES //TODO: now only Pantone
//#define USE_SUPER_LOG
//#define USE_OFX_GUI
////#define SHOW_THEM_EDITOR //TODO: show ImGui editor for debug. do not stores or load presets. linked to USE_MINIMAL_GUI

//----

// tcp link
// only one of the two
#ifdef LINK_TCP_MASTER_CLIENT_OF
#include "ofxNetwork.h"
#endif

#ifdef LINK_TCP_MASTER_CLIENT_KU
#include "ofxKuNetwork.h"
#endif

// note that threading is not implemented. so ofxColorClient hangs some seconds when connecting retries. 
// after some tries TCP toggle is auto disabled.
// you must enable TCP toggle manually again.

#include <random>

#include "presets/PresetPalette.h"

// demo scenes 
#include "demo/DEMO_Scene.h"
#include "demo/DEMO_SceneSpheres.h"
#include "ofxSCENE-SVG.h"

#include "gui/PreviewPaletteMini.h"

#ifdef USE_OFX_COLOR_BROWSER
#include "ofxColorsBrowser.h"
#endif

#ifdef USE_COLOR_LOVERS
#include "ofxColourLoversHelper.h"
#endif

#include "ofxColorPalette.h"

#include "ColorWheelScheme.h"
#include "ColorWheelSchemes.h"

#include "ofxColorMorph.h"
using namespace ofxColorMorph;
using namespace ofxColorTheory;

#ifdef USE_IMAGE_QUANTIZER
#include "ofxColorQuantizerHelper.h"
#endif

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
#include "ofxSurfing_Files.h"
#include "ofxSurfingImGui.h"
using namespace ofxSurfingHelpers;
using namespace ofxImGuiSurfing;

//--

// gui internal/debug ofxGui
#ifdef USE_OFX_GUI
#include "ofxGui.h"
#endif

#include "ofxImGui.h"
#include "imgui_internal.h" // <-- example uses some imgui internals...
//#include "tools/imgui_stdlib.h" // <-- string helpers

#include "ImGui_PalettesPicker.h"
using namespace ImGui_PalettesPicker;

#include "GradientEngine.h"

//TODO: for a posible future custom multitouch user GUI without ImGui
#ifdef USE_RECTANGLE_INTERFACES
#include "ofxInterface.h"
#include "interface/ButtonPaletteSelector.h"
#include "interface/ButtonExample.h"
#endif

#include "SplashScreen.h"

//----

class ofxColorManager : public ofBaseApp
{
	//-

public:
	ofxColorManager();
	~ofxColorManager();

	void setup();
	void startup();
	void update(ofEventArgs & args);
	void draw(ofEventArgs & args);

public:
	void exit();
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void setImage();

	//--

#ifdef USE_OFX_WINDOWAPP
	ofxWindowApp windowApp;
#endif

	//-

	// OF callbacks

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

private:
	//void draw_PresetName();

	//--

private:
	float dt; // for preview speeds only..
	float fps;

private:
	bool SHOW_EditTheme = false;

	//--

	// app version checker
#ifdef USE_VERSION_CHECKER
public:
	void onNewVersionAvailable(AppUpdateNotifier::Version& version);
private:
	ofxAppUpdateNotifier _appUpdateNotifier;
	bool _isAnUpdateAvailable;
	AppUpdateNotifier::Version _newVersion;
#endif

	//--

	// TCP LINK

private:
	std::string host = "127.0.0.1"; // hardcoded. can't be change on runtime
	int port = 10666; // localhost. hardcoded. can't be change on runtime
	//_port.set("port", 6448, 1000, 50000);
	//ofParameter<std::string> host{ "Host", "127.0.0.1" };
	//ofParameter<int> port{ "Port", 66666, 0, 99999 };

#ifdef LINK_TCP_MASTER_CLIENT_OF
	ofxTCPServer TCP_Sender;
#endif

#ifdef LINK_TCP_MASTER_CLIENT_KU
	ofxKuNetworkTcpClient TCP_Sender;
#endif

	vector <std::string> storeText;
	uint64_t lastCheckLink;
	void updateLink();
	void setupLink();
	void draw_Link(int x = 20, int y = 20);

	//--

private:
	GradientEngine gradientEngine;

	//--

private:
	PreviewPaletteMini miniPreview;

	DEMO_Svg DEMO3_Svg;

	//TODO:
	shared_ptr<ColorWheelScheme> _scheme;

	bool bExportFlag = false; // flag to avoid multiple overflow of calls.. we read on update()

	//TODO:
	enum typeEngine
	{
		TYPE_PRESET = 0,
		TYPE_LOVER,
		TYPE_THEORY,
		TYPE_RANGE,
	};

	//--

	// number of colors. 
	// must be even sometimes to get same size in all palettes

private:
	ofParameter<int> numColors_Engines;
	ofParameter<int> numColors_Range;
	ofParameter<int> numColors_Theory_G1;
	ofParameter<int> numColors_Theory_G2;

	//--

private:
	ofParameter<bool> bAutoHide_Name{ "AutoHide Name", true };

	ofParameter<bool> SHOW_Name{ "Show Extra Name", false };
	ofParameter<bool> SHOW_Advanced{ "ADVANCED", false };
	ofParameter<bool> SHOW_Panels;
	ofParameter<bool> SHOW_LayoutsAdvanced{ "LAYOUTS ", false };
	ofParameter<bool> SHOW_Layouts{ "LAYOUTS", false };
	ofParameter<bool> SHOW_PanelEngines{ "ENGINES", true };
	//shows advanced panels to tweak layout or workflow behaviour

	ofParameter<bool> Lock_DockingLayout{ "LOCK", false };
	ofParameter<bool> bAutoSave_Layout{ "AUTO SAVE", true };
	ofParameterGroup params_LayoutPanelsState{ "LayoutPanels" };

	ofParameter<bool> bPlaySlideShow{ "Play Slide-Show", false };
	ofParameter<float> auto_pilot_Duration{ "Time", 1, 0.1, 5 };
	int auto_pilot_timer;

	//--
	ofParameter<bool> bForceLayoutPosition{ "LAYOUT AUTO-POS", true };

	ofParameter<bool> SHOW_ImGui{ "ImGui", true };

private:
	ofParameter<bool> SHOW_HelpInfo;
	std::string helpInfo; // key commands

private:
	ofParameter<bool> ENABLE_keys{ "KEYS", true };
	bool ENABLE_keys_PRE;

	//-

	bool bTextInputActive = false;

	//-

public:

	void preset_Next();
	void preset_Previous();
	void preset_RandomIndex();

public:

	// public basic params
	ofParameterGroup params_Debug{ "ofxColorManager" };
	ofParameter<bool> SHOW_MINI_Preview;
	ofParameter<bool> SHOW_ALL_GUI;
	ofParameter<bool> SHOW_Scene;

	//--------------------------------------------------------------
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
	ofParameter<bool> SHOW_Engines;
	ofParameter<bool> SHOW_LinkExport;
	ofParameter<bool> SHOW_Presets;
	ofParameter<bool> SHOW_Kit;
	ofParameter<bool> SHOW_Demos;
	ofParameter<bool> SHOW_Picker;
	ofParameter<bool> SHOW_Library;
	ofParameter<bool> SHOW_Range;
	ofParameter<bool> SHOW_Palette;
	ofParameter<bool> SHOW_Editor;
	ofParameter<bool> SHOW_Theory;
	ofParameter<bool> SHOW_Quantizer;
#ifdef MODE_BACKGROUND
	ofParameter<bool> SHOW_BackGround;
#endif
	ofParameter<bool> SHOW_Gradient;
	ofParameter<bool> AutoScroll;

	//-

private:
	// app modes
	ofParameter<int> AppEnginesMode; //  not being much used now..
	ofParameter<std::string> AppMode_name;
#define NUM_APP_MODES 5

	//TODO:
	//using an int slider
	//enum Element { Element_0, Element_1, Element_2, Element_3, Element_4, Element_COUNT };
	//const char* element_names[Element_COUNT] = { "NONE", "THEORY", "RANGE", "LOVERS", "PICTURE" };
	//int current_element = Element_0;

	//--

	// ImGui layouts engine
private:
	const char* ini_to_load = NULL;
	const char* ini_to_save = NULL;
	std::string ini_to_load_Str;
	std::string ini_to_save_Str;

	enum AppLayouts
	{
		APP_DEFAULT = 0,
		APP_PRESETS,
		APP_ENGINES,
		APP_MINIMAL,
		APP_USER,
		APP_LAYOUTS_AMOUNT
	};
	void loadAppLayout(AppLayouts mode);
	void saveAppLayout(AppLayouts mode);
	ofParameter<int> appLayoutIndex{ "App Layout", 0, 0, APP_LAYOUTS_AMOUNT - 1 };
	int appLayoutIndex_PRE = -1;

	//--

	// settings file paths

private:
	std::string path_Global;
	std::string path_ImGui;
	std::string path_Kits;
	std::string path_Kit;
	std::string path_FileExport;
	std::string path_AppState;
	std::string path_Presets;
	//std::string path_Palettes;

	//export
	std::string path_Name_ExportColor;
	ofParameter <std::string> path_Folder_ExportColor_Custom;
	ofParameter <std::string> path_Folder_ExportColor_Data;

	//--

	// colors library layout

private:
	ofParameter<bool> lib_Responsive_ModeGrid;
	ofParameter<bool> lib_Responsive_ModeFit;
	ofParameter<bool> bPagerized;
	ofParameter<int> sizeLibColBox;
	ofParameter<bool> bResponsive_Panels;
	ofParameter<int> sizePaletteBox;

	ofParameter<bool> bFitLayout;

	//-

private:
	ofEventListener listener_LoverName;

	//--

	// gui info display

	std::string name_Theory = "";
	std::string name_Range = "";

	//--

	// current selectors states on user session

private:
	ofParameter<int> last_Index_Theory_PickPalette;
	ofParameter<int> last_Index_Theory{ "Last Theory Index", 0, 0,
		NUM_COLOR_THEORY_TYPES_G1 + NUM_COLOR_THEORY_TYPES_G2 - 1 }; // selected theory algorithm
	ofParameter<int> last_Index_Range{ "Last Range Index", 0, 0, NUM_TYPES_RANGES - 1 }; // selected range algorithm
	ofParameter<int> last_Index_ColorPalette{ "Color", 0, 0, 0 }; // selected color on palette on editing
	ofParameter<int> last_Index_Preset{ "Preset Index", 0, 0, 0 }; // selected preset
	int last_Lib_Index = -1; // last library picked color
	void resetLibraryLayout();

	bool bLast_Index_Theory = true;
	bool bLast_Index_Range = true;

	//--

private:
	// text labels
	//to help workflow of what's the last engine used...Not using now.
	std::string messageInfo;
	ofTrueTypeFont font;
	ofTrueTypeFont fontSmall;
	ofTrueTypeFont fontMedium;
	ofTrueTypeFont fontBig;

	ofParameter<int> last_Index_Type{ "Last Type Index", -1, 0, 3 }; // to help workflow of what's the last engine used...Not using now.
	//0 preset name
	//1 lover name
	//2 theory name
	//3 range name

	//--

	// range
private:
	void setupRange();
	void generate_Range(ofColor col1, ofColor col2);
	bool bRange_Intitiated = false;
	void refresh_Range_AutoUpdate();
	void palette_FromRange(int index);

private:
	ofParameter<ofFloatColor> color_1_Range;
	ofParameter<ofFloatColor> color_2_Range;

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
	ofParameter<bool> bExportByFile;
	ofParameter<bool> bExportByTCP;
	ofParameter<bool> bExportPreset_DefaultPath;
#ifndef USE_SIMPLE_PRESET_PALETTE	
	ofParameter<bool> bModeBundlePreset;
	ofParameter<bool> bModePalettePreset;
#endif
	void setPathPresetWatch();
	void exportPalette();
	void exportKit();

	void loadPresetFile();
	void savePresetFile();

	//--

	// import from https://coolors.co
	void doImportPaletteCoolors(std::string url);
	void doExportPaletteCoolors();

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
	void setupTheory_G1();
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
	DEMO_Scene DEMO1_Bubbles;
	DEMO_SceneSpheres DEMO2_Spheres;

	//--

private:

	std::string last_Lib_NameColor = "";
	int last_ColorPicked_Lib;

	//-	

	ImVec4 borderLineColor = ImVec4(0, 0, 0, 0.65); //  for selected color buttons, toggles and blinking 
	float borderLineWidth = 1.0f;
	float labelPadding = 0.0; // label buttons

	enum Mode
	{
		Mode_Copy,
		Mode_Swap
	};
	int mode = 1;

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

	void build_Palette_SortFlip();
	void build_Palette_SortRandom();
	void build_Palette_SortShift(bool bDirLeft = false);

	void doRandomizeColorPicker();

	// library
	void doRandomizeColorLibrary(bool bPageOnly = false);

	//--

public:
	//--------------------------------------------------------------
	void setFps(float _fps)//for the demo speed only
	{
		fps = _fps;
		dt = 1. / fps;
	}

	//----

	// pointers back 
	// to call refresh and link addons 
	// (quantizer+lovers) with addon palette/color

private:
	std::string myPalette_Name_BACK = "";
	ofColor myColor_BACK;
	vector<ofColor> myPalette_BACK;
	bool bUpdated_Palette_BACK = false;
	bool bUpdated_Color_BACK = false;

	//----

	// API 

	// pointers back to autoupdate our app.
	// ofApp project local variables, registered
	// must be initialized before setup

public:
	void setLinkName(std::string &s);
	void setLinkColorPick(ofColor &c);
	void setLinkColorBg(ofColor &c);
	void setLinkPalette(vector<ofColor> &p);

private:
	std::string *name_TARGET = NULL;
	ofColor *color_TARGET = NULL; // backwards pointer to ofApp picker color
	ofColor *colorBg_TARGET = NULL; // backwards pointer to ofApp background color
	vector<ofColor> *palette_TARGET = NULL; // backwards pointer to ofApp palette

private:
	void refresh_Palette_TARGET(vector<ofColor> &p);

	//----

public:

	// API 

	//-

	// getters

	vector<ofColor> getPalette();
	std::string getPaletteName();
	ofColor getColor(int index = -1);

	// get color from gradient at prc
//--------------------------------------------------------------
	ofColor getColorAtPercent(float control)
	{
		gradientEngine.getColorAtPercent(control);
	}

	//---

	// setters

public:
	void setPickControl(float control); // set the input control for gradient selector. to control the picked color

public:
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
	ofParameter<float> triadSpread;

	////TODO:
	////global modificators to be aplyied to all the palette
	////not implemented yet..
	//ofParameter<int> BRIGHTNESS;
	//ofParameter<int> SATURATION;
	//ofParameter<bool> bLock_palette;

	ofParameter<bool> bRandomPalette;
	ofParameter<bool> bAuto_Build_Palette; // trig last used algo palette on click or change color

private:
	void refresh_Theory_G2();
	void palettes_Resize();

	//----

	// GUI

private:
	ofxImGui::Gui gui;
	ofxImGui::Settings mainSettings = ofxImGui::Settings();
	ImFont* customFont = nullptr;
	ImFont* customFontBig = nullptr;
	ImGuiStyle *style = nullptr;
	ImGuiWindowFlags flagsWindowsLocked;
	ofParameter<int> fontSizeBigParam;
	ofParameter<int> fontSizeParam;
	std::string fontName;
	std::string fontBigName;

	// mouse or key locker
	bool mouseLockedByGui;
	//bool mouseLockedByGui_PRE;
	bool bCheckMouseOverTextInput = false; // flag to return mouse over any gui panel or over text input only!
	bool bCheckMouseOverTextInputLovers = false; // flag to return mouse over any gui panel or over text input only!
	bool bLockAllKeysByGui;
	bool bBlockedKeys;
	//bool bLockMouseByImGui = false;

	bool guiVisible;

	void setupGui();
	bool draw_Gui();

	void gui_Picker();
	void gui_Library();
	void gui_Theory();
	void gui_Range();
	void gui_Panels();
	void gui_EnginesPanel();
	void gui_Demo();
	void gui_LinkExport();
	void gui_Gradient();
	void gui_Presets();
	void gui_Kit();
	void gui_Palette();
	void gui_Editor();
	void gui_LayoutsAdvanced();
	void gui_LayoutsPresets();

	//standalone window not handled by .ini layout
	//but for the app settings
	float widthGuiLayout;
	ofParameter<glm::vec2> positionGuiLayout{ "Gui Layout Position",
	glm::vec2(ofGetWidth() / 2,ofGetHeight() / 2),//center
		glm::vec2(0,0),
		glm::vec2(ofGetWidth(), ofGetHeight())
	};

#ifdef MODE_TEXT_INPUT_WORKAROUND
	void gui_TextInput();
#endif

#ifdef MODE_BACKGROUND
	void gui_Background();
#endif

#ifndef USE_MINIMAL_GUI
	void gui_Advanced();
#endif

	//--

	// colors

private:
	void refresh_Background();
	ofParameter<ofFloatColor> color_BackGround; // main bg color
	ofParameter<bool> color_backGround_SET;
	ofParameter<bool> AutoSet_BackGround_Color;
	ofParameter<bool> color_BackGround_DarkerMode;
	ofParameter<bool> color_BackGround_GradientMode;
	ofParameter<bool> color_BackGround_Lock;
	ofParameter<float> color_BackGround_AmtDarker;
	ofParameter<bool> background_Draw_ENABLE{ "Draw", false };

	void setBackground_ENABLE(bool b);

	bool ENABLE_CALLBACKS_Pickers = true;
	bool ENABLE_CALLBACKS_Engines = true;
	bool ENABLE_CALLBACKS_Range = true;

	void refresh_Pick_ToHSB();
	void refresh_FromPicked();

	bool bFlag_refresh_EnginesFromPalette = false;
	void refresh_EnginesFromPalette();
	void refresh_ColorPickedFromEngines();

	// main picker color
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

	ofParameter<bool> bColor_rHUE;
	ofParameter<bool> bColor_rSAT;
	ofParameter<bool> bColor_rBRG;

	//--

	//TODO: required?
	//TODO: pointer color to get click from button class ?
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

	// editor

private:
	vector<ofColor> palette; //  main user palette

	vector<ofColor> palette_AUX; //  aux palette to tweak saturation and brigthness to all the palette colors
	ofParameter<bool> bTweakPalette{ "TWEAK", false };
	ofParameter<bool> bSortPalette{ "SORTING", false };
	ofParameter<float> hueTweak{ "HUE TWK", 0,-1,1 };
	ofParameter<float> saturationTweak{ "SAT TWK", 0,-1,1 };
	ofParameter<float> brigthnesTweak{ "BRG TWK", 0,-1,1 };
	void doGetPaletteToTweaker();
	void doApplyTweaker();

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
	void refresh_Kits();
	void refresh_Paths();

	int lib_RowSize = 7; // 7 colors per row Pantone lib
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
	void setupDemos();
	void updateDemos();
	void draw_Demos();

private:
	ofParameter<bool> DEMO1_Enable{ "DEMO1 Bubbles", false };
	ofParameter<bool> DEMO1_Auto{ "Auto Trig", false };
	ofParameter<bool> DEMO1_Cam{ "Edit Camera Bubbles", false };
	ofParameter<float> DEMO1_Timer{ "Frequency", 0.5, 0, 1 };
	ofParameter<float> DEMO1_Alpha{ "Alpha 1", 0.8, 0, 1 };
	int Demo_Timer = 0;
	int Demo_Timer_Max = 15000;
	bool bTEST_pause = false;

	ofParameter<bool> DEMO2_Enable{ "DEMO2 Spheres", false };

	//--

	// app settings json/xml
private:
	ofParameterGroup params_AppState; // without callbacks, just to handle settings
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

	// presets

private:
	PresetPalette PRESET_Temp;

private:
	// default name 
	std::string PRESET_Name = "_emptyPreset";
	std::string PRESET_Name_Gradient = "_";

	//--

private:

	void preset_Save(std::string p, bool absolutePath = false);
	void preset_Load(std::string p, bool absolutePath = false);

	//-

private:

	// new preset state mode
	ofParameter<bool> MODE_NewPreset{ "NEW PRESET", false };

	bool MODE_ReadyToUpdate = false; // when a preset is loaded will allow update/overwrite save

	std::string textInput_New = "name..";

	////TODO:
	////handle text input focus to disable key commands..
	//bool focus_1;
	//int has_focus = 0;

	//--

	// preset files

	bool preset_RefreshFiles(bool bForeLoadFirst = true); // read folder files
	void refresh_FilesSorting(std::string name); // after saving new preset, refresh files and select the just saved preset
	std::vector<std::string> files_Names;
	
	std::vector<std::string> files_KitNames;
	ofParameter<int> last_Index_Kit{ "Kit", -1, 0, 0 };

	std::vector<ofFile> files;
	vector<vector<ofColor>> palettesKit;
	vector<PresetData> kit;
	bool bAlertFileOverWrite = false;
	std::string textInput_temp = "type name";

	//----

private:
	ofParameter<bool> SHOW_MenuBar{ "MENU BAR", false };
	bool SHOW_About = false;
	void gui_MenuBar();
	void gui_About(bool* p_open);

	//--

	// demo to draw into about panel
private:
	ofTexture tex;
	ofFbo fbo; // mini preview
	ofFbo fboBig; // fullscreen fbo
	float wAboutDemo = 250;
	float tweenD = 1;
	void refresh_DemoFboAbout();
	void draw_DemoFbo();

	//----

	//TODO:
	// extra ImGui methods

	//TODO:
	//trying to hide the 'x' button of panels when docking and to lock the resizing of panels
//private:
	//--------------------------------------------------------------
	//inline void hideDockWindow(string name)
	//{
	//	char *cstr = &name[0];
	//
	//	//https://stackoverflow.com/questions/7352099/stdstring-to-char/7352131
	//	//const char *cstr = name.c_str();
	//
	//	bool m_show_tabs = false;
	//
	//	if (ImGui::IsWindowDocked()) {
	//		auto* wnd = ImGui::FindWindowByName(cstr);
	//		if (wnd) {
	//			ImGuiDockNode* node = wnd->DockNode;
	//			if (node)// && (!m_show_tabs && !node->IsHiddenTabBar()) || (m_show_tabs && node->IsHiddenTabBar())) 
	//			//if (node && (!m_show_tabs && !node->IsHiddenTabBar()) || (m_show_tabs && node->IsHiddenTabBar())) 
	//			{
	//				node->WantHiddenTabBarToggle = true;
	//			}
	//		}
	//	}
	//}

private:
	//TODO:
	// Helper to display a little (?) mark which shows a tooltip when hovered.
	// In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.txt)
	//--------------------------------------------------------------
	static void HelpMarker(const char* desc)
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}


	////TODO:
	////
	//// toggle exclusive selector fron index int
	//// check other widgets
	////--------------------------------------------------------------
	//inline bool AddSelectors(ofParameter<int>& index, vector<std::string> names, float w = -1, float h = -1)
	//{
	//	const int sz = index.getMax();
	//	static vector<bool> toggles;
	//	toggles.resize(sz);
	//	for (int i = 0; i < sz; i++)
	//	{
	//		if (i == index.get()) toggles[i] = true;
	//		else toggles[i] = false;
	//	}

	//	float _spcx = ImGui::GetStyle().ItemSpacing.x;
	//	float _spcy = ImGui::GetStyle().ItemSpacing.y;
	//	float _w100 = ImGui::GetContentRegionAvail().x;
	//	float _h100 = ImGui::GetContentRegionAvail().y;
	//	float _w99 = _w100 - _spcx;
	//	float _w50 = _w99 / 2;
	//	float _h = BUTTON_BIG_HEIGHT / 2;
	//	
	//	ImVec2 bb{100, 20};

	//	for (int i = 0; i < sz; i++)
	//	{
	//		//ImGui::Checkbox(names[i].c_str(), &toggles[i]);
	//	}

	//}

	ofParameter<bool> b0{ "DEFAULT", false };
	ofParameter<bool> b1{ "PRESETS", false };
	ofParameter<bool> b2{ "ENGINES", false };
	ofParameter<bool> b3{ "MINIMAL", false };
	ofParameter<bool> b4{ "USER", false };

	void Changed_LayoutPanels(ofAbstractParameter &e);
	ofParameterGroup params_LayoutSPanel{ "LAYOUTS PANEL" };

	//--------------------------------------------------------------
	std::string getLayoutName(AppLayouts mode) {
		std::string s = "";
		//switch (appLayoutIndex)
		switch (mode)
		{
		case APP_DEFAULT: s = path_ImGui + "imgui_DEFAULT.ini"; break;
		case APP_PRESETS: s = path_ImGui + "imgui_PRESETS.ini"; break;
		case APP_ENGINES: s = path_ImGui + "imgui_ENGINES.ini"; break;
		case APP_MINIMAL: s = path_ImGui + "imgui_MINIMAL.ini"; break;
		case APP_USER: s = path_ImGui + "imgui_USER.ini"; break;
		default:break;
		}
		return s;
	}

	//--

	// color utils

	//from colourLovers
	static string hexToWeb(ofColor col) {
		return "#" + ofToHex(col.r) + ofToHex(col.g) + ofToHex(col.b);
	}

	SplashScreen splash;

	void buildHelpInfo();

	ofRectangle rectangle_Central_MAX;
	ofRectangle rectangle_Central;
	ofRectangle rectangle_Central_Transposed;
	ofParameter<bool> bDebugRectCentral{ "Rectangle Central", false };

	//---

//TODO:
// ASE exporter/importer
#ifdef USE_ASE_MODULE

	void createAse() {
		ofLogNotice(__FUNCTION__) << "----------------- EXPORT ASE -----------------" << endl;

		ASE_COLORTYPE _ASE_COLORTYPE = ASE_COLORTYPE_RGB;

		float _col0[4] = { 1,0,0,1 };
		ASE_COLOR _ASE_COLOR0;
		_ASE_COLOR0.name = "myColor0";
		_ASE_COLOR0.type = ASE_COLORTYPE_RGB;
		_ASE_COLOR0.col[0] = _col0[0];
		_ASE_COLOR0.col[1] = _col0[1];
		_ASE_COLOR0.col[2] = _col0[2];

		float _col1[4] = { 0,1,0,1 };
		ASE_COLOR _ASE_COLOR1;
		_ASE_COLOR1.name = "myColor1";
		_ASE_COLOR1.type = ASE_COLORTYPE_RGB;
		_ASE_COLOR1.col[0] = _col1[0];
		_ASE_COLOR1.col[1] = _col1[1];
		_ASE_COLOR1.col[2] = _col1[2];

		float _col2[4] = { 0,0,1,1 };
		ASE_COLOR _ASE_COLOR2;
		_ASE_COLOR2.name = "myColor2";
		_ASE_COLOR2.type = ASE_COLORTYPE_RGB;
		_ASE_COLOR2.col[0] = _col2[0];
		_ASE_COLOR2.col[1] = _col2[1];
		_ASE_COLOR2.col[2] = _col2[2];

		ASE_COLOR myColors[3];

		ASE_GROUP _ASE_GROUP;
		_ASE_GROUP.numColors = 3;
		_ASE_GROUP.name = "myGroup";
		_ASE_GROUP.colors = &_ASE_COLOR0;

		ASE_FILE _ASE_FILE;
		_ASE_FILE.numGroups = 1;
		_ASE_FILE.version[0] = 1;
		_ASE_FILE.version[1] = 1;
		_ASE_FILE.groups = &_ASE_GROUP;

		ASE_ERRORTYPE error;
		FILE *f; //  = fopen(ofToDataPath("myASE"));

		//error = ase_writeAseFile(&_ASE_FILE, f);
		//ase_writeAseFile(ASE_FILE *ase, FILE *f);
		//https://github.com/JamesHovet/ofxASE
}
#endif

	//----

	//TODO:
	//extra THEORY COLOR helpers
	//https://github.com/borg/ofxColourTheory/blob/master/src/ofxColourTheory.h
	static void invert(ofColor &col) {
		col.set(255 - col.r, 255 - col.g, 255 - col.b);
	}
	static float luminance(ofColor col) {
		float tot = 255 * 0.299f + 255 * 0.587f + 255 * 0.114f;
		return (col.r * 0.299f + col.g * 0.587f + col.b * 0.114f) / tot;
	}
	/**
	* Inverts all colors in the list.
	*
	* @return itself
	*/
	static void invert(vector<ofColor> &colors) {
		for (int i = 0; i < colors.size(); i++) {
			invert(colors[i]);
		}
	}
	static  vector<ofVec2f> getRYB_WHEEL() {
		vector<ofVec2f> RYB_WHEEL;
		RYB_WHEEL.push_back(ofVec2f(0, 0));
		RYB_WHEEL.push_back(ofVec2f(15, 8));
		RYB_WHEEL.push_back(ofVec2f(30, 17));
		RYB_WHEEL.push_back(ofVec2f(45, 26));
		RYB_WHEEL.push_back(ofVec2f(60, 34));
		RYB_WHEEL.push_back(ofVec2f(75, 41));
		RYB_WHEEL.push_back(ofVec2f(90, 48));
		RYB_WHEEL.push_back(ofVec2f(105, 54));
		RYB_WHEEL.push_back(ofVec2f(120, 60));
		RYB_WHEEL.push_back(ofVec2f(135, 81));
		RYB_WHEEL.push_back(ofVec2f(150, 103));
		RYB_WHEEL.push_back(ofVec2f(165, 123));
		RYB_WHEEL.push_back(ofVec2f(180, 138));
		RYB_WHEEL.push_back(ofVec2f(195, 155));
		RYB_WHEEL.push_back(ofVec2f(210, 171));
		RYB_WHEEL.push_back(ofVec2f(225, 187));
		RYB_WHEEL.push_back(ofVec2f(240, 204));
		RYB_WHEEL.push_back(ofVec2f(255, 219));
		RYB_WHEEL.push_back(ofVec2f(270, 234));
		RYB_WHEEL.push_back(ofVec2f(285, 251));
		RYB_WHEEL.push_back(ofVec2f(300, 267));
		RYB_WHEEL.push_back(ofVec2f(315, 282));
		RYB_WHEEL.push_back(ofVec2f(330, 298));
		RYB_WHEEL.push_back(ofVec2f(345, 329));
		RYB_WHEEL.push_back(ofVec2f(360, 0));

		return RYB_WHEEL;
	}
	static ofColor rotateRYB(ofColor col, int theta) {

		float h = (float)col.getHue() / 255.0 * 360;
		vector<float> hsb;
		hsb.push_back((float)col.getHue() / 255.0);
		hsb.push_back((float)col.getSaturation() / 255.0);
		hsb.push_back((float)col.getBrightness() / 255.0);
		theta %= 360;

		float resultHue = 0;

		vector<ofVec2f> RYB_WHEEL = getRYB_WHEEL();

		for (int i = 0; i < RYB_WHEEL.size() - 1; i++) {
			ofVec2f p = RYB_WHEEL[i];
			ofVec2f q = RYB_WHEEL[i + 1];
			if (q.y < p.y) {
				q.y += 360;
			}
			if (p.y <= h && h <= q.y) {
				resultHue = p.x + (q.x - p.x) * (h - p.y) / (q.y - p.y);
				break;
			}
		}

		//fmod = %, ie remainder

		// And the user-given angle (e.g. complement).
		resultHue = fmod((resultHue + theta), 360);

		// For the given angle, find out what hue is
		// located there on the artistic color wheel.
		for (int i = 0; i < RYB_WHEEL.size() - 1; i++) {
			ofVec2f p = RYB_WHEEL[i];
			ofVec2f q = RYB_WHEEL[i + 1];
			if (q.y < p.y) {
				q.y += 360;
			}
			if (p.x <= resultHue && resultHue <= q.x) {
				h = p.y + (q.y - p.y) * (resultHue - p.x) / (q.x - p.x);
				break;
			}
		}

		// col.setHsb(<#float hue#>, <#float saturation#>, <#float brightness#>)
		hsb[0] = fmod(h, 360) / 360.0f;

		ofColor newCol;
		newCol.setHsb(hsb[0] * 255, hsb[1] * 255, hsb[2] * 255);
		return newCol;
	}
	static ofColor getComplement(ofColor col) {
		return rotateRYB(col, 180);
	}

};