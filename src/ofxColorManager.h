
#pragma once
#include "ofMain.h"

/*

TODO:

+ simplify num colors calls..
+ fix auto build palette wf

*/


//----------

//	OPTIONAL

//#define USE_RECTANGLE_INTERFACES // should be nice to completely disable! to disable ofxInterface

//	modules
#define USE_COLOR_LOVERS
#define USE_IMAGE_QUANTIZER
#define USE_OFX_COLOR_BROWSER
#define USE_EXTRA_LIBRARIES
//#define USE_SUPER_LOG

//	layout
#define MAX_PALETTE_COLORS 10
#define INCLUDE_IMGUI_CUSTOM_THEME_AND_FONT
#define BUTTON_BIG_HEIGHT 50
#define BUTTON_COLOR_SIZE 40
#define COLOR_STRIP_COLOR_HEIGHT 15
#define PANEL_WIDGETS_WIDTH 200
//#define INCL_LAYOUT
//#define USE_OFX_GUI

#define AUTO_DRAW_CALLBACK

//------


#ifdef USE_SUPER_LOG
#include "ofxSuperLog.h"
#endif

#include "ofxColorGradient.h"
#include "ofxColorPalette.h"

#ifdef USE_OFX_COLOR_BROWSER
#include "ofxColorsBrowser.h"
#endif

#include "ofxInteractiveRect.h" // engine to move the gui. TODO: add resize by mouse too.

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

//ofxCurvesTool
#include "ofxCurvesTool.h"
#include "ofxSimpleSlider.h"

#ifdef USE_RECTANGLE_INTERFACES
#include "ofxInterface.h"
#include "interface/ButtonPaletteSelector.h"
#include "interface/ButtonExample.h"
#endif

#include "ofxUndoSimple.h"
#include "ofxMouseRuler.h"

#ifdef INCL_LAYOUT
#include "ofxGuiPanelsLayout.h"
#endif

#ifdef USE_OFX_GUI
#include "ofxGui.h"
#endif

#include "ofxImGui.h"
//#include "imgui_demo.cpp"

#include "ImGui_PalettesPicker.h"
using namespace ImGui_PalettesPicker;

#include "presets/PresetManager.h"
#include "presets/PresetPalette.h"

#include "demo/DEMO_palette.h"

#include "ofxSurfingHelpers.h"
#include "ofxSurfing_ImGui.h"
using namespace ofxSurfingHelpers;

#include "ofxSerializer.h"

//#include "ofxCosineGradient.h"

//--

class ofxColorManager : public ofBaseApp
{
	//--

public:
	ofxColorManager();
	~ofxColorManager();

	void setup();
	void startup();

#ifndef AUTO_DRAW_CALLBACK
	void draw();
#endif
#ifdef AUTO_DRAW_CALLBACK
	void draw(ofEventArgs & args);
	//TODO: to allow auto update we must set the order priority layer/drawing
#endif

	void update(ofEventArgs & args);
	//void update();

	void exit();
	void windowResized(int w, int h);

	//--

private:
	void draw_Curve();
	void update_Curve();

	void draw_Info();

	//--

private:
	float dt;
	float fps;

	//-
	
	std::string infoHelp;//key commands

private:
	ofParameter<bool> SHOW_ColourLovers;
	ofParameter<bool> SHOW_ColourLovers_searcher;
	ofParameter<bool> SHOW_AlgoPalettes;
	ofParameter<bool> SHOW_BrowserColors;
	ofParameter<bool> SHOW_Gradient;
	ofParameter<bool> SHOW_Curve;
	ofParameter<bool> SHOW_ALL_GUI;
	ofParameter<bool> SHOW_GUI_MINI;
	ofParameter<bool> SHOW_debugText;
	ofParameter<bool> SHOW_Panels;
	ofParameter<bool> SHOW_Presets;
	ofParameter<bool> SHOW_Kit;
	ofParameter<bool> AutoScroll;
	ofParameter<bool> SHOW_Demo;
	ofParameter<bool> SHOW_BackGround;
	ofParameter<bool> SHOW_Picker;
	ofParameter<bool> SHOW_Library;
	ofParameter<bool> SHOW_Range;
	ofParameter<bool> SHOW_UserPalette;
	ofParameter<bool> SHOW_Theory;
	ofParameter<bool> SHOW_Quantizer;
	//ofParameter<bool> SHOW_CosineGradient;

	ofParameter<int> AppMode;
	ofParameter<std::string> AppMode_name;

