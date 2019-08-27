#pragma once

#include "ofMain.h"

#include "ofxColorGradient.h"
#include "ofxColorPalette.h"
#include "ofxColorsBrowser.h"
#include "ofxColourLoversHelper.h"
#include "ColorWheelScheme.h"
#include "ColorWheelSchemes.h"

using namespace ofxColorTheory;
using namespace std;

#include "ofxColorQuantizerHelper.h"

#include "ofxCurvesTool.h"
#include "ofxSimpleSlider.h"

#include "ofxInterface.h"
#include "ButtonPaletteSelector.h"
#include "ButtonExample.h"
#include "ofxUndoSimple.h"
#include "ofxMouseRuler.h"

#include "ofxGuiPanelsLayout.h"
#include "ofxGui.h"
#include "ofxImGui.h"

#include "PresetManager.h"
#include "PresetPalette.h"
#include "DEMO_palette.h"

//--

class ofxColorManager
{

public:

    ofxColorQuantizerHelper colorQuantizer;

    bool ENABLE_keys = true;

    string textInput_New = "new preset";

    //-

    // ColorWheelSchemes

    void palettes_colorTheory_setup();
    void palettes_colorTheory_update();
    //    void ColorWheel_draw();

    //    shared_ptr<ColorWheelScheme> scheme;
    //    vector<ofColor> colors;
    //
    //    ofxPanel panel;
    //    ofParameterGroup group;
    ofParameter<ofColor> primaryColor;
    //    ofParameter<int> colorScheme;
    //    ofParameter<string> colorSchemeName;
    //    ofParameter<int> numColors;

    string scheme_Analogous_name;
    string scheme_Complementary_name;
    string scheme_SplitComplementary_name;
    string scheme_Compound_name;
    string scheme_FlippedCompound_name;
    string scheme_Monochrome_name;
    string scheme_Tetrad_name;
    string scheme_Triad_name;

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

    ofxGuiPanelsLayout panels;
    ofParameterGroup p_TOGGLES;
    ofParameter<bool> SHOW_Layout_Gui{"SHOW_Layout_Gui", true};
    ofParameter<bool> SHOW_ImGui{"SHOW_ImGui", true};

    //--

    // DEMO

    DEMO_palette myDEMO_palette;

    //--

    // PRESETS

    PresetPalette myPresetPalette;

    //TODO
    //BUG: should create a default preset because if myPreset is not detected it crashes
    std::string PRESET_name = "_emptyPreset";//default preset
    string PRESET_curveName = "curve01";

    //-

    // COLOUR LOVERS

    void colourLovers_drawPreview();

    ofxColourLoversHelper ColourLoversHelper;
    string myPalette_Name = "";
    ofColor myColor;
    vector<ofColor> myPalette;
    bool bUpdated_Palette_BACK = false;
    bool bUpdated_Color_BACK = false;

    //--

    // TODO: TESTING
    ofParameter<bool> preview{"Preview", false};
    bool show_another_window;

    //--

    ofxColorManager();
    ~ofxColorManager();

    void setup();
    void update();
    void draw();
    void exit();
    void windowResized(int w, int h);

    //--

    float dt;

    ofParameter<bool> SHOW_ColourLovers;
    ofParameter<bool> SHOW_ColourLovers_searcher;
    ofParameter<bool> SHOW_AlgoPalettes;
    ofParameter<bool> SHOW_BrowserColors;
    ofParameter<bool> SHOW_Gradient;
    ofParameter<bool> SHOW_Curve;
    ofParameter<bool> SHOW_ALL_GUI;
    ofParameter<bool> SHOW_GUI_MINI;
    ofParameter<bool> SHOW_debugText;
    ofParameter<bool> SHOW_TEST_Curve;
    ofParameter<bool> SHOW_PanelsManager;
    ofParameter<bool> SHOW_PresetManager;
    ofParameter<bool> SHOW_ColorManager;
    ofParameter<bool> SHOW_ColorPicker;
    ofParameter<bool> SHOW_UserPalette;
    ofParameter<bool> SHOW_ColorQuantizer;

    void setColor_TARGET(ofColor &c);//backwards pointer ofApp color
    ofColor *color_TARGET;//backwards pointer ofApp color

    //-----------------------------------------------------------

    // API

    vector<ofColor> getPalette();
    ofColor getColorAtPercent(float control);
    void setControl(float control);
    void setVisible(bool b);
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

    void Changed_CONTROL(ofAbstractParameter &e);

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
    ofParameter<int> NUM_COLORS_ALGO_PALETTES;
    //number of colors. must be even sometimes to get same size in all palettes
    //ofxColorPalette::ColorChannel mode;

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

    ofParameter<bool> MODE_Palette;
    // force SAT/BRG from panel SB sliders or all from color

    ofFloatColor base;

    //--

    // GUI

    ofxImGui::Gui gui;
    bool guiVisible;
    bool mouseOverGui;
    float widgetFactor = 0.9;
    ofxImGui::Settings mainSettings = ofxImGui::Settings();

