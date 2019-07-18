
#pragma once
#include "ofMain.h"
#include "ofxImGui.h"

#include "ofxColorPalette.h"
#define NUM_COLOR 9
#define RECT_SIZE 35
#define PADDING 5

#include "ofxInterface.h"
#include "ButtonExample.h"

class ofxColorManager {

public:

    ofxColorManager();
    ~ofxColorManager();

    void setup();
    void update();
    void draw();
    void exit();

    ofxColorPalette complement;
    ofxColorPalette complementBrightness;
    ofxColorPalette triad;
    ofxColorPalette complementTriad;
    ofxColorPalette monochrome;
    ofxColorPalette monochromeBrightness;
    ofxColorPalette analogue;
    ofxColorPalette random;
    ofxColorPalette::ColorChannel mode;
    float brightness;
    float saturation;
    ofParameter<float> BRIGHTNESS;
    ofParameter<float> SATURATION;
    void update_color(int brg, int sat);
    ofParameter<bool> MODE;

    bool imGui();
    ofxImGui::Gui gui;
    bool guiVisible;
    bool mouseOverGui;

    // Parameters
    ofParameter<ofFloatColor> myColor{ "myColor", ofFloatColor::black };

    ofColor color_backGround;

    void add_color(ofColor c);
    vector<ofColor> palette;
    vector<ofColor> colors;

    ofxInterface::Node* scene;
	vector<ButtonExample*> buttons;
    void setup_UI();
    void update_UI();
    void draw_UI();
    void addColorUI(ofColor c);
    bool bShowDebug = true;

    void keyPressed( ofKeyEventArgs& eventArgs);
    void keyReleased( ofKeyEventArgs& eventArgs );
    void addKeysListeners();
    void removeKeysListeners();

    void mouseDragged( ofMouseEventArgs& eventArgs );
    void mousePressed( ofMouseEventArgs& eventArgs );
    void mouseReleased( ofMouseEventArgs& eventArgs );
    void addMouseListeners();
    void removeMouseListeners();

};