
#pragma once
#include "ofMain.h"

#include "ofxImGui.h"
#include "ofxColorPalette.h"
//#define NUM_COLOR 9
//#define RECT_SIZE 35
//#define PADDING 5
#include "ofxInterface.h"
#include "ButtonExample.h"
#include "ofxColorGradient.h"
#include "ofxCurvesTool.h"
#include "ofxMouseRuler.h"
#include "ofxSimpleSlider.h"

class ofxColorManager {

public:

    ofxMouseRuler mouseRuler;

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

    //--

    ofxColorManager();
    ~ofxColorManager();

    void setup();
    void update();
    void draw();
    void exit();

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

    //-

    // PARAMETERS

    ofParameterGroup params_data;
    ofParameterGroup params_color;
    ofParameterGroup params_palette;
    ofParameterGroup params_curve;

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
//    ofParameter<bool> MODE;

    //-

    // GUI

    bool imGui();
    ofxImGui::Gui gui;
    bool guiVisible;
    bool mouseOverGui;
    void imGui_theme();

    //-

    // COLORS

    ofParameter<ofFloatColor> myColor;
    ofParameter<ofFloatColor> color_backGround;

    ofFloatColor color_p;//TODO: pointer color to get click from button class

    //-

    // USER PALETTE

    void add_color(ofColor c);
    void remove_colorLast();
    vector<ofColor> palette;
    vector<ofColor> colors;

    //-

    // INTERFACE

    ofxInterface::Node* scene;
	vector<ButtonExample*> buttons;
    void setup_Interface();
    void update_Interface();
    void draw_Interface();
    void add_color_Interface(ofColor c);
    bool bShowDebug = false;

    vector<ButtonExample*> buttons_palette_Triad;       // 1
    vector<ButtonExample*> buttons_palette_ComplTriad;  // 2
    vector<ButtonExample*> buttons_palette_CompSat;     // 3
    vector<ButtonExample*> buttons_palette_ComplBrgt;   // 4
    vector<ButtonExample*> buttons_palette_MonoSat;     // 5
    vector<ButtonExample*> buttons_palette_MonoBrgt;    // 6
    vector<ButtonExample*> buttons_palette_Analog;      // 7
    vector<ButtonExample*> buttons_palette_Random;      // 8

//    void add_color_Palette(int i);

    //-

    // GRADIENT

    ofxColorGradient<ofColor> gradient;
    ofParameter<bool> gradient_hard;

    //-

    // CURVES

    ofxCurvesTool curvesTool;
    ofImage img;
    bool show;
    float cnt = 0;
    int amount = amount;
    void setup_curveTool();
    void update_curveTool();
    void draw_curveTool();
    ofParameter<float> curve_pos;
    ofParameter<bool> bResetCurve;
    ofxSimpleSlider curveSlider;

    //--

    // XML settings

    void save_group_XML(ofParameterGroup &g, string path);
    void load_group_XML(ofParameterGroup &g, string path);
    ofParameterGroup XML_params;
    string XML_path = "ofxColorManager.xml";

    //--
};