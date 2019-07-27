#ifndef __example_basic__ButtonExample__
#define __example_basic__ButtonExample__

#include <stdio.h>
#include "ofMain.h"

#include "ofxInterface.h"

using namespace ofxInterface;

class ButtonExample : public ofxInterface::Node
{

public:

    //pointer to the external color that will be updated onClick callback
    void setup_colorBACK(ofFloatColor &c);
    ofFloatColor *colorBACK;
//    ofParameter<ofFloatColor>  *colorBACK_param;//

	void setup(float x, float y, float w, float h);
	void update();
	void draw();

	void onTouchDown(TouchEvent& event);
	void onTouchMove(TouchEvent& event);
	void onTouchUp(TouchEvent& event);

    ofColor color;
    ofColor color_picked;
    ofColor color_over;
    ofColor color_down;

    ofVec2f touchAnchor;
	bool bTouched;

	bool bLocked = false;
    void setLocked(bool b);

    void setColor(ofColor c);
    ofColor getColor();

    bool bIsSelected = false;
    void setSelected(bool b);
    bool bIsSelectable = false;
    void setSelectable(bool b);
};

#endif /* defined(__example_basic__ButtonExample__) */
