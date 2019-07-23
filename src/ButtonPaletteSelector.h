#ifndef __ofxInterface__ButtonPaletteSelector__
#define __ofxInterface__ButtonPaletteSelector__

#include <stdio.h>
#include "ofMain.h"

#include "ofxInterface.h"

namespace ofxInterface
{

class ButtonPaletteSelector : public Node
{
public:

	void setup(const string& label);
	void draw();

    void onTouchDown(TouchEvent& event);
    void onTouchMove(TouchEvent& event);
    void onTouchUp(TouchEvent& event);

	void setBackground(bool set) { bDrawBackground = set; }
	void setBorder(bool set) { bDrawBorder = set; }

	void setLabelColor(const ofColor& c) { labelColor = c; }
	void setBGColor(const ofColor& c) { bgColor = c; }
	void setBorderColor(const ofColor& c) { borderColor = c; }

	//-

    // pointer back link the outside (ofApp) variable
    void set_SELECTED_palette(int &palette);
    int *SELECTED_palette_pointer;

    //-

    //0:triad, 1:compTriad ... 7:random
    int thisPaletteType = -1;//undefined
    void setThisPaletteType(int palette);

private:

	bool bDrawBackground;
	bool bDrawBorder;

	ofColor borderColor;
	ofColor bgColor;
	ofColor labelColor;

	string label;
};

}	// namespace

#endif /* defined(__ofxUINode__ButtonPaletteSelector__) */
