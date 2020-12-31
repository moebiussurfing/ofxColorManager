
#pragma once
#include "ofMain.h"

//--

#define INCL_LAYOUT
#define USE_RECTANGLE_INTERFACES
#define USE_COLOR_LOVERS
#define USE_IMAGE_QUANTIZER
#define INCLUDE_IMGUI_CUSTOM_THEME_AND_FONT

//--

#include "ofxColorGradient.h"
#include "ofxColorPalette.h"
#include "ofxColorsBrowser.h"

//#include "ofxScaleDragRect.h"
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
#include "ButtonPaletteSelector.h"
#include "ButtonExample.h"
#endif

#include "ofxUndoSimple.h"
#include "ofxMouseRuler.h"

#ifdef INCL_LAYOUT
#include "ofxGuiPanelsLayout.h"
#endif

#include "ofxGui.h"

#include "ofxImGui.h"

#include "PresetManager.h"
#include "PresetPalette.h"
#include "DEMO_palette.h"

//#include "ofxCosineGradient.h"

#include "ofxSurfingHelpers.h"
#include "ofxSurfing_ImGui.h"

#include "ofxSerializer.h"

//--

class ofxColorManager : public ofBaseApp
{
	//-
private:
	ofxInteractiveRect rPreview = { "_Curve_Gui" };
	ofParameter<bool> MODE_Editor;
	ofParameter<bool> SHOW_Editor;

	//ofxColorMorph
public:
	void generateRange(ofColor col1, ofColor col2);
private:
	ofParameter<ofColor> c1_Rng;
	ofParameter<ofColor> c2_Rng;
	ofColor color;
	ofFloatColor guiCol1;
	ofFloatColor guiCol2;
	std::vector<ofColor> paletteRange;
	std::vector<std::string> types;
	bool bRefreshMorph;
	ofParameter<bool> bRangeAutoGenerate;
	ofParameter<bool> autoPickColor1;
	ofParameter<bool> autoPickColor2;
	ofParameter<bool> bGetPaletteFromRange;
	ofParameter<int> numColorsRange;
	ofParameterGroup params_rangTypes;
#define NUM_TYPES_RANGES 12
	ofParameter<bool> rangTypes[NUM_TYPES_RANGES];
	void Changed_ColorRange(ofAbstractParameter &e);

	//extra
	ofParameter<bool> algoTypes[7];
	ofParameterGroup params_algoTypes;

	//-

	bool bErrorNoFiles = true;

public:

	//live reload colors file
	std::string path_Colors;
	void saveColors();
	bool bAutoExportPreset = true;

	//TODO
	//BUG: trying to avoid bug moving mouse..
	//void mouseEvent(ofxMacMouseEventArg &arg) {
	//    ofLogNotice() << "global mouse position: " << arg.x << ", " << arg.y;
	//}

	bool ENABLE_keys = false;
	//bool ENABLE_keys_PRE = false;

	// colorQuantizer
#ifdef USE_IMAGE_QUANTIZER
	ofxColorQuantizerHelper colorQuantizer;

	ofImage imageButtonSource;
	GLuint imageButtonID;
#endif

	//-

	// color theory
#define NUM_COLOR_THEORY_TYPES 8
	ofParameter<bool> theoryTypes[NUM_COLOR_THEORY_TYPES];
	shared_ptr<ColorWheelScheme> scheme;
	vector<ofColor> colorsTheory[NUM_COLOR_THEORY_TYPES];
	ofParameterGroup params_ColorTheory;
	ofParameter<ofColor> colorTheoryBase;
	ofParameter<int> colorScheme;
	ofParameter<std::string> colorSchemeName;
	ofParameter<int> amountColors;
	ofParameter<bool> bGetFromPicker;
	ofParameter<int> lastColorTheoryPicked_Palette;
	void Changed_ColorTheory(ofAbstractParameter &e);
	void refreshColorTheory();

	void Changed_ColorUserPalette(ofAbstractParameter &e);
	ofParameterGroup params_UserPalette;

	//-

	//user palette
	ofParameter<bool> bEditUserPalette;
	ofParameter<int> boxSizeUser;
	ofParameter<int> boxRowsUser;
	ofParameter<float> boxScale;
	ofParameter<bool> bFlipUserPalette;
	//ofParameter<bool> bUserPaletteVertical;

	//-

	// ColorWheelSchemes

	void palettes_colorTheory_setup();
	void palettes_colorTheory_update();
	//void ColorWheel_draw();

	ofParameter<ofColor> primaryColor;

	//shared_ptr<ColorWheelScheme> scheme;
	//vector<ofColor> colors;
	//ofxPanel panel;

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

	//-

	PresetManager myPresetManager;

