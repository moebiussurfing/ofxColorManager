//
// Created by moebiussurfing on 8/6/19.
//

#include "DEMO_palette.h"


//--------------------------------------------------------------
void DEMO_palette::setup() {
    //---

    // DEMO 1

    ofSetCircleResolution(50);

    ofxLoadCamera(cam, "DEMOcam");
    cam.disableMouseInput();
//    cam.enableOrtho();

    //--
}


//--------------------------------------------------------------
void DEMO_palette::draw() {
    // DEMO 1

    if (ENABLE_DEMO1) {
        ofPushMatrix();
        ofPushStyle();
        ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

        for (int i = 0; i < this->locations.size(); i++) {
            ofSetColor(this->colors[i]);

//        float radius = (2 * PI * glm::length(this->locations[i])) / 360 * 10;
            float radius = (2 * PI * glm::length(this->locations[i])) / 360 * 20;
//        float radius = (2 * PI * glm::length(this->locations[i])) / 360 * 40;

            ofDrawCircle(this->locations[i], radius);
        }
        ofPopStyle();
        ofPopMatrix();
    }

    //--

    // DEMO 2 - ROTATING RECTANGLES

    if (ENABLE_DEMO2) {
        this->cam.begin();

        ofPushMatrix();
        ofPushStyle();

        float scaleRects = 2.f;

//          float radius = 25;
        float radius = 300;
//          float radius = 150;

//          int iDeg = 25;
        int iDeg = 36;

//          int iDeg = 360/ (2*palette.size());
          int iCol = 0;

        for (int deg = 0; deg < 360; deg += iDeg) {
            float x = radius * cos(deg * DEG_TO_RAD);
            float y = radius * sin(deg * DEG_TO_RAD);
            ofColor c;

            //-

//            // 1. color from gradient
//            float cnt = ofMap(deg, 0, 360, 0.f, 1.f);
//            c = getColorAtPercent(cnt);//gradient not in the class...

            // 2. color from palette
//            c.set((palette[iCol]);
//            c.set((palette[iCol]->getColor()));
            iCol++;
//            iCol = iCol%palette.size();
            iCol = iCol%palette->size();

            //-

            ofSetColor(c);

//                ofRotateZ(ofGetFrameNum() * 0.25);
            ofRotateZ(ofGetFrameNum() * 0.01);

            ofPushMatrix();
            ofTranslate(ofVec3f(x, y, 0));
            ofRotateX(90);
            ofRotateY(deg + 90);

            ofRect(ofVec3f(0, 0, 0), scaleRects * 1000, scaleRects * 1000);

            ofPopMatrix();
        }

        ofPopStyle();
        ofPopMatrix();

        this->cam.end();
    }
}

//--------------------------------------------------------------
void DEMO_palette::update() {


// DEMO 1 - CIRCLES

    if (bDEMO1_clear)
//    if (bDEMO1_clear || locations.size()>10)
    {
        bDEMO1_clear = false;
        locations.clear();
        velocities.clear();
        colors.clear();
    }

//        int bloquer = locations.size()>10
    ofColor color;

// 1. create circle every x frames
    int frameCurrent = ofGetFrameNum() % 5;//speed retrig
    if (frameCurrent == 0 && !pauseCreate) {

//            // 2. create circle with randomly frequency
//        if ((ofRandom(100) < 50) && !pauseCreate) {
//        if ((ofRandom(100) < 5) && !pauseCreate) {
//        if (ofRandom(100) < 50) {//prob speed?
//        if (ofRandom(100) < 80) {//prob speed?

        this->locations.push_back(glm::vec2());

// randomize each circle/color speed
        this->velocities.push_back(glm::normalize(glm::vec2(ofRandom(-1, 1), ofRandom(-1, 1))) * 2);

//        if (iColor == palette.size() - 1)
        if (iColor == palette->size() - 1)
            pauseCreate = true;

//        // 1. gradient: get random color for all gradient
//        float RandomNorm = ofRandom(0., 1.);
//        color.set(getColorAtPercent(RandomNorm));

// 2. palette color: get one of the palette colors
//        color.set(palette[iColor]);//TODO
        //TODO
        color.set(ofRandom(255), ofRandom(255), ofRandom(255));

        iColor++;
//        int palSize = palette.size();
        int palSize = palette->size();
        iColor = iColor % palSize;

        this->colors.push_back(color);

// sort each circle/color speed
//            this->velocities.push_back(glm::normalize( glm::vec2(palSize/(float)iColor * ofRandom(-1, 1), ofRandom(-1, 1)) ));
    }

    for (int i = this->locations.size() - 1; i > -1; i--) {

        this->locations[i] += this->velocities[i];

// distance to erase circles outside screen
//        int maxCirclesDist = 720;
        int maxCirclesDist = 1440;

        if (glm::length(this->locations[i]) > maxCirclesDist) {

            this->locations.erase(this->locations.begin() + i);
            this->velocities.erase(this->velocities.begin() + i);
            this->colors.erase(this->colors.begin() + i);
        }
    }
}