//
// Created by moebiussurfing on 8/9/19.
//

#ifndef INC_5_COLORMANAGER_PRESETMANAGER_H
#define INC_5_COLORMANAGER_PRESETMANAGER_H

#include "ofMain.h"
#include "PresetPalette.h"
#include "ofxImGui.h"

class PresetManager {

public:

    PresetManager(){
        addMouseListeners();
    };

    ~PresetManager(){
        removeMouseListeners();
    };

    void setup();
    void update();
    void draw();

    //--


    //--

    void mouseDragged( ofMouseEventArgs& eventArgs );
    void mousePressed( ofMouseEventArgs& eventArgs );
    void mouseReleased( ofMouseEventArgs& eventArgs );
    void addMouseListeners();
    void removeMouseListeners();

private:
    ofxImGui::Gui gui;
    float v;
};


#endif //INC_5_COLORMANAGER_PRESETMANAGER_H