	//--

	// ofxGuiPanelsLayout

#ifdef INCL_LAYOUT
	ofxGuiPanelsLayout panels;
#endif
	ofParameterGroup p_TOGGLES;
	ofParameter<bool> SHOW_GuiInternal{ "Gui Internal", false };
	ofParameter<bool> SHOW_ImGui{ "ImGui", true };

	//--

	// DEMO

	DEMO_palette myDEMO_palette;

	//--

	// PRESETS

	PresetPalette myPresetPalette;

	//TODO
	//BUG: should create a default preset because if myPreset is not detected it crashes
	std::string PRESET_name = "_emptyPreset";//default preset
	std::string PRESET_curveName = "curve01";

	//-

	// COLOUR LOVERS

	void colourLovers_drawPreview();
	#ifdef USE_COLOR_LOVERS
	ofxColourLoversHelper ColourLoversHelper;
#endif
	std::string myPalette_Name = "";
	ofColor myColor;
	vector<ofColor> myPalette;
	bool bUpdated_Palette_BACK = false;
	bool bUpdated_Color_BACK = false;

	//--

	// TODO: TESTING
	ofParameter<bool> preview{ "Preview", false };
	bool show_another_window;

	//--

	ofxColorManager();
	~ofxColorManager();

	void setup();
	void startup();

	//void update();
	//void draw();

	void update(ofEventArgs & args);
	void draw(ofEventArgs & args);

	void exit();
	void windowResized(int w, int h);

	//--

	float dt;
	float fps;

	//--------------------------------------------------------------
	void setFps(float _fps)
	{
		fps = _fps;
		dt = 1. / fps;
	}

	ofParameter<bool> SHOW_ColourLovers;
	ofParameter<bool> SHOW_ColourLovers_searcher;
	ofParameter<bool> SHOW_AlgoPalettes;
	ofParameter<bool> SHOW_BrowserColors;
	ofParameter<bool> SHOW_Gradient;
	ofParameter<bool> SHOW_Curve;
	ofParameter<bool> SHOW_ALL_GUI;
	ofParameter<bool> SHOW_GUI_MINI;
	ofParameter<bool> SHOW_debugText;
	//ofParameter<bool> SHOW_Curve;
	ofParameter<bool> SHOW_Panels;
	ofParameter<bool> SHOW_Presets;
	ofParameter<bool> SHOW_BackGround;
	ofParameter<bool> SHOW_Picker;
	ofParameter<bool> SHOW_Library;
	ofParameter<bool> SHOW_Range;
	ofParameter<bool> SHOW_UserPalette;
	ofParameter<bool> SHOW_Theory;
	ofParameter<bool> SHOW_Quantizer;
	//ofParameter<bool> SHOW_CosineGradient;

	void setColor_TARGET(ofColor &c);//backwards pointer ofApp color
	ofColor *color_TARGET;//backwards pointer ofApp color

	//-----------------------------------------------------------

	// API

	vector<ofColor> getPalette();
	ofColor getColorAtPercent(float control);
	void setControl(float control);
	void setVisible(bool b);
	void setToggleVisible();
	void setVisible_GUI_MINI(bool b);
	void setVisible_debugText(bool b);
	void palette_drawMINI();
	void gradient_drawPreview(glm::vec2 pos, bool horizontal);
	void disableListeners();
	void enableListeners();

	//-----------------------------------------------------------

	ofxMouseRuler mouseRuler;

	//--

	// COLOR BROWSER

	ofxColorsBrowser ColorBrowser;
	vector<ofColor> ColorBrowser_palette;
	ofFloatColor color_BACK;
	ofFloatColor color_BACK_PRE;

	//--

	// USER PALETTE & CONTROL

	ofParameter<bool> bPaletteEdit;
	ofParameter<bool> bRandomColor;
	ofParameter<bool> bAddColor;
	ofParameter<bool> bRemoveColor;
	ofParameter<bool> bClearPalette;
	int palette_colorSelected = -1;

	//--

	// PARAMETERS

	ofParameterGroup params_data;
	ofParameterGroup params_color;
	ofParameterGroup params_palette;
	ofParameterGroup params_curve;
	ofParameterGroup params_control;

	void Changed_Controls(ofAbstractParameter &e);

	//--

	// ALGORITHMIC PALETTES

	ofxColorPalette complement;
	ofxColorPalette complementBrightness;
	ofxColorPalette triad;
	ofxColorPalette complementTriad;
	ofxColorPalette monochrome;
	ofxColorPalette monochromeBrightness;
	ofxColorPalette analogue;
	ofxColorPalette random;
	//ofxColorPalette::ColorChannel mode;

