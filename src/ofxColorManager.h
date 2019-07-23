
#pragma once
#include "ofMain.h"

#include "ofxImGui.h"
#include "ofxInterface.h"
#include "ofxInterfaceWidgets.h"
#include "ButtonPaletteSelector.h"
#include "ButtonExample.h"
#include "ofxColorGradient.h"
#include "ofxColorPalette.h"
#include "ofxColorsBrowser.h"
#include "ofxCurvesTool.h"
#include "ofxSimpleSlider.h"
#include "ofxCereal.h"
#include "ofxMouseRuler.h"

//--

// JSON SERIALIZER

using namespace ofxCereal;

struct CustomData
{
    string name;
    vector<ofColor> palette;

OFX_CEREAL_DEFINE(CEREAL_NVP(name), CEREAL_NVP(palette))

};

//--

class ofxColorManager {

    CustomData data;
    ofPolyline poly;

public:

    ofxMouseRuler mouseRuler;

    //--

    // COLOR BROWSER

    ofxColorsBrowser ColorBrowser;
    ofFloatColor color_BACK;

    //--

    // LAYOUT

    int pad; //global mini pad

    int gui_x, gui_y, gui_w, gui_h;

    int c_grad_x, c_grad_y, c_grad_w, c_grad_h;
    int pos_curve_x;
    int pos_curve_y;
    int pos_curve_prev_x;
    int pos_curve_prev_y;
    int pos_curve_prev_w;
    int pos_curve_prev_h;

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
    int box_size;

    int palette_x;
    int palette_y;
    int color_size;

    int currColor_x;
    int currColor_y;

    int colorPick_x, colorPick_y, colorPick_w, colorPick_h;
    int color_x, color_y, color_w, color_h;

    //--

    ofxColorManager();
    ~ofxColorManager();

    void setup();
    void update();
    void draw();
    void exit();

    float dt;

    //-

    // PARAMETERS

    ofParameter<bool> bRandomColor;
    ofParameter<bool> bAddColor;
    ofParameter<bool> bRemoveColor;
    ofParameter<bool> bClearPalette;

    ofParameterGroup params_data;
    ofParameterGroup params_color;
    ofParameterGroup params_palette;
    ofParameterGroup params_curve;
    ofParameterGroup params_control;
    void Changed_control(ofAbstractParameter &e);

    //-

    // ALGORITHMIC PALETTES

    ofxColorPalette complement;
    ofxColorPalette complementBrightness;
    ofxColorPalette triad;
    ofxColorPalette complementTriad;
    ofxColorPalette monochrome;
    ofxColorPalette monochromeBrightness;
    ofxColorPalette analogue;
    ofxColorPalette random;
//    ofxColorPalette::ColorChannel mode;

    float brightness;
    float saturation;
    ofParameter<float> BRIGHTNESS;
    ofParameter<float> SATURATION;
    ofParameter<bool> bRandomPalette;
    void setup_palettes();
    void update_palettes();
    void draw_palettes();
    ofParameter<bool> MODE_Palette;

    //-

    // GUI

    bool imGui();
    ofxImGui::Gui gui;
    bool guiVisible;
    bool mouseOverGui;
    void setup_Gui_layout();
    void imGui_theme();

    //-

    // COLORS

    ofParameter<ofFloatColor> color_backGround;

    ofParameter<ofFloatColor> color_picked;
    ofRectangle r_color_picked;

    ofParameter<int> color_HUE;
    ofParameter<int> color_SAT;
    ofParameter<int> color_BRG;

    ofFloatColor color_clicked;//TODO: pointer color to get click from button class
    ofFloatColor color_clicked_PRE;
    ofRectangle r_color_clicked;
    bool bColor_clicked_DISABLED = false;

    //-

    // TODO: TEST LINKING

    ofParameter<ofFloatColor> color_clicked_param;

//    ofEventListener listener = test1->myParameter.newListener([this](unsigned char &v){ test3->myParameter.set(v); });
//    ofEventListener listener = color_picked.newListener([this](unsigned char &v){ color_backGround.set(v); });
    void Changed_color_picked(ofFloatColor &color);
    void Changed_color_clicked(ofFloatColor &color);

    //-

    // USER PALETTE

    vector<ofColor> palette;
//    vector<ofColor> colors;

    void add_color(ofColor c);
    void remove_colorLast();
    void clearPalette();

    // JSON

    void savePalette(string p);
    void loadPalette(string p);
    string path_palettes = "assets/palettes/";

    //-

    // INTERFACE

    ofxInterface::Node* scene;
    vector<ButtonExample*> btns_palette;
    void setup_Interface();
    void update_Interface();
    void draw_Interface();
    void add_color_Interface(ofColor c);
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

    // pointer back link the outside (ofApp) variable
    vector<ButtonPaletteSelector*> btns_plt_Selector; // 1-8
    int SELECTED_palette = -1;
    int SELECTED_palette_PRE = -1;//to check if changed on update() loop

    void recall_AlgorithmicPalette(int p);

//    void add_color_Palette(int i);

    //-

    // GRADIENT

    ofxColorGradient<ofColor> gradient;
    ofParameter<bool> gradient_hard;

    //-

    // CURVES

    ofxCurvesTool curvesTool;
    ofImage img;
    bool curveShow = true;
    float cnt = 0;
    int amount = 256;
    void setup_curveTool();
    void update_curveTool();
    void draw_curveTool();
    ofParameter<float> curve_pos;
    ofxSimpleSlider curveSlider;
    ofParameter<bool> bResetCurve;

    //--

    // XML settings

    void save_group_XML(ofParameterGroup &g, string path);
    void load_group_XML(ofParameterGroup &g, string path);
    ofParameterGroup XML_params;
    string XML_path = "ofxColorManager.xml";

    //-

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