	enum Element { Element_0, Element_1, Element_2, Element_3, Element_4, Element_5, Element_COUNT };
	const char* element_names[Element_COUNT] = { "PRESETS", "THEORY", "RANGE", "LOVERS", "PICTURE", "GRADIENT" };
	int current_element = Element_0;

	//-

	bool bTextInputActive = false;

private:
	std::string path_Global;
	std::string path_Kits;
	std::string path_Folder_Curves;
	std::string path_Layout;
	std::string path_Curves_Lut;
	std::string path_Curve_Preset;
	std::string path_Folder_Color;
	std::string path_Colors;
	std::string path_AppState;
	std::string path_Presets;
	std::string path_Palettes;

	//-

public:
	ofParameter<bool> lib_Responsive_Mode;
	ofParameter<bool> bPagerized;
	ofParameter<int> sizeLibColBox;

public:
	ofParameter<bool> bResponsive_Presets;
	ofParameter<int> sizePaletteBox;

public:
	void dragEvent(ofDragInfo dragInfo);

private:
	ofEventListener listener_LoverName;

	//--

	bool ENABLE_keys_PRE;

	//--

private:
	ofxInteractiveRect rPreview = { "_Curve_Gui" };
	ofParameter<bool> MODE_Editor;
	ofParameter<bool> SHOW_Editor;
	ofColor colCurveTest;
	ofParameter<glm::vec2> pos_CurveEditor;

public:
	//font
	std::string messageInfo;
	ofTrueTypeFont fontSmall;
	ofTrueTypeFont fontMedium;
	ofTrueTypeFont fontBig;

	bool txt_lineActive[4];

	ofTrueTypeFont font;

	//-

	//ofxColorMorph

public:
	void setup_Range();
	void generate_Range(ofColor col1, ofColor col2);
	bool bRange_Intitiated = false;
	void refresh_Range_AutoUpdate();

private:
	ofParameter<ofColor> color_1_Range;
	ofParameter<ofColor> color_2_Range;
	std::vector<ofColor> palette_Range;
	std::vector<std::string> rangeTypes_names;
	ofParameter<bool> autoGenerate_Range;
	ofParameter<bool> autoPick_RangeColor1;
	ofParameter<bool> autoPick_RangeColor2;
	ofParameter<bool> bGetPaletteFromRange;
	ofParameter<int> numColors_Range;
	ofParameterGroup params_Ranges;
#define NUM_TYPES_RANGES 12
	ofParameter<bool> rangTypes[NUM_TYPES_RANGES];
	void Changed_ColorRange(ofAbstractParameter &e);

	//extra
	ofParameter<bool> algoTypes[7];
	ofParameterGroup params_algoTypes;

	//-

public:

	//live reload colors file
	void saveColors();

private:
	//TODO
	//BUG: trying to avoid bug moving mouse..
	//void mouseEvent(ofxMacMouseEventArg &arg) {
	//    ofLogNotice() << "global mouse position: " << arg.x << ", " << arg.y;
	//}

	bool ENABLE_keys = false;

	// colorQuantizer
#ifdef USE_IMAGE_QUANTIZER
	ofxColorQuantizerHelper colorQuantizer;
#endif

	//-
	
	ofParameter<int> numColors_TheoryEngines;

private:
	// color theory
#define NUM_COLOR_THEORY_TYPES 8
	ofParameter<bool> theoryTypes[NUM_COLOR_THEORY_TYPES];
	shared_ptr<ColorWheelScheme> scheme;
	vector<ofColor> colorsTheory[NUM_COLOR_THEORY_TYPES];
	ofParameterGroup params_ColorTheory;
	ofParameter<ofColor> color_TheoryBase;
	ofParameter<int> colorScheme;
	ofParameter<std::string> colorSchemeName;
	ofParameter<int> numColors_Theory;
	ofParameter<bool> bAuto_TheoryFromPicker;

	void Changed_ColorTheory(ofAbstractParameter &e);
	void refresh_Interface();

	void Changed_ParamsPalette(ofAbstractParameter &e);
	ofParameterGroup params_Palette;

	//-

private:
	//user palette
	ofParameter<int> boxSizeUser;
	ofParameter<int> boxRowsUser;
	ofParameter<float> scale_ColPalette;
	ofParameter<bool> bFlipUserPalette;

	ofParameter<float> scale_ColRange;

	//-

private:
	// ColorWheelSchemes
	void setup_Theory();
	void update_Theory();

	//-

//#ifdef USE_OFX_GUI
////ofxPanel panel;
//#endif

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

	// ofxGuiPanelsLayout
private:

#ifdef INCL_LAYOUT
	ofxGuiPanelsLayout panels;
#endif