    bool gui_imGui();
    void gui_imGui_ColorPicker();
    void gui_imGui_ColorManager();
    void gui_imGui_PresetManager();
    void gui_imGui_ControlPanels();
    void gui_setup_layout();
    void gui_imGui_theme();

    //--

    // COLORS

    ofParameter<ofFloatColor> color_backGround;//main color
    ofParameter<bool> color_backGround_SET;
    ofParameter<bool> color_backGround_SETAUTO;
    ofParameter<bool> color_backGround_Darker;
    ofParameter<float> backgroundDarkness;
    //    float backgroundDarkness_PRE;

    bool backgroundENABLE = false;

    void setBackground_ENABLE(bool b);

    // TEST
    bool LISTEN_isEnabled = true;

    void color_picked_Update_To_HSV();

    // MAIN COLOR
    ofParameter<ofFloatColor> color_picked;
    ofxUndoSimple<ofFloatColor> color_Undo;

    ofRectangle r_color_picked;
    ofParameter<int> color_HUE;
    ofParameter<int> color_SAT;
    ofParameter<int> color_BRG;

    // ?color clicked comes from algo palette colors
    //TODO: pointer color to get click from button class
    ofFloatColor color_clicked;
    ofFloatColor color_clicked_PRE;
    ofRectangle r_color_clicked;
    bool bColor_clicked_DISABLED = false;

    //-

    // TODO: TEST LINKING

    ofParameter<ofFloatColor> color_clicked_param;

    void Changed_color_picked(ofFloatColor &color);
    void Changed_color_clicked(ofFloatColor &color);

    // TEST
    void update_color_picked_CHANGES();

    //--

    // USER PALETTE OF COLORS

    vector<ofColor> palette;//main user palette

    void palette_addColor(ofColor c);
    void palette_removeColorLast();
    void palette_clear();
    void palette_addColor_toInterface(ofColor c);
    void palette_rearrenge();//resize boxes when adding removing colors to user palette
    void palette_touchedColor(string name);
    void palette_recallFromPalettes(int p);
    void palette_load_ColourLovers();

    //-

    // PALETTE LIBRARY

#define NUM_COLORS_PANTONE 2310
    //    int palSize = IM_ARRAYSIZE(saved_palette);
    int palSize = (NUM_COLORS_PANTONE);
    int rowSizePal = 7;//7 colors per row Pantone lib
    int numLines = 10;//rows per page
    int numColorsPage = numLines * rowSizePal;//70
    int totalNumColors = NUM_COLORS_PANTONE;//pantone
    int maxPages = totalNumColors / numColorsPage - 1;
    string lastColorPickedNameColor = "";
    int lastColorPicked;
    int paletteLibPage = 0;
    ofParameter<int> paletteLibPage_param{"page", 0, 0, maxPages};


    //-

    // INTERFACE

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
    //    vector<ButtonExample *> btns_plt_Random;    // 8
    //    int NUM_PALETTES = 8;
    int NUM_PALETTES = 7;//without random

    // colour theory color palettes
    vector<ButtonExample *> btns_plt_CT_Analogous;
    vector<ButtonExample *> btns_plt_CT_Complementary;
    vector<ButtonExample *> btns_plt_CT_SplitComplementary;
    vector<ButtonExample *> btns_plt_CT_Compound;
    vector<ButtonExample *> btns_plt_CT_FlippedCompound;
    vector<ButtonExample *> btns_plt_CT_Monochrome;
    vector<ButtonExample *> btns_plt_CT_Triad;
    vector<ButtonExample *> btns_plt_CT_Tetrad;
    int NUM_CT_PALETTES = 8;

    //    int NUM_TOTAL_PALETTES = 16;//TODO
    int NUM_TOTAL_PALETTES = NUM_PALETTES + NUM_CT_PALETTES;//TODO //without random

    // pointer back link the outside (ofApp) variable
    vector<ButtonPaletteSelector *> btns_plt_Selector; // 1-8
    int SELECTED_palette = -1;
    int SELECTED_palette_PRE = -1;//to check if changed on update() loop
    int SELECTED_palette_LAST = 3;//default last palette type triggered. compBrg by default

    //-

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
    string curveTool_name = "curves.yml";
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
    ofParameter<bool> TEST_DEMO{"ENABLE DEMO", false};

    ////TODO: make pauses between any test trig..
    //bool bTEST_pause = false;
    //int TEST_pauseLong = 2000;
    //int TEST_pauseChrono = 0;

    //--

    // PRESET MANAGER

    void preset_save(string p);
    void preset_load(string p);

    void palette_save(string p);
    void palette_load(string p);

    bool MODE_newPreset = false;
    //-

    // FILES

    void preset_filesRefresh();
    std::vector<std::string> fileNames;
    std::vector<ofFile> files;
    int currentFile = 0;
    string textInput_temp = "type name";

    //--

    // APP SETTINGS XML

    void XML_save_AppSettings(ofParameterGroup &g, string path);
    void XML_load_AppSettings(ofParameterGroup &g, string path);
    ofParameterGroup XML_params;
    string XML_path = "ofxColorManager.xml";

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
};
