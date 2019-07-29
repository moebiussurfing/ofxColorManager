#pragma once
#include "ofMain.h"

#include "ofxImGui.h"
#include "ofxInterface.h"
#include "ButtonPaletteSelector.h"
#include "ButtonExample.h"
#include "ofxColorGradient.h"
#include "ofxColorPalette.h"
#include "ofxColorsBrowser.h"
#include "ofxCurvesTool.h"
#include "ofxSimpleSlider.h"
#include "ofxCereal.h"
#include "ofxMouseRuler.h"
#include "ofxColourLoversHelper.h"

//--

// JSON SERIALIZER

using namespace ofxCereal;

struct CustomData
{
    string name;
    vector<ofColor> palette;

OFX_CEREAL_DEFINE(CEREAL_NVP(name), CEREAL_NVP(palette))
};

struct PresetData
{
    string name;
    vector<ofColor> palette;
    ofColor backgroundColor;
    string curveName;

OFX_CEREAL_DEFINE(CEREAL_NVP(name), CEREAL_NVP(curveName), CEREAL_NVP(palette), CEREAL_NVP(backgroundColor))
};

//--

class ofxColorManager {

public:

    //-

    // COLOUR LOVERS

    void draw_ColourLovers();
    ofxColourLoversHelper ColourLoversHelper;
    string myPalette_Name = "";
    ofColor myColor;
    vector<ofColor> myPalette;
    bool bUpdated_BACK = false;

    //--

    // TODO: TESTING
    ofParameter<bool> preview{ "Preview", false };
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
    ofParameter<bool> SHOW_AlgoPalettes;
    ofParameter<bool> SHOW_BrowserColors;
    ofParameter<bool> SHOW_Curve;
    ofParameter<bool> SHOW_ALL_GUI;
    ofParameter<bool> SHOW_GUI_MINI;
    ofParameter<bool> SHOW_debugText;

    void setColor_TARGET(ofColor &c);//backwards pointer ofApp color
    ofColor *color_TARGET;//backwards pointer ofApp color

    //----

    // API

    vector<ofColor> getPalette();
    ofColor getColorAtPercent(float control);
    void setControl(float control);

    void setVisible(bool b);
    void setVisible_GUI_MINI(bool b);
    void setVisible_debugText(bool b);

    void draw_PaleteMINI();
    void draw_previewGradient(glm::vec2 pos, bool horizontal);

    void disableListeners();
    void enableListeners();

    //----

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
    void Changed_control(ofAbstractParameter &e);

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
    ofParameter<int> NUM_ALGO_PALETTES;//number of colors. must be even
    //ofxColorPalette::ColorChannel mode;

    ofParameter<int> BRIGHTNESS;
    ofParameter<int> SATURATION;
    ofParameter<bool> bRandomPalette;
    ofParameter<bool> bAuto_palette_recall;//trig last used algo palette on click or change color
    ofParameter<bool> bLock_palette;

    void palettes_setup();
    void palettes_setup_labels();
    void palettes_update();
    void palettes_resize();
    void palettes_setVisible(bool b);
    void draw_palettes();
    ofParameter<bool> MODE_Palette;

    //--

    // GUI

    ofxImGui::Gui gui;
    bool gui_imGui();
    bool guiVisible;
    bool mouseOverGui;
    void gui_setup_layout();
    void gui_imGui_theme();

    //--

    // COLORS

    ofParameter<ofFloatColor> color_backGround;
    bool backgroundENABLE = false;
    void setBackground_ENABLE(bool b);

    ofParameter<ofFloatColor> color_picked;
    ofRectangle r_color_picked;
    ofParameter<int> color_HUE;
    ofParameter<int> color_SAT;
    ofParameter<int> color_BRG;

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

    //--

    // USER PALETTE OF COLORS

    vector<ofColor> palette;
    void palette_addColor(ofColor c);
    void palette_removeColorLast();
    void palette_clear();
    void palette_addColor_toInterface(ofColor c);
    void palette_rearrenge();//resize boxes when adding removing colors to user palette
    void palette_touched(string name);
    void palette_recallFromPalettes(int p);
    void palette_load_ColourLovers();

    //-

    // INTERFACE

    ofxInterface::Node* scene;
    vector<ButtonExample*> btns_palette;
    void interface_setup();
    void interface_update();
    void interface_draw();
    bool bShowDebug = false;

    // ALGORITHMIC PALETTES
    vector<ButtonExample*> btns_plt_Triad;       // 1
    vector<ButtonExample*> btns_plt_ComplTriad;  // 2
    vector<ButtonExample*> btns_plt_CompSat;     // 3
    vector<ButtonExample*> btns_plt_ComplBrgt;   // 4
    vector<ButtonExample*> btns_plt_MonoSat;     // 5
    vector<ButtonExample*> btns_plt_MonoBrgt;    // 6
    vector<ButtonExample*> btns_plt_Analog;      // 7
    vector<ButtonExample*> btns_plt_Random;      // 8
    int NUM_PALETTES = 8;

    // pointer back link the outside (ofApp) variable
    vector<ButtonPaletteSelector*> btns_plt_Selector; // 1-8
    int SELECTED_palette = -1;
    int SELECTED_palette_PRE = -1;//to check if changed on update() loop
    int SELECTED_palette_LAST = 3;//default last palette type triggered. compBrg by default

    //-

    // GRADIENT

    ofxColorGradient<ofColor> gradient;//unmodified gradient with curveTool
    ofParameter<bool> gradient_hard;//stepped

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
    ofParameter<bool> bCurveSlider;
    int curve_pos_LUT = 0;

    //--

    // JSON PALETTES SERIALIZER

    CustomData data;//palette
    PresetData presetData;//bundle preset: palette+curve+gradient+background

    void palette_save(string p);
    void palette_load(string p);
    string path_palettes = "assets/palettes/";

    void preset_save(string p);
    void preset_load(string p);
    string preset_path = "assets/presets/";

    //--

    // App settings XML

    void save_group_XML(ofParameterGroup &g, string path);
    void load_group_XML(ofParameterGroup &g, string path);
    ofParameterGroup XML_params;
    string XML_path = "ofxColorManager.xml";

    //--

private:

    //--

    // LAYOUT

    int gui_x, gui_y, gui_w, gui_h;
    int box_size;
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

    void keyPressed( ofKeyEventArgs& eventArgs);
    void keyReleased( ofKeyEventArgs& eventArgs );
    void addKeysListeners();
    void removeKeysListeners();
    void mouseDragged( ofMouseEventArgs& eventArgs );
    void mousePressed( ofMouseEventArgs& eventArgs );
    void mouseReleased( ofMouseEventArgs& eventArgs );
    void addMouseListeners();
    void removeMouseListeners();

    //--
};