	ofParameter<bool> SHOW_GuiInternal{ "GUI INTERNAL", false };
	ofParameter<bool> SHOW_ImGui{ "ImGui", true };
	ofParameter<bool> ENABLE_HelpInfo;// { "HELP INFO", false };

	//--

	// DEMO

	DEMO_palette myDEMO;

	//--

	// presets

	PresetPalette palette_TEMP;

	//TODO:
	//PresetManager myPresetManager;

	//TODO
	//BUG: should create a default preset because if myPreset is not detected it crashes
	//default preset
	std::string PRESET_name = "_emptyPreset";
	std::string PRESET_curveName = "curve01";

	//-

	// gui info display
	int last_Index_Theory = -1;
	int last_Index_Range = -1;
	std::string theory_Name = "";
	std::string range_Name = "";

	//-

	ofParameter<int> last_Theory_PickPalette;

	std::string last_Lib_NameColor = "";
	int last_ColorPicked_Lib;

	int last_ColorPicked_Palette;

	ImVec4 color_Pick{ 1,1,1,0.5 };
	float linew_Pick = 2.0;

	//-

	// colour lovers
private:

	void colourLovers_drawPreview();
#ifdef USE_COLOR_LOVERS
	ofxColourLoversHelper colourLoversHelper;
#endif

	std::string myPalette_Name = "";
	ofColor myColor;
	vector<ofColor> myPalette;

	bool bUpdated_Palette_BACK = false;
	bool bUpdated_Color_BACK = false;

	//--

	// TODO: 
	//TESTING
	ofParameter<bool> preview{ "Preview", false };
	bool show_another_window;

public:
	//--------------------------------------------------------------
	void setFps(float _fps)
	{
		fps = _fps;
		dt = 1. / fps;
	}

	void setColor_TARGET(ofColor &c);//backwards pointer ofApp color
	ofColor *color_TARGET;//backwards pointer ofApp color

	//-----------------------------------------------------------

	// API
public:
	vector<ofColor> getPalette();
	ofColor getColorAtPercent(float control);
	void setControl(float control);
	void setVisible(bool b);
	void setToggleVisible();
	void setVisible_GUI_MINI(bool b);
	void setVisible_debugText(bool b);

public:
	void draw_Mini();
	void draw_GradientPreview(glm::vec2 pos, bool horizontal);

private:
	void disableListeners();
	void enableListeners();

	//-----------------------------------------------------------

private:
	ofxMouseRuler mouseRuler;

	//--

	// user palette & control

private:
	ofParameter<bool> bPaletteEdit;
	ofParameter<bool> bRandomColor;
	ofParameter<bool> bAddColor;
	ofParameter<bool> bRemoveColor;
	ofParameter<bool> bClearPalette;
	int palette_colorSelected = -1;

	//--

	// parameters

private:
	ofParameterGroup params_data;
	ofParameterGroup params_color;
	ofParameterGroup params_Theory;
	ofParameterGroup params_curve;
	ofParameterGroup params_control;

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

	ofParameter<int> numColors_Alg;
	//number of colors. must be even sometimes to get same size in all palettes

	ofParameter<int> BRIGHTNESS;
	ofParameter<int> SATURATION;

	ofParameter<bool> bRandomPalette;
	ofParameter<bool> bAuto_TheoryToPalette;//trig last used algo palette on click or change color
	//ofParameter<bool> bLock_palette;

	//TODO
private:
	void palettes_setPosition(glm::vec2 pos)
	{
		palettes_x = pos.x;
		palettes_y = pos.y;
	}

private:
	void refresh_TheoryEngine();
	void palettes_resize();

	//--

	// GUI

private:
	ofxImGui::Gui gui;

	bool guiVisible;

	bool mouseOverGui;
	bool mouseOverGui_PRE;

	float widgetFactor = 0.9;
	ofxImGui::Settings mainSettings = ofxImGui::Settings();

	bool bCheckMouseOverTextInput = true;//to return mouse over any gui panel or over text input only!

	bool draw_Gui();
	void gui_Palette();
	void gui_Theory();
	void gui_Picker();
	void gui_Library();
	void gui_Background();
	void gui_Curve();
	void gui_Range();
	void gui_Presets();
	void gui_Panels();
	void gui_Demo();

	void refresh_Gui_Layout();

	//--

	// COLORS

private:
	ofParameter<ofFloatColor> color_BackGround;//main bg color
	ofParameter<bool> color_backGround_SET;
	ofParameter<bool> color_BackGround_AutoSet;
	ofParameter<bool> color_BackGround_Darker;
	ofParameter<float> color_BackGround_Darkness;
	//float backgroundDarkness_PRE;

