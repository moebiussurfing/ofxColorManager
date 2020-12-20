#pragma once

#include <vector>
#include "ofVec2f.h"
#include "ofRectangle.h"
#include "ofColor.h"
#include "ofEvents.h"


// ofxControlPoint
//--------------------------------------------------------------
class ofxControlPoint : public ofVec2f
{
public:
    ofxControlPoint(float _x, float _y);
    ofxControlPoint(ofVec2f _vec);

    void setHover(bool _b);
    bool isHover();

    void select(bool _b);
    bool isSelected();

private:
    bool bHover;
    bool bSelected;
};


// ofxControlPointManager
//--------------------------------------------------------------
class ofxControlPointManager
{
public:

    // check changed
    bool bChanged = false;
    bool hasChanged();

    //get a point
    ofVec2f getPointPosition(int i);

    //---

    ofxControlPointManager();

    void setPointSize(float _r);
    float getPointSize();

    void setPointColor(ofColor _c);
    ofColor getPointColor(ofColor _c);

    void setLimitArea(ofRectangle _limitArea);

    void addPoint(float _x, float _y);
    void addPoint(ofVec2f _p);
    void clear();

    std::vector<ofVec2f> getPoints();

    ofVec2f clampMouse(float _x, float _y);

    void keyPressed(ofKeyEventArgs &_data);
    void keyReleased(ofKeyEventArgs &_data);
    void mouseMoved(ofMouseEventArgs &_data);
    void mouseDragged(ofMouseEventArgs &_data);
    void mousePressed(ofMouseEventArgs &_data);
    void mouseReleased(ofMouseEventArgs &_data);

    void enableKeyEvents();
    void disableKeyEvents();

    void enableMouseEvents();
    void disableMouseEvents();

    void draw();
    void drawInfo();

    void saveToCsv(std::string _fileName);
    void loadFromCsv(std::string _fileName);

private:
    float pointSize;
    ofColor pointColor;

    ofRectangle limitArea;

    bool bMoving;

    // drag
    ofVec2f clickedPos;
    ofVec2f lastDragPos;
    ofRectangle dragArea;
    bool bDragging;
    bool bMultiSelecting;

    // key
    bool bCtrlPressed;
    bool bShiftPressed;

    std::vector<ofxControlPoint> controlPoints;
};