	ofParameter<int> amountColors_Alg;
	//number of colors. must be even sometimes to get same size in all palettes

	ofParameter<int> BRIGHTNESS;
	ofParameter<int> SATURATION;

	ofParameter<bool> bRandomPalette;
	ofParameter<bool> bAuto_palette_recall;//trig last used algo palette on click or change color
	ofParameter<bool> bLock_palette;

	//TODO
	void palettes_setPosition(glm::vec2 pos)
	{
		palettes_x = pos.x;
		palettes_y = pos.y;
	}

	void palettes_setup();
	void palettes_setup_labels();
	void palettes_update();
	void palettes_resize();
	void palettes_setVisible(bool b);

	void draw_palettes();

	ofParameter<bool> MODE_TweakSatBrg;
	// force SAT/BRG from panel SB sliders or all from color

	ofFloatColor base;

	//--

	// GUI

	ofxImGui::Gui gui;

	bool guiVisible;
	bool mouseOverGui;
	float widgetFactor = 0.9;
	ofxImGui::Settings mainSettings = ofxImGui::Settings();

	bool gui_Draw();
	void gui_Palette();
	void gui_Theory();
	void gui_Picker();
	void gui_Library();
	void gui_Background();
	void gui_Curve();
	void gui_Range();
	void gui_Presets();
	void gui_Panels();
	void gui_Quantizer();

	void gui_Layout();
	//void gui_imGui_Theme();

	//--

	// COLORS

	ofParameter<ofFloatColor> color_backGround;//main color
	ofParameter<bool> color_backGround_SET;
	ofParameter<bool> color_backGround_SETAUTO;
	ofParameter<bool> color_backGround_Darker;
	ofParameter<float> backgroundDarkness;
	//float backgroundDarkness_PRE;

	ofParameter<bool> backgroundENABLE{ "DRAW BG",false };

	void setBackground_ENABLE(bool b);

	// TEST
	bool bCallback_ENABLED = true;

	void color_picked_Update_To_HSV();

	// MAIN COLOR
	ofParameter<ofFloatColor> color_Picked;
	//ofParameter<ofFloatColor> color_picked2;
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
	ofFloatColor color_clicked;
	ofFloatColor color_clicked_PRE;
	ofRectangle r_color_clicked;
	bool bColor_clicked_DISABLED = false;

	//-

	// TODO: TEST LINKING

	ofParameter<ofFloatColor> color_Clicked;

	void Changed_ColorPicked(ofFloatColor &color);
	void Changed_ColorClicked(ofFloatColor &color);

	// TEST
	void refreshPicker_Touched();

	//--

	// USER PALETTE OF COLORS

	vector<ofColor> palette;//main user palette

	void palette_addColor(ofColor c);
	void palette_removeColorLast();
	void palette_removeColor(int c);
	void palette_clear();
	void palette_addColor_toInterface(ofColor c);
	void palette_rearrenge();//resize boxes when adding removing colors to user palette
	void palette_touchedColor(std::string name);
	void palette_recallFromPalettes(int p);
	void palette_load_ColourLovers();

	//-

	// PALETTE LIBRARY

#define NUM_COLORS_PANTONE 2310
	//int palSize = IM_ARRAYSIZE(saved_palette);
	int palSize = (NUM_COLORS_PANTONE);
	int rowSizePal = 7;//7 colors per row Pantone lib
	bool doublePage;
	int numLines = 10;//rows per page
	int numColorsPage = numLines * rowSizePal;//70
	int totalNumColors = NUM_COLORS_PANTONE;//pantone
	int maxPages = totalNumColors / numColorsPage - 1;
	std::string lastColorPickedNameColor = "";
	int lastColorPicked;
	int lastColorPicked_Palette;
	ofParameter<int> paletteLibPage{ "PAGE" , 0, 0, maxPages };
	ofParameter<int> paletteLibPage_param{ "page", 0, 0, maxPages };

	//----

	// INTERFACE
	
#ifdef USE_RECTANGLE_INTERFACES
	ofxInterface::Node *scene;
	vector<ButtonExample *> btns_palette;//button color box for each color of all algorithmic palettes

	void interface_setup();
	void interface_update();
	void interface_draw();

	bool bShowDebug = false;

	// ALGORITHMIC PALETTES
	vector<ButtonExample *> btns_plt_Triad;       // 1
	vector<ButtonExample *> btns_plt_ComplTriad;  // 2
	vector<ButtonExample *> btns_plt_CompSat;     // 3
	vector<ButtonExample *> btns_plt_ComplBrgt;   // 4
	vector<ButtonExample *> btns_plt_MonoSat;     // 5
	vector<ButtonExample *> btns_plt_MonoBrgt;    // 6
	vector<ButtonExample *> btns_plt_Analog;      // 7
	//vector<ButtonExample *> btns_plt_Random;    // 8
	//int NUM_PALETTES = 8;