	ofParameter<bool> background_Draw_ENABLE{ "Draw", false };

	void setBackground_ENABLE(bool b);

	bool ENABLE_Callbacks_cPickers = true;

	void refresh_Picked_toHSB();
	void refresh_FromPicked();

	// main color
	ofParameter<ofFloatColor> color_Picked;

	ofxUndoSimple<ofFloatColor> color_Undo;

	ofRectangle r_color_picked;

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


	// color clicked comes from algo palette colors
	//TODO: pointer color to get click from button class
	ofFloatColor color_Clicked2;
	ofFloatColor color_Clicked2_PRE;

	ofFloatColor color_BACK;
	ofFloatColor color_BACK_PRE;

	//-

private:
	ofParameter<ofFloatColor> color_Clicked;

	void Changed_ColorPicked(ofFloatColor &color);
	void Changed_ColorClicked(ofFloatColor &color);

	void build_GradientPalette();

	void refresh_Picker_Touched();

	//--

	// user palette of colors

private:
	vector<ofColor> palette;// main user palette

	void palette_addColor(ofColor c);
	void palette_removeColorLast();
	void palette_removeColor(int c);
	void palette_clear();
	void palette_addColor_toInterface(ofColor c);
	void palette_rearrenge();//resize boxes when adding removing colors to user palette
	void palette_touchedColor(std::string name);
	void palette_FromTheory(int p);
	void palette_load_FromColourLovers();

	//----

	// libs library palette
	// using colorBrowser

private:

#ifdef USE_OFX_COLOR_BROWSER
	ofxColorsBrowser colorBrowser;
#endif

	vector<ofColor> palette_Lib_Cols;
	vector<std::string> palette_Lib_Names;

	//-

	ofEventListener listener_Library;
	ofEventListener listener_ModeSorting;

	void refresh_Libs();

	int lib_RowSize = 7;//7 colors per row Pantone lib
	int lib_Page_NumColors;
	int lib_TotalColors;
	int lib_Page_Max;

	int last_Lib_Index = -1;

	//rows per page
	ofParameter<int> lib_NumRows{ "Rows Amnt" , 10, 5, 10 * 5 };
	ofParameter<int> lib_Page_Index{ "Page" , 0, 0, lib_Page_Max };
	ofParameter<bool>lib_CardsMode{ "Mode Cards", false };
	ofParameter<int> lib_MaxColumns{ "Columns Max", 7, 1, 7 * 6 };
	ofParameter<float> scale_LibCol{ "Scale", 1, 0.5, 1.5 };

	//-

	//TODO:
private:
	int NUM_PALETTES = 7;//without random
	int NUM_CT_PALETTES = 8;

	//int NUM_TOTAL_PALETTES = 16;//TODO
	int NUM_TOTAL_PALETTES = NUM_PALETTES + NUM_CT_PALETTES;//TODO //without random

	int SELECTED_palette = -1;
	int SELECTED_palette_PRE = -1;//to check if changed on update() loop
	int SELECTED_palette_LAST = 3;//default last palette type triggered. compBrg by default

	//----

	// gradient

private:
	ofxColorGradient<ofColor> gradient;//unmodified gradient with curveTool
	ofParameter<bool> gradient_HardMode;//stepped
	void draw_Gradient();

	//--

	// curves
private:

	ofxCurvesTool curvesTool;

	void setup_CurveTool();
	void update_CurveTool();
	void draw_CurveTools();

	int cAmt = 256;
	std::string curveTool_name = "curves.yml";
	ofImage curve_img_gradient;
	ofParameter<float> curve_Ctrl_In;
	ofParameter<float> curve_Ctrl_Out;
	ofParameter<bool> bResetCurve;
	ofParameter<float> curve_Gradient_TEST_Prc;
	int curve_Index = 0;

	ofxSimpleSlider curve_Slider_InExp;
	ofxSimpleSlider curve_Slider_OutPick;

	//--

	// test curve
private:

	ofParameter<bool> TEST_Mode{ "Enable Test", false };
	float TEST_Speed = .75;
	bool TEST_LFO_Mode = true;
	int TEST_maxFrames = 300;//slowest period
	bool TEST_toBackground = true;
	float framePrc;

	//--

	// demo
private:

