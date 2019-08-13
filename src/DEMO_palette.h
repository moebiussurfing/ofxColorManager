//
// Created by Manu Molina on 8/6/19.
//

#ifndef INC_4_COLORMANAGER_DEMO_PALETTE_H
#define INC_4_COLORMANAGER_DEMO_PALETTE_H

#include "ofMain.h";
#include "ofxCameraSaveLoad.h"

class DEMO_palette {

public:
    void setup();
    void update();
    void draw();

    void clear(){
        // clear DEMO1 objects
        bDEMO1_clear = true;
        pauseCreate = false;
    }

    void restart(){
        bDEMO1_clear = true;
        pauseCreate = false;
    }

    void start(){
        pauseCreate = false;
    }

    void setClear(bool b){
        bDEMO1_clear = true;
    }

    void setPalette(vector<ofColor> &_palette )
    {
        palette = &_palette;
    }

    //-

private:

    vector<ofColor> *palette;

    // DEMO 1
    bool ENABLE_DEMO1 = true;
    vector<glm::vec2> locations;
    vector<glm::vec2> velocities;
    vector<ofColor> colors;
    bool bDEMO1_clear = false;
    bool bDEMO1_stop = false;
    ofEasyCam cam;
    int iColor = 0;
    bool pauseCreate = false;

    // DEMO 2
    bool ENABLE_DEMO2 = true;
};


#endif //INC_4_COLORMANAGER_DEMO_PALETTE_H
