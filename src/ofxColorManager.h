
#pragma once
#include "ofMain.h"

#include "ofxImGui.h"
#include "ofxColorPalette.h"
#define NUM_COLOR 9
#define RECT_SIZE 35
#define PADDING 5
#include "ofxInterface.h"
#include "ButtonExample.h"
#include "ofxColorGradient.h"
#include "ofxCurvesTool.h"

class ofxColorManager {

public:

    ofxColorManager();
    ~ofxColorManager();

    ofFloatColor color_p;

    void setup();
    void update();
    void draw();
    void exit();

    //-

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

//    ofParameter<bool> MODE;
    float brightness;
    float saturation;
    ofParameter<float> BRIGHTNESS;
    ofParameter<float> SATURATION;
    ofParameter<bool> bRandomPalette;
    void setup_palettes();
    void update_palettes();
    void draw_palettes();

    //-

    // GUI
    bool imGui();
    ofxImGui::Gui gui;
    bool guiVisible;
    bool mouseOverGui;

    ofParameter<ofFloatColor> myColor;
    ofParameter<ofFloatColor> color_backGround;

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

    vector<ButtonExample*> buttons_palette_Triad;//1
    vector<ButtonExample*> buttons_palette_ComplTriad;//2
    vector<ButtonExample*> buttons_palette_CompSat;//3
    vector<ButtonExample*> buttons_palette_ComplBrgt;//4
    vector<ButtonExample*> buttons_palette_MonoSat;//5
    vector<ButtonExample*> buttons_palette_MonoBrgt;//6
    vector<ButtonExample*> buttons_palette_Analog;//7
    vector<ButtonExample*> buttons_palette_Random;//8

    void add_color_Palette(int i);

    //-

    ofxColorGradient<ofColor> gradient;

    //-

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

    //--
};