	// colour theory color palettes
	vector<ButtonExample *> btns_plt_CT_Analogous;
	vector<ButtonExample *> btns_plt_CT_Complementary;
	vector<ButtonExample *> btns_plt_CT_SplitComplementary;
	vector<ButtonExample *> btns_plt_CT_Compound;
	vector<ButtonExample *> btns_plt_CT_FlippedCompound;
	vector<ButtonExample *> btns_plt_CT_Monochrome;
	vector<ButtonExample *> btns_plt_CT_Triad;
	vector<ButtonExample *> btns_plt_CT_Tetrad;

	// pointer back link the outside (ofApp) variable
	vector<ButtonPaletteSelector *> btns_plt_Selector; // 1-8
#endif

	int NUM_PALETTES = 7;//without random
	int NUM_CT_PALETTES = 8;

	//int NUM_TOTAL_PALETTES = 16;//TODO
	int NUM_TOTAL_PALETTES = NUM_PALETTES + NUM_CT_PALETTES;//TODO //without random

	int SELECTED_palette = -1;
	int SELECTED_palette_PRE = -1;//to check if changed on update() loop
	int SELECTED_palette_LAST = 3;//default last palette type triggered. compBrg by default

	//----

	// GRADIENT

	ofxColorGradient<ofColor> gradient;//unmodified gradient with curveTool
	ofParameter<bool> gradient_hard;//stepped
	void gradient_draw();

	//-

	// CURVES

	ofxCurvesTool curvesTool;

	void curveTool_setup();
	void curveTool_update();
	void curveTool_draw();

	int curveTool_amount = 256;
	std::string curveTool_name = "curves.yml";
	ofImage curve_img_gradient;
	ofxSimpleSlider curve_pos_slider;
	ofParameter<float> curve_pos;
	ofParameter<float> curve_pos_out;
	ofParameter<bool> bResetCurve;
	ofParameter<float> curveMod;
	ofxSimpleSlider curveMod_Slider;
	int curve_pos_LUT = 0;

	//-

	// TEST CURVE

	bool TEST_MODE = false;
	float TEST_Speed = .75;
	bool TEST_CycleMODE = false;
	int TEST_maxFrames = 300;//slowest period
	bool TEST_toBackground = true;
	float framePrc;
	ofParameter<bool> TEST_DEMO{ "ENABLE DEMO", false };

	////TODO: make pauses between any test trig..
	bool bTEST_pause = false;
	//int TEST_pauseLong = 2000;
	//int TEST_pauseChrono = 0;

	//--

	// PRESET MANAGER

	void preset_save(std::string p);
	void preset_load(std::string p);

	void palette_save(std::string p);
	void palette_load(std::string p);

	bool MODE_newPreset = false;
	std::string textInput_New = "new preset";

	//-

	// FILES

	void preset_filesRefresh();
	std::vector<std::string> fileNames;
	std::vector<ofFile> files;
	int currentFile = 0;
	std::string textInput_temp = "type name";

	//--

	// APP SETTINGS XML

	void saveAppSettings(ofParameterGroup &g, std::string path);
	void loadAppSettings(ofParameterGroup &g, std::string path);
	ofParameterGroup XML_params;
	std::string XML_path = "settings/ofxColorManager.xml";

	//--

private:

	//--

	// LAYOUT

	int guiWidth;
	int gui_x, gui_y, gui_w, gui_h;
	int gui2_x, gui2_y, gui2_w, gui2_h;
	int gui3_x, gui3_y, gui3_w, gui3_h;
	int gui4_x, gui4_y, gui4_w, gui4_h;
	int box_user_size;//user palette colors
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
	int slider_x;
	int slider_y;
	int slider_w;
	int slider_h;
	int sliderMod_x;
	int sliderMod_y;
	int sliderMod_w;
	int sliderMod_h;
	int grad_x;
	int grad_y;
	int grad_w;
	int grad_h;
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

	// LISTENERS

	void keyPressed(ofKeyEventArgs &eventArgs);
	void keyReleased(ofKeyEventArgs &eventArgs);
	void mouseDragged(ofMouseEventArgs &eventArgs);
	void mousePressed(ofMouseEventArgs &eventArgs);
	void mouseReleased(ofMouseEventArgs &eventArgs);
	void addKeysListeners();
	void removeKeysListeners();
	void addMouseListeners();
	void removeMouseListeners();

	//--

	////COSINE GRADIENT
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

	// Helper to display a little (?) mark which shows a tooltip when hovered.
	// In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.txt)
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

};