	ofParameter<bool> DEMO_Test{ "ENABLE DEMO", false };
	ofParameter<bool> DEMO_Auto{ "AUTO", false };
	ofParameter<bool> DEMO_Cam{ "CAM", false };
	ofParameter<float> DEMO_Timer{ "SPEED", 0.5, 0, 1 };
	ofParameter<float> DEMO_Alpha{ "ALPHA", 0.8, 0, 1 };
	int Demo_Timer = 0;
	int Demo_Timer_Max = 15000;

	////TODO: make pauses between any test trig..
	bool bTEST_pause = false;
	//int TEST_pauseLong = 2000;
	//int TEST_pauseChrono = 0;

	ofParameter<float> curve_Gradient_InExp{ "Gradient Exp", 0, 0, 1 };
	ofParameter<float> curve_Gradient_OutPick{ "Gradient Pick", 0, 0, 1 };

	//--

	// preset manager
private:

	void preset_save(std::string p);
	void preset_load(std::string p);

	//TODO: this is to save an standalone palette
	//void palette_save(std::string p);
	//void palette_load(std::string p);

private:
	ofParameter<bool> bNewPreset{ "New Preset", false };
	bool MODE_NewPreset = false;
	std::string textInput_New = "new preset";
	bool focus_1;
	int has_focus = 0;
	char tab[128];
	void refresh_RearrengeFiles(std::string name);

	//-

	// preset files

	void preset_refreshFiles();
	std::vector<std::string> files_Names;
	std::vector<ofFile> files;
	std::string textInput_temp = "type name";
	ofParameter<int> preset_Index{ "Preset Index", 0, 0, 0 };
	//int preset_Index = 0;

	vector<vector<ofColor>> palettesKit;
	vector<PaletteData> kit;

	//--

	// app settings xml
private:
	ofParameterGroup params_AppState;
	ofParameterGroup params_Panels{ "PANELS" };
	ofParameterGroup params_Background{ "BACKGROUND" };
	ofParameterGroup params_Demo{ "DEMO" };
	ofParameterGroup params_Picker{ "PICKER" };
	ofParameterGroup params_Library{ "LIBRARY" };
	ofParameterGroup params_Palette2{ "PALETTE" };
	ofParameterGroup params_Curve{ "CURVE" };

	//--

private:
	// layout
	int box_size_user;//user palette colors
	int box_size;//palettes colors
	int pad; //global mini pad
	int c_grad_x, c_grad_y, c_grad_w, c_grad_h;
	int curveTool_x;
	int curveTool_y;
	int curveTool_w;
	int curveTool_h;
	int image_curvedGradient_x;
	int image_curvedGradient_y;
	int image_curvedGradient_w;
	int image_curvedGradient_h;
	int grad_x;
	int grad_y;
	int grad_w;
	int grad_h;
	int slider_In_x;
	int slider_In_y;
	int slider_In_w;
	int slider_In_h;
	int slider_Out_x;
	int slider_Out_y;
	int slider_Out_w;
	int slider_Out_h;
	int palettes_x;
	int palettes_y;
	int palette_x;
	int palette_y;
	int currColor_x;
	int currColor_y;
	int colorPick_x, colorPick_y, colorPick_w, colorPick_h;
	int color_x, color_y, color_w, color_h;
	glm::vec2 colorBrowserPos;

	//--

	// listeners

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

	//--

	////cosine gradient
	//
	//ofxCosineGradient mCosineGradient;
	//ofxImGui::Gui mGui;
	//ofParameterGroup cosineGradient_params;
	//ofParameter<glm::vec3> mBias;
	//ofParameter<glm::vec3> mAmplitude;
	//ofParameter<glm::vec3> mFrequency;
	//ofParameter<glm::vec3> mPhase;
	//
	//glm::vec3 mB;
	//glm::vec3 mA;
	//glm::vec3 mF;
	//glm::vec3 mP;
	//
	//void cosineGradient_update()
	//{
	//    mB = mBias.get();
	//    mA = mAmplitude.get();
	//    mF = mFrequency.get();
	//    mP = mPhase.get();
	//
	//    mCosineGradient.setBias(mB);
	//    mCosineGradient.setAmplitude(mA);
	//    mCosineGradient.setFrequency(mF);
	//    mCosineGradient.setPhase(mP);
	//    mCosineGradient.update();
	//
	//    ////cout << "cosineGradient_update" << endl;
	//    //mCosineGradient.setBias(mBias.get());
	//    //mCosineGradient.setAmplitude(mAmplitude.get());
	//    //mCosineGradient.setFrequency(mFrequency.get());
	//    //mCosineGradient.setPhase(mPhase.get());
	//    //mCosineGradient.update();
	//}
	//void gui_imGui_CosineGradient();

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
};
